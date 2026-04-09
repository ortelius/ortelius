#!/bin/bash
# test-rbac.sh
# End-to-end test for RBAC synchronization, invitation, and enforcement.

set -e

if [ $# -lt 1 ]; then
	echo "❌ Error: Admin password required"
	echo "Usage: $0 <admin-password> [base-url] [rbac-file]"
	exit 1
fi

ADMIN_PASSWORD="$1"
BASE_URL="${2:-http://localhost:3000/api/v1}"
RBAC_FILE="${3:-config/rbac.yaml}"

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo "🚀 Starting Full End-to-End RBAC Test"
echo "======================================"

# 1. Admin Login
# Path: /api/v1/auth/login
echo "1️⃣  Logging in as bootstrap admin..."
LOGIN_RESPONSE=$(curl -s -c admin_cookies.txt -X POST \
	"${BASE_URL}/auth/login" \
	-H "Content-Type: application/json" \
	-d "{\"username\":\"admin\",\"password\":\"${ADMIN_PASSWORD}\"}")

if ! echo "$LOGIN_RESPONSE" | jq -e '.username' >/dev/null 2>&1; then
	echo -e "${RED}❌ Admin login failed. Response: $LOGIN_RESPONSE${NC}"
	exit 1
fi
echo -e "${GREEN}✅ Admin authenticated as: $(echo "$LOGIN_RESPONSE" | jq -r '.role')${NC}"

# 2. Apply RBAC Configuration
# Path: /api/v1/rbac/apply/content
echo "2️⃣  Applying RBAC from $RBAC_FILE..."
if [ ! -f "$RBAC_FILE" ]; then
	echo -e "${RED}❌ RBAC file not found at $RBAC_FILE${NC}"
	exit 1
fi

RBAC_RESPONSE=$(curl -s -b admin_cookies.txt -X POST \
	"${BASE_URL}/rbac/apply/content" \
	-H "Content-Type: application/x-yaml" \
	--data-binary @"$RBAC_FILE")

if [ "$(echo "$RBAC_RESPONSE" | jq -r '.success // "false"')" != "true" ]; then
	echo -e "${RED}❌ RBAC apply failed. Response: $RBAC_RESPONSE${NC}"
	exit 1
fi

# Summary uses the .summary object from the response
INVITED_COUNT=$(echo "$RBAC_RESPONSE" | jq -r '.summary.invited')
echo -e "${GREEN}✅ RBAC applied. Users invited: $INVITED_COUNT${NC}"

# 3. Extract Token and Activate User
# FIX: Path updated to .result.invited[0] to match latest backend
FIRST_USER=$(echo "$RBAC_RESPONSE" | jq -r '.result.invited[0] // empty')

if [ -z "$FIRST_USER" ] || [ "$FIRST_USER" == "null" ]; then
	echo -e "${YELLOW}ℹ️  No new users were invited (they may already exist). Skipping activation test.${NC}"
else
	TEST_PASS="P@sswordValidation123!"
	# FIX: Extracting invitation link from the top-level .invitations map
	INV_LINK=$(echo "$RBAC_RESPONSE" | jq -r --arg u "$FIRST_USER" '.invitations[$u]')
	TOKEN=$(basename "$INV_LINK")

	echo "3️⃣  Activating account for invited user: $FIRST_USER..."
	# Path: /api/v1/invitation/:token/accept
	ACTIVATE_RESPONSE=$(curl -s -X POST \
		"${BASE_URL}/invitation/${TOKEN}/accept" \
		-H "Content-Type: application/json" \
		-d "{\"password\":\"$TEST_PASS\", \"password_confirm\":\"$TEST_PASS\"}")

	if ! echo "$ACTIVATE_RESPONSE" | jq -e '.message' >/dev/null 2>&1; then
		echo -e "${RED}❌ Activation failed for $FIRST_USER. Response: $ACTIVATE_RESPONSE${NC}"
		exit 1
	fi
	echo -e "${GREEN}✅ Account activated for $FIRST_USER${NC}"

	# 4. Validate Login and RBAC Enforcement
	echo "4️⃣  Validating login with new credentials..."
	USER_LOGIN_RESPONSE=$(curl -s -c user_cookies.txt -X POST \
		"${BASE_URL}/auth/login" \
		-H "Content-Type: application/json" \
		-d "{\"username\":\"${FIRST_USER}\",\"password\":\"${TEST_PASS}\"}")

	if echo "$USER_LOGIN_RESPONSE" | jq -e '.username' >/dev/null 2>&1; then
		USER_ROLE=$(echo "$USER_LOGIN_RESPONSE" | jq -r '.role')
		echo -e "${GREEN}✅ Login successful. User: $FIRST_USER, Role: $USER_ROLE${NC}"

		# 5. Verifying RBAC enforcement
		# Path: /api/v1/rbac/config (Admin Only)
		echo "5️⃣  Verifying RBAC enforcement (Access to /rbac/config)..."
		HTTP_CODE=$(curl -s -o /dev/null -w "%{http_code}" -b user_cookies.txt "${BASE_URL}/rbac/config")

		if [ "$USER_ROLE" != "admin" ]; then
			if [ "$HTTP_CODE" == "403" ]; then
				echo -e "${GREEN}✅ Enforcement verified: Access correctly denied (403)${NC}"
			else
				echo -e "${RED}❌ Security Failure: Access was NOT denied for role $USER_ROLE (HTTP $HTTP_CODE)${NC}"
				exit 1
			fi
		else
			if [ "$HTTP_CODE" == "200" ]; then
				echo -e "${GREEN}✅ Admin access verified (200)${NC}"
			else
				echo -e "${RED}❌ Admin check failed (HTTP $HTTP_CODE)${NC}"
				exit 1
			fi
		fi
	else
		echo -e "${RED}❌ Validation failed: Could not log in as $FIRST_USER${NC}"
		exit 1
	fi
fi

echo ""
echo -e "${GREEN}🎉 Full End-to-End RBAC Test Completed Successfully!${NC}"
rm -f admin_cookies.txt user_cookies.txt
