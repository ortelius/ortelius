/*
 * Copyright (c) 2021 Linux Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __audit_h

#define __audit_h


#include "list.h"


/**
 * This is a dummy class that is used for -noexec and populate
 */
class Audit
{
protected:
	int m_deployId;
	int m_currStepId;
	char *m_status;

	virtual void startAuditEntry(class AuditEntry &entry);
	virtual void finishAuditEntry(class AuditEntry &entry);
	virtual void recordInstance(
		class AuditEntry &entry, class ProviderObjectImpl &inst,
		class Context &ctx);
	virtual void recordTransfer(
		class AuditEntry &entry, class Server &target,
		class TransferResult &tr);
	virtual void recordRemoteScript(
		class AuditEntry &entry, class Server &target,
		const char *cmd, int exitCode);

public:
	Audit();
	virtual ~Audit();

	int deploymentId()  { return m_deployId; }
	int currStepId()	{ return m_currStepId; }

	virtual void startAudit(class Environment *env, class Application *app, int eventid=0);
	virtual void finishAudit(int exitCode);

	AuditEntry *newAuditEntry(const char *type);

	virtual void recordAction(class Action &action);

	virtual void writevToAuditLog(int stream, long threadId, const char *buffer);
	virtual void writeBufferToAuditLog(int stream, long threadId, const char *buffer, int len);
    virtual int getDeploymentLog(char *logstr,long buflen);
	void setStatus(const char *status);

	friend class AuditEntry;
};


/**
 * This is the real class that writes audit information to the database
 */
class DatabaseAudit : public Audit
{
private:
	int m_userId;
	class triODBC &m_odbc;
	bool m_auditStarted;
	class TempFile* m_auditLog;
	int m_lineno;

	int allocateNewDeployId();

protected:
	void startAuditEntry(class AuditEntry &entry);
	void finishAuditEntry(class AuditEntry &entry);
	void recordInstance(
		class AuditEntry &entry, class ProviderObjectImpl &inst,
		class Context &ctx);
	void recordTransfer(
		class AuditEntry &entry, class Server &target,
		class TransferResult &tr);
	void recordRemoteScript(
		class AuditEntry &entry, class Server &target,
		const char *cmd, int exitCode);

public:
	DatabaseAudit(int userId, class triODBC &odbc);
	virtual ~DatabaseAudit();

	void startAudit(class Environment *env, class Application *app, int eventid=0);
	void finishAudit(int exitCode);

	void recordAction(class Action &action);
    void writevToAuditLog(int stream, long threadId, const char *buffer);
	void writeBufferToAuditLog(int stream, long threadId, const char *buffer, int len);
	int getDeploymentLog(char *logstr, long buflen);
};


class AuditEntry
{
private:
	Audit &m_audit;
	int m_stepId;
	char *m_type;

	AuditEntry(Audit &audit, int stepId, const char *type);

public:
	~AuditEntry();

	int m_compId;
	int stepId()  { return m_stepId; }
	int compId()  { return m_compId; }

	const char *type()  { return m_type; }

	void start();
	void finish();

	void recordInstance(
		class ProviderObjectImpl &inst, class Context &ctx);
	void recordTransferResults(
		class Server &target, List<class TransferResult> &results);
	void recordRemoteScript(
		class Server &target, const char *cmd, int exitCode);

	friend class Audit;
};


#endif /*__audit_h*/
