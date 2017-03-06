/*
 *  DeployHub is an Agile Application Release Automation Solution
 *  Copyright (C) 2017 Catalyst Systems Corporation DBA OpenMake Software
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifdef WIN32
#include "windows.h"
#define HARVEST_EXPORT __declspec(dllexport)
#else
#define HARVEST_EXPORT
#endif /*WIN32*/

#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif /*WIN32*/

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>

#include <openssl/hmac.h>
#include <openssl/bio.h>

#include "harvest.h"
#include "harvestlog.h"

#include "model.h"
#include "dropzone.h"
#include "cmdline.h"
#include "exceptions.h"
#include "expr.h"
#include "charptr.h"
#include "node.h"
#include "context.h"
#include "dm.h"
#include "properties.h"
#include "triodbc.h"


#define HARVEST_PLUGIN_NAME    "harvest"
#define HARVEST_REPO_NAME      "harvest"
#define HARVEST_PKGLOOP_NAME   "harvest_packageloop"
#define HARVEST_SETPKGS_NAME   "harvest_setpackages"
#define HARVEST_CRTPKG_NAME    "harvest_createpackage"
#define HARVEST_PLUGINOBJ_NAME "harvest"
#define HARVEST_CRED_NAME      "harvest"

#define HARVEST_PACKAGE_SCOPE "package"

#define HARVEST_PLUGIN_VERSION "1.00"


///////////////////////////////////////////////////////////////////////////////


unsigned char *loadFileIntoMemory(const char *filename)
{
	struct stat s;
	int t = stat(filename,&s);
	if(t >= 0) {
		int f = open(filename, O_RDONLY);
		if(f >0 ) {
			unsigned char *buffer = (unsigned char*) malloc(s.st_size);
			read(f, buffer, s.st_size);
			close(f);
//dumpbuffer((char*)buffer, s.st_size);
			return buffer;
		}
	}
	return (unsigned char*) 0;
}


long getNativeLength(unsigned char *p)
{
	short a1 = (short)p[0];
	short a2 = (short)p[1];
	short a3 = (short)p[2];
	short a4 = (short)p[3];
#ifdef BIG_ENDIAN
	// e.g.: Sparc
	return (a1<<24)+(a2<<16)+(a3<<8)+a4;
#else
	// e.g.: Intel
	return (a4<<24)+(a3<<16)+(a2<<8)+a1;
#endif
}


long getLength(unsigned char *p)
{
	// Always Big-Endian
	short a1 = (short)p[0];
	short a2 = (short)p[1];
	short a3 = (short)p[2];
	short a4 = (short)p[3];
	return (a1<<24)+(a2<<16)+(a3<<8)+a4;
}


char *decryptHarvestString(unsigned char *key, unsigned char *data)
{
	long nativeLength = getNativeLength(data);
	if(nativeLength == 0) {
		// RHT 31/05/2012 - This is a better test for no password in dfo - windows auth
		return NULL;
	}

//printf("nativeLength = %d\n", nativeLength);

	long totalDataLength = nativeLength + 4;	// include 4 bytes of AA AA AA AA itself
	long IVDataLength = getLength(&(data[4]));
	if(IVDataLength == 0) {
		// RHT 02/03/2012 - no password in dfo - this happens with windows authentication
		return NULL;
	}

	long encryptedDataLength = totalDataLength - (8+IVDataLength);

//printf("totalDataLength = %d, IVDataLength = %d, encryptedDataLength = %d\n", totalDataLength, IVDataLength, encryptedDataLength);

	unsigned char *encryptedString = &(data[8+IVDataLength]);
	unsigned char *iv = &(data[8]);

	BIO *bmem = BIO_new_mem_buf((void*) encryptedString, encryptedDataLength);

	const EVP_CIPHER *cipher = EVP_des_ede3_cbc();
	EVP_CIPHER_CTX *ctx = NULL;
	BIO *benc = BIO_new(BIO_f_cipher());
	BIO_get_cipher_ctx(benc, &ctx);
	if(!EVP_CipherInit_ex(ctx, cipher, NULL, NULL, NULL, 0)) {
		BIO_free_all(bmem);
		throw RuntimeError("ERROR: Failed setting cipher '%s'\n", EVP_CIPHER_name(cipher));
	}
	if(!EVP_CipherInit_ex(ctx, NULL, NULL, key, iv, 0)) {
		BIO_free_all(bmem);
		throw RuntimeError("ERROR: Failed setting key for cipher '%s'\n", EVP_CIPHER_name(cipher));
	}

	bmem = BIO_push(benc, bmem);

	char buffer[1024];
	int rlen = BIO_read(bmem, buffer, sizeof(buffer));
	if(rlen >= 0) {
		buffer[rlen] = '\0';
//printf("decrypted string is '%s'\n", buffer);
		return strdup(buffer);
	} else {
		printf("rlen = %d\n", rlen);
	}

	BIO_free_all(bmem);

	return NULL;
}


void decryptHarvestDfo(const char *filename, char **user, char **pass)
{
	unsigned char *dptr = loadFileIntoMemory(filename);
	if(!dptr) {
		throw RuntimeError("Failed to load '%s'", filename);
	}

	long headerLength = getNativeLength(dptr);
	if(headerLength > 1024) {
		throw RuntimeError("Bad endian");
	}
	unsigned char *userNameBlock = &(dptr[4 + headerLength]);
	long userNameBlockLength = getNativeLength(&(dptr[4 + headerLength]));
	unsigned char *passwordBlock = &(dptr[8 + headerLength + userNameBlockLength]);

	const char *szEncodedSecretKey = "N5IBBFJrGjEIx+BJ+I9uUmi6rWfgPhby";
	BIO *bmem = BIO_new_mem_buf((void*) szEncodedSecretKey, strlen(szEncodedSecretKey));
	BIO *b64 = BIO_new(BIO_f_base64());
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	bmem = BIO_push(b64, bmem);

	unsigned char key[EVP_MAX_KEY_LENGTH];
	if(BIO_read(bmem, key, sizeof(key)) != 24) {
		throw RuntimeError("ERROR: Failed to read input in order to decrypt");
	}

	BIO_free_all(bmem);

	*user = decryptHarvestString(key, userNameBlock);
	*pass = decryptHarvestString(key, passwordBlock);
}


void harvest_handleCredentials(
	ExtendedStmt &stmt, Repository &repo, Credentials *&creds,
	const char *& dfoFile, Context &ctx)
{
	creds = repo.getCredentials();
	if(!creds) {
		throw RuntimeError(stmt, ctx.stack(),
				"No credentials specified for harvest repository");
	}

	switch(creds->kind()) {
	case CREDENTIALS_HARVEST_DFO_IN_FILESYSTEM:
		// Harvest DFO file given - take filename
		dfoFile = creds->filename(ctx);
		break;
	default:
		// Can we turn this credentials object into a Harvest DFO file?
		if(creds->canTransform(HARVEST_CRED_NAME, ctx)) {
			dfoFile = creds->transform(HARVEST_CRED_NAME, repo, ctx);
		}
		break;
	}

	if(!dfoFile && !(creds->hasUsername() && creds->hasPassword())) {
		throw RuntimeError(stmt, ctx.stack(),
				"No usable credentials specified for harvest repository");
	}
}


char *harvest_getBinary(Repository &repo, const char *bin, Context &ctx)
{
	char propname[128];
	sprintf(propname, "%s_bin", bin);
	Property *exe = repo.getDefProperty(propname);
//#ifdef DEV_VERSION
//	// Dev version runs two platforms out of one database - this code allows
//	// two different values to be specified
//	if(!exe) {
//#ifdef WIN32
//		sprintf(propname, "%s_bin_win", bin);
//#else
//		sprintf(propname, "%s_bin_unix", bin);
//#endif /*WIN32*/
//		exe = repo.getDefProperty(propname);
//	}
//#endif /*DEV_VERSION*/
	if(!exe) {
		throw RuntimeError(ctx.stack(),
			"Missing provider definition property '%s_bin' for provider '%s'",
			bin, repo.name());
	}

	// Expand any embedded variables
	Node nexe(NODE_STR, exe->decryptedValue(repo), true);
	ExprPtr eexe = nexe.evaluate(ctx);
	return eexe->stringify();
}


Repository &harvest_getRepository(ExtendedStmt &stmt, Context &ctx)
{
	ConstCharPtr repository = stmt.getArgAsString("repository", ctx);
	if(!repository) {
		throw RuntimeError(stmt, ctx.stack(),
			"No repository specified");
	}
	Repository *repo = ctx.dm().getRepository(repository);
	if(!repo) {
		throw RuntimeError(stmt, ctx.stack(),
			"Repository '%s' not found", (const char*) repository);
	}

	// TODO: If not a harvest repo, throw an exception

	return *repo;
}


bool harvest_packageExists(
	Repository &repo, const char *broker, const char *project,
	const char *pkgname, Context &ctx)
{
	HarvestPluginObject po(&(ctx.dm()),repo, broker, project);
	AutoPtr<HarvestPackage> pkg = po.findPackage(pkgname);
	return pkg ? true : false;
}


bool harvest_createPackage(
	ExtendedStmt &stmt, Repository &repo, const char *broker, const char *project, const char *state,
	const char *pkg, const char *process, const char *dfoFile, Credentials *creds, Context &ctx)
{
	ConstCharPtr hcp_exe = harvest_getBinary(repo, "hcp", ctx);

	char *logFile = NULL;
	FILE *log = ctx.dm().createTemporaryFile(&logFile);
	if(!log) {
		throw RuntimeError(stmt, ctx.stack(),
				"Unable to create log file for harvest repository");
	}
	fclose(log);
	log = NULL;

	char *paramFile = NULL;
	FILE *out = ctx.dm().createTemporaryFile(&paramFile);
	if(!out) {
		throw RuntimeError(stmt, ctx.stack(),
				"Unable to create parameter file for harvest repository");
	}

	fprintf(out, " -b \"%s\"\n", (const char*) broker);
	fprintf(out, " -en \"%s\"\n", (const char*) project);
	if(dfoFile) {
		fprintf(out, " -eh \"%s\"\n", dfoFile);
	} else {
		char *username = creds->getDecryptedUsername(ctx);
		char *password = creds->getDecryptedPassword(ctx);
		fprintf(out, " -usr \"%s\" -pw \"%s\"\n", username, password);
		SECURE_FREE(username);
		SECURE_FREE(password);
	}
	fprintf(out, " -st \"%s\"\n", (const char*) state);
	if(process) {
		fprintf(out, " -pn \"%s\"\n", (const char*) process);
	}
	fprintf(out, " -oa \"%s\"\n", logFile);
	fprintf(out, " \"%s\"\n", (const char*) pkg);
	fclose(out);

	catFile3(paramFile);

	CmdLine cmd(hcp_exe);
	cmd.add("-i").add(paramFile);

	char *pocmd = cmd.toCommandString(true);
	debug1("%s", pocmd);

	//int res = pclose(popen(pocmd, "w"));
	int res = cmd.exec(ctx);
	free(pocmd);

	if(res == -1) {
		throw RuntimeError(stmt, ctx.stack(),
				"Command did not execute successfully");
	}

	catFile2(logFile);
	debug3("res = %d", res);

	return (res == 0) ? true : false;
}


///////////////////////////////////////////////////////////////////////////////
// HarvestOptions
///////////////////////////////////////////////////////////////////////////////

HarvestOptions::HarvestOptions(bool isCheckin)
	: m_isCheckin(isCheckin)
{
	pathOption = PATH_OPTION_NOT_SPECIFIED;
	itemOption = ITEM_OPTION_NOT_SPECIFIED;
	createEmptyDirs = false;
	replaceOption = REPLACE_OPTION_NOT_SPECIFIED;
}


void HarvestOptions::parseOptions(const char *options)
{
	CharPtr opts = strdup(options);
	char *temp = NULL;

	for(char *opt = STRTOK_R(opts, ",", &temp);
		opt; opt = STRTOK_R(NULL, ",", &temp)) {
		if(STRCASECMP(opt, "pc") == 0) {
			pathOption = PATH_OPTION_PRESERVE_AND_CREATE;
		} else if(STRCASECMP(opt, "p") == 0) {
			pathOption = PATH_OPTION_PRESERVE;
		} else if(STRCASECMP(opt, "as") == 0) {
			pathOption = PATH_OPTION_ALL_TO_SAME;
		} else if(m_isCheckin && (STRCASECMP(opt, "ne") == 0)) {
			itemOption = ITEM_OPTION_NEW_AND_EXISTING;
		} else if(m_isCheckin && (STRCASECMP(opt, "eo") == 0)) {
			itemOption = ITEM_OPTION_EXISTING_ONLY;
		} else if(m_isCheckin && (STRCASECMP(opt, "no") == 0)) {
			itemOption = ITEM_OPTION_NEW_ONLY;
		} else if(STRCASECMP(opt, "ced") == 0) {
			createEmptyDirs = true;
		} else if(!m_isCheckin && (STRCASECMP(opt, "ro") == 0)) {
			replaceOption = REPLACE_OPTION_READ_ONLY;
		} else if(!m_isCheckin && (STRCASECMP(opt, "rw") == 0)) {
			replaceOption = REPLACE_OPTION_READ_WRITE;
		} else if(!m_isCheckin && (STRCASECMP(opt, "all") == 0)) {
			replaceOption = REPLACE_OPTION_ALL;
		}
	}
}


void HarvestOptions::generateOptions(FILE *out)
{
	switch(pathOption) {
	case PATH_OPTION_PRESERVE_AND_CREATE: fprintf(out, " -op pc\n"); break;
	case PATH_OPTION_PRESERVE:            fprintf(out, " -op p\n"); break;
	case PATH_OPTION_ALL_TO_SAME:         fprintf(out, " -op as\n"); break;
	}

	if(createEmptyDirs) {
		fprintf(out, " -ced\n");
	}

	if(m_isCheckin) {
		switch(itemOption) {
		case ITEM_OPTION_NEW_AND_EXISTING: fprintf(out, " -if ne\n"); break;
		case ITEM_OPTION_EXISTING_ONLY:    fprintf(out, " -if eo\n"); break;
		case ITEM_OPTION_NEW_ONLY:         fprintf(out, " -if no\n"); break;
		}
	} else {
		switch(replaceOption) {
		case REPLACE_OPTION_READ_ONLY:     fprintf(out, " -replace ro\n"); break;
		case REPLACE_OPTION_READ_WRITE:    fprintf(out, " -replace rw\n"); break;
		case REPLACE_OPTION_ALL:           fprintf(out, " -replace all\n"); break;
		default:                           fprintf(out, " -r\n"); break;
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
// HarvestRepositoryImpl
///////////////////////////////////////////////////////////////////////////////

HarvestRepositoryImpl::HarvestRepositoryImpl(
		RepositoryImplFactory &factory, DMINT32 implId, Repository &repository,
		const char *broker, const char *project,
		const char *state, const char *viewpath,
		const char *options, const char *package,
		const char *process, const char *coprocess
	)
	: RepositoryImpl(factory, implId, repository),
	  m_broker(DUP_NULL(broker)), m_project(DUP_NULL(project)),
	  m_state(DUP_NULL(state)), m_viewpath(DUP_NULL(viewpath)),
	  m_options(DUP_NULL(options)), m_package(DUP_NULL(package)),
	  m_process(DUP_NULL(process)), m_coprocess(DUP_NULL(coprocess))
{
	debug1("HarvestRepositoryImpl('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",
		NULL_CHECK(m_broker), NULL_CHECK(m_project),
		NULL_CHECK(m_state), NULL_CHECK(m_viewpath),
		NULL_CHECK(m_options), NULL_CHECK(m_package),
		NULL_CHECK(m_process), NULL_CHECK(m_coprocess));
}


HarvestRepositoryImpl::~HarvestRepositoryImpl()
{
	SAFE_FREE(m_broker);
	SAFE_FREE(m_project);
	SAFE_FREE(m_state);
	SAFE_FREE(m_viewpath);
	SAFE_FREE(m_options);
	SAFE_FREE(m_package);
	SAFE_FREE(m_process);
	SAFE_FREE(m_coprocess);
}


void HarvestRepositoryImpl::handleCredentials(
		class ExtendedStmt &stmt, class Credentials *&creds,
		const char *& dfoFile, class Context &ctx)
{
	harvest_handleCredentials(stmt, m_repository, creds, dfoFile, ctx);
}


/**
 * Very simple test to make sure that the repository is correctly configured
 * with all the mandatory properties.  Just tests to make sure that "hco"
 * binary is available.
 */
bool HarvestRepositoryImpl::testProvider(class Context &ctx)
{
	if(!m_broker) {
		ctx.writeToStdErr("\"broker\" property must be specified");
		return false;
	}

	ConstCharPtr hco_exe = harvest_getBinary(m_repository, "hco", ctx);

	struct stat buf;
	if(stat(hco_exe, &buf) == -1) {
		ctx.writeToStdErr("Executable '%s' not found", (const char*) hco_exe);
		return false;
	}

	return true;
}


void HarvestRepositoryImpl::checkout(
	const char *dzbase, const char *dzpath,
	bool recursive, StringList *pattern,
	ExtendedStmt &stmt, DropzoneCallback &callback,
	Context &ctx)
{
	// Dropzone: $TEMP/$DEPID.$DEPMAJOR
	// Viewpath: \ or whatever is specified on the repository
	// Username/Password: ???

	HarvestOptions options(false);
	if(m_options) {
		options.parseOptions(m_options);
	}

	ConstCharPtr hco_exe = harvest_getBinary(m_repository, "hco", ctx);

	Credentials *creds = NULL;
	const char *dfoFile = NULL;
	handleCredentials(stmt, creds, dfoFile, ctx);

	char *logFile = NULL;
	FILE *log = ctx.dm().createTemporaryFile(&logFile);
	if(!log) {
		throw RuntimeError(stmt, ctx.stack(),
				"Unable to create log file for harvest repository");
	}
	fclose(log);
	log = NULL;

	char *paramFile = NULL;
	FILE *out = ctx.dm().createTemporaryFile(&paramFile);
	if(!out) {
		throw RuntimeError(stmt, ctx.stack(),
				"Unable to create parameter file for harvest repository");
	}

	fprintf(out, " -b \"%s\"\n", m_broker);
	fprintf(out, " -en \"%s\"\n", m_project);
	if(dfoFile) {
		fprintf(out, " -eh \"%s\"\n", dfoFile);
	} else {
		char *username = creds->getDecryptedUsername(ctx);
		char *password = creds->getDecryptedPassword(ctx);
		fprintf(out, " -usr \"%s\" -pw \"%s\"\n", username, password);
		SECURE_FREE(username);
		SECURE_FREE(password);
	}
	fprintf(out, " -st \"%s\"\n", m_state);
	fprintf(out, " -vp \"%s\"\n", m_viewpath);
	fprintf(out, " -cp \"%s\"\n", dzpath);
	fprintf(out, " -br\n");
	if(m_package) {
		fprintf(out, " -pf \"%s\"\n", m_package);
		fprintf(out, " -po\n");
	}
	if(m_process) {
		fprintf(out, " -pn \"%s\"\n", m_process);
	}
	options.generateOptions(out);
	fprintf(out, " -oa \"%s\"\n", logFile);
	if(recursive) {
		fprintf(out, " -s\n");
	}
	if(pattern) {
		StringListIterator iter(*pattern);
		for(const char *patt = iter.first(); patt; patt = iter.next()) {
			fprintf(out, " \"%s\"\n", patt);
		}
	} else {
		fprintf(out, " \"*\"\n");
	}
	fclose(out);

	catFile3(paramFile);

	CmdLine cmd(hco_exe);
	cmd.add("-i").add(paramFile);

	char *pocmd = cmd.toCommandString(true);
	debug1("%s", pocmd);

	//int res = pclose(popen(pocmd, "w"));
	int res = cmd.exec(ctx);
	free(pocmd);

	if(res == -1) {
		throw RuntimeError(stmt, ctx.stack(),
				"Command did not execute successfully");
	}

	// Record viewpath folders up front, as these all map to the info file for the repository
	callback.checked_out_folder("\\", "\\", true);
	callback.checked_out_folder(m_viewpath, "\\", true);

	// Process the logfile
	struct stat s;
	res = stat(logFile, &s);
	if(res != 0) {
		throw RuntimeError(stmt, ctx.stack(),
				"Failed to stat logfile \"%s\"", logFile);
	}

	if(s.st_size == 0) {
		throw RuntimeError(stmt, ctx.stack(),
				"Logfile \"%s\" is zero length", logFile);
	}

	int total = 0, success = 0, failed = 0, notProcessed = 0;
	bool summaryLineFound = false;
	char buf[2048];

	FILE *in = fopen(logFile, "r");
	if(!in) {
		throw RuntimeError(stmt, ctx.stack(),
				"Failed to open logfile \"%s\"", logFile);
	}

	while(fgets(buf, sizeof(buf), in)) {
		// Trim trailing newlines
		for(int n = strlen(buf)-1; (n >= 0) && (buf[n] == '\r') || (buf[n] == '\n'); n--) {
			buf[n] = '\0';
		}

		HarvestLogEntry entry(buf, ctx);
		if(entry.isError()) {
			ctx.writeToStdOut("%s", buf);
		}
		if(entry.msgkey()) {
			if(strstr(entry.msgkey(), "_I_CHECKOUT_SUMMARY")) {
				summaryLineFound = true;
				callback.checkout_summary(entry.total(), entry.success(), entry.failed(), entry.notProcessed());
				ctx.writeToStdOut("%s", buf);
			} else if(strcmp(entry.msgkey(), "HSRV_I_CHECKOUT") == 0) {
				const char *pn = entry.clientpath() ? strstr(entry.clientpath(), dzbase) : NULL;
				if(pn) {
					pn += strlen(dzbase);
					if(*pn == DIR_SEP_CHAR) {
						pn++;
					}
					callback.checked_out_file(this, entry.viewpath(), pn, entry.mappedversion());
					
					// Now work out the folder the file is in and where it maps to
					ConstCharPtr vpfolder = dirname(entry.viewpath());
					ConstCharPtr dzfolder = Dropzone::dirname(pn);				
					callback.checked_out_folder(
						(vpfolder ? (const char*) vpfolder : m_viewpath),					// TODO: not sure dirname returns null any more
						(dzfolder ? (const char*) dzfolder : DIR_SEP_STR), false);
				}
			} else {
				debug1("msgkey = '%s'; viewpath = '%s'; version = '%s'; clientpath = '%s'",
					entry.msgkey(), NULL_CHECK(entry.viewpath()), NULL_CHECK(entry.mappedversion()),
					NULL_CHECK(entry.clientpath()));
			}
		}

//		if(strncmp(buf, "ERROR:", 6) == 0) {
//			// There was some sort of error with the command line
//			ctx.writeToStdErr("%s", buf);
//			summaryLineFound = true;
//		} else if((strlen(buf) > 11) && (buf[0] == 'E') && (buf[9] == ':') && (buf[10] == ' ')) {
//			// For files that we can recognise, we record an error against them in a DropzoneFile
//// HSRV_E_CHECKOUT_DELETED_VERSION 0x0302015e Could not check out the item %s1; it is deleted.
//// HSRV_E_CICO_VIEWPATH_NOT_VISIBLE 0x03020674 View Path %s1 is invalid or not visible in the current working view.
//// HSRV_E_CHECKOUT_RESERVED_VERSION 0x03020784 Could not check out the item %s1; version has a 'R' tag.
//// HSRV_E_CHECKOUT_MTAGGED_VERSION
//// HSRV_E_ITEM_VERSION_STATUS
//// HSRV_I_SAME_VERSION_NOT_MODIFIED
//// HSRV_E_FA
//// HSRV_E_ITM_MNT_UNMERGED_BRANCH
//// HSRV_E_CHECKOUT_PKG_TRUNK_VERSION_EXIST
//// HSRV_E_DELETED_VERSION_EXIST
//// HSRV_E_RESERVED_VERSION_EXIST
//// HSRV_E_LATER_VERSION_EXIST
//// HSRV_E_CICO_VIEWPATH
//
//			//??? m_repository.fail_checked_out_file(dz, bs, pn, sc, error);
//
//			// Error lines - these give clues to problems
//			ctx.dm().writeToLogFile("%s", buf);
//			// TODO: add reportErrors attribute - need to do something different if summary line not found as well
//			if(/*reportErrors*/ false) {
//				ctx.writeToStdErr("%s", buf);
//			}
//		}
	}
	fclose(in);
	in = NULL;

	if(!summaryLineFound) {
		// There was no summary line - output the entire log file as something has gone wrong
		in = fopen(logFile, "r");
		while (fgets(buf, sizeof(buf), in)) {
			ctx.writeToStdOut("%s", buf);
		}
		fclose(in);
		in = NULL;
	}
}


void HarvestRepositoryImpl::checkin(
	const char *dzbase, const char *dzpath, ExtendedStmt &stmt,
	DropzoneCallback &callback, Context &ctx)
{
	HarvestOptions options(true);
	if(m_options) {
		options.parseOptions(m_options);
	}

	debug1("Harvest checkin('%s', '%s') - viewpath '%s'",
		dzbase, dzpath, (const char*) m_viewpath);

	ConstCharPtr hco_exe = harvest_getBinary(m_repository, "hco", ctx);
	ConstCharPtr hci_exe = harvest_getBinary(m_repository, "hci", ctx);

	Credentials *creds = NULL;
	const char *dfoFile = NULL;
	handleCredentials(stmt, creds, dfoFile, ctx);

	if(!m_package) {
		throw RuntimeError(stmt, ctx.stack(),
				"A package must be specified to checkin to a harvest repository");
	}

	ConstCharPtr description = stmt.getArgAsString("description", ctx);
	ConstCharPtr pattern = stmt.getArgAsString("pattern", ctx);
	if(!pattern) {
		pattern = strdup("*");
	}

	ExprPtr ecreatePackage = stmt.getArg("createpackage", ctx);
	bool createPackage = ecreatePackage ? ecreatePackage->toBool() : false;

	if(createPackage) {
		if(!harvest_packageExists(m_repository, m_broker, m_project, m_package, ctx)) {
			if(!harvest_createPackage(
					stmt, m_repository, m_broker, m_project, m_state,
					m_package, NULL, dfoFile, creds, ctx)) {
				throw RuntimeError(stmt, ctx.stack(),
						"Failed to create package \"%s\"", m_package);
			}
		}
	}

	// If new and existing or existing only, reserve items matching pattern
	if((options.itemOption == ITEM_OPTION_EXISTING_ONLY)
		|| (options.itemOption == ITEM_OPTION_NEW_AND_EXISTING)) {
		char *logFile = NULL;
		FILE *log = ctx.dm().createTemporaryFile(&logFile);
		if(!log) {
			throw RuntimeError(stmt, ctx.stack(),
					"Unable to create log file for harvest repository");
		}
		fclose(log);
		log = NULL;

		char *paramFile = NULL;
		FILE *out = ctx.dm().createTemporaryFile(&paramFile);
		if(!out) {
			throw RuntimeError(stmt, ctx.stack(),
					"Unable to create parameter file for harvest repository");
		}

		fprintf(out, " -b \"%s\"\n", m_broker);
		fprintf(out, " -en \"%s\"\n", m_project);
		if(dfoFile) {
			fprintf(out, " -eh \"%s\"\n", dfoFile);
		} else {
			char *username = creds->getDecryptedUsername(ctx);
			char *password = creds->getDecryptedPassword(ctx);
			fprintf(out, " -usr \"%s\" -pw \"%s\"\n", username, password);
			SECURE_FREE(username);
			SECURE_FREE(password);
		}
		fprintf(out, " -st \"%s\"\n", m_state);
		fprintf(out, " -vp \"%s\"\n", m_viewpath);
		fprintf(out, " -cp \"%s\"\n", dzpath);
		fprintf(out, " -ro\n");
		fprintf(out, " -p \"%s\"\n", m_package);
		if(m_coprocess) {
			fprintf(out, " -pn \"%s\"\n", m_coprocess);
		}
		fprintf(out, " -oa \"%s\"\n", logFile);
		fprintf(out, " -s \"%s\"\n", (const char*) pattern);
		fclose(out);

		catFile3(paramFile);

		CmdLine cmd(hco_exe);
		cmd.add("-i").add(paramFile);

		char *pocmd = cmd.toCommandString(true);
		debug1("%s", pocmd);

		//int res = pclose(popen(pocmd, "w"));
		int res = cmd.exec(ctx);
		free(pocmd);

		catFile2(logFile);

		if(res == -1) {
			throw RuntimeError(stmt, ctx.stack(),
					"Command did not execute successfully");
		}
	}

	// Do the checkin
	char *logFile = NULL;
	FILE *log = ctx.dm().createTemporaryFile(&logFile);
	if(!log) {
		throw RuntimeError(stmt, ctx.stack(),
				"Unable to create log file for harvest repository");
	}
	fclose(log);
	log = NULL;

	char *paramFile = NULL;
	FILE *out = ctx.dm().createTemporaryFile(&paramFile);
	if(!out) {
		throw RuntimeError(stmt, ctx.stack(),
				"Unable to create parameter file for harvest repository");
	}

	fprintf(out, " -b \"%s\"\n", m_broker);
	fprintf(out, " -en \"%s\"\n", m_project);
	if(dfoFile) {
		fprintf(out, " -eh \"%s\"\n", dfoFile);
	} else {
		char *username = creds->getDecryptedUsername(ctx);
		char *password = creds->getDecryptedPassword(ctx);
		fprintf(out, " -usr \"%s\" -pw \"%s\"\n", username, password);
		SECURE_FREE(username);
		SECURE_FREE(password);
	}
	fprintf(out, " -st \"%s\"\n", m_state);
	fprintf(out, " -vp \"%s\"\n", m_viewpath);
	fprintf(out, " -cp \"%s\"\n", dzpath);
	fprintf(out, " -ur\n");
	fprintf(out, " -p \"%s\"\n", m_package);
	if(m_process) {
		fprintf(out, " -pn \"%s\"\n", m_process);
	}
	if(description) {
		fprintf(out, " -de \"%s\"\n", (const char*) description);
	}
	options.generateOptions(out);
	fprintf(out, " -oa \"%s\"\n", logFile);
	fprintf(out, " -s \"%s\"\n", (const char*) pattern);
	fclose(out);

	catFile3(paramFile);

	CmdLine cmd(hci_exe);
	cmd.add("-i").add(paramFile);

	char *pocmd = cmd.toCommandString(true);
	debug1("%s", pocmd);

	//int res = pclose(popen(pocmd, "w"));
	int res = cmd.exec(ctx);
	free(pocmd);

	if(res == -1) {
		throw RuntimeError(stmt, ctx.stack(),
				"Command did not execute successfully");
	}

	catFile1(logFile);

	// If new and existing or existing only, release all items in package
	if((options.itemOption == ITEM_OPTION_EXISTING_ONLY)
		|| (options.itemOption == ITEM_OPTION_NEW_AND_EXISTING)) {
		char *logFile = NULL;
		FILE *log = ctx.dm().createTemporaryFile(&logFile);
		if(!log) {
			throw RuntimeError(stmt, ctx.stack(),
					"Unable to create log file for harvest repository");
		}
		fclose(log);
		log = NULL;

		char *paramFile = NULL;
		FILE *out = ctx.dm().createTemporaryFile(&paramFile);
		if(!out) {
			throw RuntimeError(stmt, ctx.stack(),
					"Unable to create parameter file for harvest repository");
		}

		fprintf(out, " -b \"%s\"\n", m_broker);
		fprintf(out, " -en \"%s\"\n", m_project);
		if(dfoFile) {
			fprintf(out, " -eh \"%s\"\n", dfoFile);
		} else {
			char *username = creds->getDecryptedUsername(ctx);
			char *password = creds->getDecryptedPassword(ctx);
			fprintf(out, " -usr \"%s\" -pw \"%s\"\n", username, password);
			SECURE_FREE(username);
			SECURE_FREE(password);
		}
		fprintf(out, " -st \"%s\"\n", m_state);
		fprintf(out, " -vp \"%s\"\n", m_viewpath);
		//fprintf(out, " -cp \"%s\"\n", dzpath);
		fprintf(out, " -ro\n");
		fprintf(out, " -p \"%s\"\n", m_package);
		if(m_process) {
			fprintf(out, " -pn \"%s\"\n", m_process);
		}
		//if(description) {
		//	fprintf(out, " -de \"%s\"\n", (const char*) description);
		//}
		//options.generateOptions(out);
		fprintf(out, " -oa \"%s\"\n", logFile);
		fprintf(out, " -s \"%s\"\n", (const char*) pattern);
		fclose(out);

		catFile3(paramFile);

		CmdLine cmd(hci_exe);
		cmd.add("-i").add(paramFile);

		char *pocmd = cmd.toCommandString(true);
		debug1("%s", pocmd);

		//int res = pclose(popen(pocmd, "w"));
		int res = cmd.exec(ctx);
		free(pocmd);

		if(res == -1) {
			throw RuntimeError(stmt, ctx.stack(),
					"Command did not execute successfully");
		}

		catFile2(logFile);
	}

	// Now process the checkin log
	struct stat s;
	res = stat(logFile, &s);
	if(res != 0) {
		throw RuntimeError(stmt, ctx.stack(),
				"Failed to stat logfile \"%s\"", logFile);
	}

	if(s.st_size == 0) {
		throw RuntimeError(stmt, ctx.stack(),
				"Logfile \"%s\" is zero length", logFile);
	}

	int total = 0, success = 0, failed = 0, notProcessed = 0;
	bool summaryLineFound = false;
	char buf[2048];

	FILE *in = fopen(logFile, "r");
	if(!in) {
		throw RuntimeError(stmt, ctx.stack(),
				"Failed to open logfile \"%s\"", logFile);
	}

	while(fgets(buf, sizeof(buf), in)) {
		// Trim trailing newlines
		for(int n = strlen(buf)-1; (n >= 0) && (buf[n] == '\r') || (buf[n] == '\n'); n--) {
			buf[n] = '\0';
		}

		HarvestLogEntry entry(buf, ctx);
		if(entry.isError()) {
			ctx.writeToStdOut("%s", buf);
		}
		if(entry.msgkey()) {
			if(strstr(entry.msgkey(), "_I_CHECKIN_SUMMARY")) {
				summaryLineFound = true;
				callback.checkin_summary(entry.total(), entry.success(), entry.failed(), entry.notProcessed());
				ctx.writeToStdOut("%s", buf);
			} else if((strcmp(entry.msgkey(), "HSRV_I_CHECKIN") == 0)
					  || (strcmp(entry.msgkey(), "HSRV_I_CHECKIN_NO_CHANGES") == 0)) {
				const char *fn = entry.clientpath() ? strstr(entry.clientpath(), dzbase) : NULL;
				if(fn) {
					fn += strlen(dzbase);
					if(*fn == DIR_SEP_CHAR) {
						fn++;
					}
					callback.checked_in_file(entry.viewpath(), fn, entry.mappedversion());
					
					// Now work out the folder the file is in and where it maps to
					ConstCharPtr vpfolder = entry.viewpath() ? dirname(entry.viewpath()) : strdup("\\");
					ConstCharPtr dzfolder = Dropzone::dirname(fn);				
					callback.checked_out_folder(
						(vpfolder ? (const char*) vpfolder : m_viewpath),					// TODO: not sure dirname returns null any more
						(dzfolder ? (const char*) dzfolder : DIR_SEP_STR), false);
				}
			} else if(entry.clientpath()) {
				// If we have parsed out the clientpath, record the message against that file
				const char *fn = strstr(entry.clientpath(), dzbase);
				if(fn) {
					fn += strlen(dzbase);
					if(*fn == DIR_SEP_CHAR) {
						fn++;
					}
					callback.checkin_file_error(fn, buf);
				}
			} else {
				debug1("msgkey = '%s'; viewpath = '%s'; version = '%s'; clientpath = '%s'",
					entry.msgkey(), NULL_CHECK(entry.viewpath()), NULL_CHECK(entry.mappedversion()),
					NULL_CHECK(entry.clientpath()));
			}
		}

		if(strncmp(buf, "ERROR:", 6) == 0) {
			// There was some sort of error with the command line
			ctx.writeToStdErr("%s", buf);
			summaryLineFound = true;
		}
	}

	fclose(in);
	in = NULL;

	if(!summaryLineFound) {
		// There was no summary line - output the entire log file as something has gone wrong
		in = fopen(logFile, "r");
		while (fgets(buf, sizeof(buf), in)) {
			ctx.writeToStdOut("%s", buf);
		}
		fclose(in);
		in = NULL;
	}
}


char *HarvestRepositoryImpl::dirname(const char *filename)
{
	const char *ls = strrchr(filename, '\\');
	if(!ls) {
		return NULL;
	}

	int len = ls - filename;

	char *ret = (char*) malloc(len + 1);
	strncpy(ret, filename, len);
	ret[len] = '\0';
	return ret;
}


Expr *HarvestRepositoryImpl::getAttribute(const char *name, Context &ctx)
{
	if(strcmp(name, "broker") == 0) {
		return new Expr(m_broker);
	} else if(strcmp(name, "project") == 0) {
		return new Expr(m_project);
	} else if(strcmp(name, "state") == 0) {
		return new Expr(m_state);
	} else if(strcmp(name, "viewpath") == 0) {
		return new Expr(m_viewpath);
	} else if(strcmp(name, "options") == 0) {
		return m_options ? new Expr(m_options) : NULL;
	} else if(strcmp(name, "package") == 0) {
		return m_package ? new Expr(m_package) : NULL;
	} else if(strcmp(name, "process") == 0) {
		return m_process ? new Expr(m_process) : NULL;
	} else if(strcmp(name, "coprocess") == 0) {
		return m_coprocess ? new Expr(m_coprocess) : NULL;
	}

	return RepositoryImpl::getAttribute(name, ctx);
}


///////////////////////////////////////////////////////////////////////////////
// HarvestRepositoryImplFactory
///////////////////////////////////////////////////////////////////////////////

HarvestRepositoryImplFactory::HarvestRepositoryImplFactory()
{}


RepositoryImpl *HarvestRepositoryImplFactory::create(
	DMINT32 implId, Repository &parent, ExtendedStmt &stmt, Context &ctx)
{
	// snapshot?

	ConstCharPtr broker    = stmt.getOverridableArgAsString("broker",  parent, true, ctx);
	ConstCharPtr project   = stmt.getOverridableArgAsString("project", parent, true, ctx);
	ConstCharPtr state     = stmt.getOverridableArgAsString("state",   parent, true, ctx);
	ConstCharPtr viewpath  = stmt.getOverridableAppendableArgAsString("viewpath", '\\', '\\', parent, false, ctx);
	if(!viewpath) {
		viewpath = strdup("\\");
	}
	ConstCharPtr options   = stmt.getOverridableArgAsString("options",  parent, false, ctx);
	ConstCharPtr package   = stmt.getOverridableArgAsString("package",  parent, false, ctx);
	ConstCharPtr process   = stmt.getOverridableArgAsString("process",  parent, false, ctx);
	ConstCharPtr coprocess = stmt.getOverridableArgAsString("coprocess", parent, false, ctx);	// for checkin only

	return new HarvestRepositoryImpl(*this, implId, parent, broker, project, state, viewpath, options, package, process, coprocess);
}


///////////////////////////////////////////////////////////////////////////////
// Harvest_SetPackagesStmtImpl
///////////////////////////////////////////////////////////////////////////////

Harvest_SetPackagesStmtImpl::Harvest_SetPackagesStmtImpl(ExtendedStmt &parent)
	: m_parent(parent)
{}


void Harvest_SetPackagesStmtImpl::execute(class Context &ctx)
{
	Repository &repo = harvest_getRepository(m_parent, ctx);

	ConstCharPtr broker  = m_parent.getOverridableArgAsString("broker",  repo, true, ctx);
	ConstCharPtr project = m_parent.getOverridableArgAsString("project", repo, true, ctx);

	ExprPtr epkgs = m_parent.getArg("packages", ctx);
	if(!epkgs) {
		throw RuntimeError(m_parent, ctx.stack(),
				"\"packages\" argument must be specified for harvest_packageloop");
	}

	if((epkgs->kind() != KIND_STR) && (epkgs->kind() != KIND_ARRAY)) {
		throw RuntimeError(m_parent, ctx.stack(),
				"\"packages\" argument must be a string or an array for harvest_packageloop");
	}

	// We set the plugin object here so that we can use it in subsequent calls to package loop
	HarvestPluginObject *po = new HarvestPluginObject(&(ctx.dm()),repo, broker, project);
	ctx.dm().setPluginObject(HARVEST_PLUGINOBJ_NAME, po);

	if(epkgs->kind() == KIND_ARRAY) {
		DMArray *arr = epkgs->toArray();
		AutoPtr<StringList> keys = (arr ? arr->keys() : NULL);
		if(keys) {
			StringListIterator iter(*keys);
			for(const char *key = iter.first(); key; key = iter.next()) {
				Variable *var = arr->get(key);
				ConstCharPtr pkg = (var ? var->toString() : NULL);
				if(!pkg) { 
					throw RuntimeError(m_parent, ctx.stack(),
						"\"packages\" argument must be an array of strings for harvest_setpackage");
				}

				HarvestPackage *p = po->findPackage(pkg);
				if(p) {
					po->addPackage(p);
				} else {
					throw RuntimeError("Failed to find package '%s'", (const char *)pkg);
				}
			}
		}
	} else {
		ConstCharPtr pkgs = epkgs->toString();
	}

	po->disconnectFromDatabase();
}


///////////////////////////////////////////////////////////////////////////////
// Harvest_SetPackagesStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

Harvest_SetPackagesStmtImplFactory::Harvest_SetPackagesStmtImplFactory()
{}


ExtendedStmtImpl *Harvest_SetPackagesStmtImplFactory::create(class DM &dm, ExtendedStmt &parent)
{
	return new Harvest_SetPackagesStmtImpl(parent);
}


///////////////////////////////////////////////////////////////////////////////
// Harvest_CreatePackageStmtImpl
///////////////////////////////////////////////////////////////////////////////

Harvest_CreatePackageStmtImpl::Harvest_CreatePackageStmtImpl(ExtendedStmt &parent)
	: m_parent(parent)
{}


void Harvest_CreatePackageStmtImpl::execute(class Context &ctx)
{
	Repository &repo = harvest_getRepository(m_parent, ctx);

	ConstCharPtr broker  = m_parent.getOverridableArgAsString("broker",  repo, true, ctx);
	ConstCharPtr project = m_parent.getOverridableArgAsString("project", repo, true, ctx);
	ConstCharPtr state   = m_parent.getOverridableArgAsString("state",   repo, true, ctx);

	ConstCharPtr pkgname = m_parent.getArgAsString("name", ctx);
	if(!pkgname) {
		throw RuntimeError(m_parent, ctx.stack(),
				"\"name\" argument must be specified for harvest_createpackage");
	}

	ConstCharPtr process = m_parent.getArgAsString("process", ctx);

	const char *dfoFile = NULL;
	Credentials *creds;
	harvest_handleCredentials(m_parent, repo, creds, dfoFile, ctx);

	if(harvest_packageExists(repo, broker, project, pkgname, ctx)) {
		throw RuntimeError(m_parent, ctx.stack(),
				"A package with the name \"%s\" already exists", (const char*) pkgname);
	}

	if(!harvest_createPackage(m_parent, repo, broker, project, state, pkgname, process, dfoFile, creds, ctx)) {
		throw RuntimeError(m_parent, ctx.stack(),
				"Failed to create package \"%s\"", (const char*) pkgname);
	}
}


///////////////////////////////////////////////////////////////////////////////
// Harvest_CreatePackageStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

Harvest_CreatePackageStmtImplFactory::Harvest_CreatePackageStmtImplFactory()
{}


ExtendedStmtImpl *Harvest_CreatePackageStmtImplFactory::create(class DM &dm, ExtendedStmt &parent)
{
	return new Harvest_CreatePackageStmtImpl(parent);
}


///////////////////////////////////////////////////////////////////////////////
// Harvest_PackageLoopStmtImpl
///////////////////////////////////////////////////////////////////////////////

Harvest_PackageLoopStmtImpl::Harvest_PackageLoopStmtImpl(ExtendedStmt &parent)
	: m_parent(parent)
{}


void Harvest_PackageLoopStmtImpl::execute(class Context &ctx)
{
	HarvestPluginObject *po = (HarvestPluginObject*) ctx.dm().getPluginObject(HARVEST_PLUGINOBJ_NAME);
	if(!po) {
		throw RuntimeError(m_parent, ctx.stack(),
				"harvest_setpackages must be called before using harvest_packageloop");
	}

	ListIterator<HarvestPackage> iter(po->packages());
	for(HarvestPackage *pkg = iter.first(); pkg; pkg = iter.next()) {
		bool doPush = !ctx.stack().contains(HARVEST_PACKAGE_SCOPE);
		if(doPush) {
			Scope *scope = pkg->getVars();
			scope->set("TRIDM_PACKAGE", pkg->name());
			ctx.stack().push(scope);
		}

		try {
			m_parent.executeBody(ctx);
		} catch(...) {
			if(doPush) {
				ctx.stack().pop(HARVEST_PACKAGE_SCOPE);
			}
			throw;
		}

		if(doPush) {
			ctx.stack().pop(HARVEST_PACKAGE_SCOPE);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
// Harvest_PackageLoopStmtImplFactory
///////////////////////////////////////////////////////////////////////////////

Harvest_PackageLoopStmtImplFactory::Harvest_PackageLoopStmtImplFactory()
{}


ExtendedStmtImpl *Harvest_PackageLoopStmtImplFactory::create(class DM &dm, ExtendedStmt &parent)
{
	return new Harvest_PackageLoopStmtImpl(parent);
}


///////////////////////////////////////////////////////////////////////////////
// HarvestCredentialsProvider
///////////////////////////////////////////////////////////////////////////////

HarvestCredentialsProvider::HarvestCredentialsProvider(
		class CredentialsProviderFactory &factory,
		class Credentials &credentials
	)
	: CredentialsProvider(factory, credentials), m_dfoFileName(NULL)
{}


HarvestCredentialsProvider::~HarvestCredentialsProvider()
{
	SAFE_FREE(m_dfoFileName);
}


bool HarvestCredentialsProvider::canTransform()
{
	switch(m_credentials.kind()) {
	case CREDENTIALS_ENCRYPTED:
	case CREDENTIALS_FROM_VARS:
		return (m_credentials.hasUsername() && m_credentials.hasPassword());
	case CREDENTIALS_HARVEST_DFO_IN_FILESYSTEM:
		return true;
	}

	return false;
}


const char *HarvestCredentialsProvider::transform(Object &auth, Context &ctx)
{
	debug2("HarvestCredentialsProvider::transform()");
	if(m_dfoFileName) {
		return m_dfoFileName;
	}

	char *dfoFileName;

	FILE *dfo = ctx.dm().createTemporaryFile(&dfoFileName);
	if(dfo) {
		fclose(dfo);
	}

	//char cmd[1024];
	char *dirname = strdup(dfoFileName);
	char *filename = NULL;
	for(int n = strlen(dirname); n > 0; n--) {
#ifdef WIN32
		if(dirname[n] == '\\') {
#else
		if(dirname[n] == '/') {
#endif /*WIN32*/
			dirname[n] = '\0';
			filename = &dirname[n+1];
			break;
		}
	}

	char *username = m_credentials.getDecryptedUsername(ctx);
	char *password = m_credentials.getDecryptedPassword(ctx);

	// auth may not be a repository, so default to using PATH if it isn't
	// TODO: Maybe this should be taken from plugin def props if we ever have such a thing
	ConstCharPtr svrenc_exe;
	if(auth.kind() == OBJ_KIND_REPOSITORY) {
		svrenc_exe = harvest_getBinary((Repository&) auth, "svrenc", ctx);
	} else {
		debug1("Auth is not a repository - using PATH to find svrenc");
		svrenc_exe = strdup("svrenc");
	}
	CmdLine cmd(svrenc_exe);
	cmd.add("-dir").add(dirname).add("-f").add(filename)
		.add("-usr").add(username).add("-pw").add(password);

	//sprintf(cmd, "svrenc -dir \"%s\" -f \"%s\" -usr \"%s\" -pw \"%s\"", dirname, filename, username, password);
	ctx.dm().writeToLogFile("Running svrenc -dir \"%s\" -f \"%s\" -usr \"%s\" -pw \"********\"", dirname, filename, username);
	SECURE_FREE(username);
	SECURE_FREE(password);

	SAFE_FREE(dirname);

	//int ret = system(cmd);
	//memset(cmd, 0, sizeof(cmd));
	int ret = cmd.exec(ctx);

	if(ret != 0) {
		ctx.dm().writeToStdOut/*LogFile*/("Failed to transform credentials into Harvest DFO file");
	}

	m_dfoFileName = strdup(dfoFileName);
	return m_dfoFileName;
}


///////////////////////////////////////////////////////////////////////////////
// HarvestCredentialsProviderFactory
///////////////////////////////////////////////////////////////////////////////


HarvestCredentialsProviderFactory::HarvestCredentialsProviderFactory()
{}


CredentialsProvider *HarvestCredentialsProviderFactory::create(
	class Credentials &parent)
{
	return new HarvestCredentialsProvider(*this, parent);
}


///////////////////////////////////////////////////////////////////////////////
// HarvestUser
///////////////////////////////////////////////////////////////////////////////

HarvestUser::HarvestUser(long id, const char *name, const char *email)
	: m_id(id), m_name(DUP_NULL(name)), m_email(DUP_NULL(email))
{}


HarvestUser::~HarvestUser()
{
	SAFE_FREE(m_name);
	SAFE_FREE(m_email);
}


Expr *HarvestUser::getAttribute(const char *name, Context &ctx)
{
	if(strcmp(name, "name") == 0) {
		return new Expr(m_name);
	}
	if(strcmp(name, "email") == 0) {
		return m_email ? new Expr(m_email) : NULL;
	}
	return NULL;
}


//class DMArray *HarvestUser::getArrayAttribute(const char *name, class Context &ctx)
//{
//	return NULL;
//}

//class IObject *HarvestUser::getObjectAttribute(const char *name, class Context &ctx)
//{
//	return NULL;
//}


///////////////////////////////////////////////////////////////////////////////
// HarvestPackage
///////////////////////////////////////////////////////////////////////////////

HarvestPackage::HarvestPackage(
		HarvestPluginObject &plugin, long id, const char *name
	)
	: m_plugin(plugin), m_id(id), m_name(DUP_NULL(name)), m_vars(NULL),
	  m_creator(NULL), m_creatorSet(false),
	  m_assignee(NULL), m_assigneeSet(false),
	  m_contributors(NULL), m_contributorsCache(NULL)
{}


HarvestPackage::~HarvestPackage()
{
	SAFE_FREE(m_name);
	SAFE_DELETE(m_vars);
	SAFE_DELETE(m_creator);
	SAFE_DELETE(m_assignee);
	SAFE_DELETE(m_contributors);
	SAFE_DELETE(m_contributorsCache);
}


Expr *HarvestPackage::getAttribute(const char *name, Context &ctx)
{
	if(strcmp(name, "name") == 0) {
		return new Expr(m_name);
	}
	if(strcmp(name, "contributors") == 0) {
		if(m_contributorsCache) {
			return new Expr(m_contributorsCache);
		}
		DMArray *ret = new DMArray(false, false, true);
		ret->addRef();	// This prevents the array being deleted
		List<HarvestUser> *list = getContributors();
		ListIterator<HarvestUser> iter(*list);
		for(HarvestUser *u = iter.first(); u; u = iter.next()) {
			ret->put(u->name(), new Variable(NULL, u));
		}
		m_contributorsCache = ret;
		return new Expr(ret);
	}
	if(strcmp(name, "creator") == 0) {
		HarvestUser *creator = getCreator();
		return creator ? new Expr(creator) : NULL;
	}
	if(strcmp(name, "assignee") == 0) {
		HarvestUser *assignee = getAssignee();
		return assignee ? new Expr(assignee) : NULL;
	}
	return NULL;
}


//class DMArray *HarvestPackage::getArrayAttribute(const char *name, Context &ctx)
//{
//	if(strcmp(name, "contributors") == 0) {
//		if(m_contributorsCache) {
//			return m_contributorsCache;
//		}
//		DMArray *ret = new DMArray(false, false, true);
//		ret->addRef();	// This prevents the array being deleted
//		List<HarvestUser> *list = getContributors();
//		ListIterator<HarvestUser> iter(*list);
//		for(HarvestUser *u = iter.first(); u; u = iter.next()) {
//			ret->put(u->name(), new Variable(NULL, u));
//		}
//		m_contributorsCache = ret;
//		return ret;
//	}
//	return NULL;
//}


//IObject *HarvestPackage::getObjectAttribute(const char *name, Context &ctx)
//{
//	if(strcmp(name, "creator") == 0) {
//		return getCreator();
//	}
//	if(strcmp(name, "assignee") == 0) {
//		return getAssignee();
//	}
//	return NULL;
//}


class Scope *HarvestPackage::getVars()
{
	if(!m_vars) {
		m_vars = new Scope(HARVEST_PACKAGE_SCOPE, this);
		m_vars->addRef();
	}

	return m_vars;
}


HarvestUser *HarvestPackage::getCreator()
{
	if(!m_creatorSet) {
		m_plugin.getCreator(*this);
	}
	return m_creator;
}


void HarvestPackage::setCreator(HarvestUser *creator)
{
	m_creator = creator;
	m_creatorSet = true;
}


HarvestUser *HarvestPackage::getAssignee()
{
	if(!m_assigneeSet) {
		m_plugin.getAssignee(*this);
	}
	return m_assignee;
}


void HarvestPackage::setAssignee(HarvestUser *assignee)
{
	m_assignee = assignee;
	m_assigneeSet = true;
}


List<HarvestUser> *HarvestPackage::getContributors()
{
	if(!m_contributors) {
		m_plugin.getContributors(*this);
	}
	return m_contributors;
}


void HarvestPackage::addContributor(HarvestUser *contributor)
{
	if(!m_contributors) {
		m_contributors = new List<HarvestUser>(true);
	}
	m_contributors->add(contributor);
}


///////////////////////////////////////////////////////////////////////////////
// HarvestPluginObject
///////////////////////////////////////////////////////////////////////////////

HarvestPluginObject::HarvestPluginObject(
		DM *dm,
		Repository &repository,
		const char *broker,
		const char *project
	)
	: m_repository(repository), m_broker(DUP_NULL(broker)),
	  m_project(DUP_NULL(project)), m_packages(true), m_odbc(NULL), m_dm(dm)
{}


HarvestPluginObject::~HarvestPluginObject()
{
	SAFE_FREE(m_broker);
	SAFE_FREE(m_project);
	SAFE_DELETE(m_odbc);
}


/**
 * This method connects to either the local Harvest database using the
 * datasource specified in HServer.arg and the credentials stored in the
 * server's own DFO file, or to a possibly remote Harvest database using
 * the datasource specified in the "datasource" property on the repository
 * and the credentials from ???.
 */
void HarvestPluginObject::connectToDatabase()
{
	CharPtr DSN = NULL;
	char *user = NULL;
	char *pass = NULL;

	// If repository specifies a datasource, we use simple ODBC, otherwise
	// we look in local Harvest install
	Property *datasource = m_repository.getProperty("datasource");
	if(datasource) {
		DSN = datasource->decryptedValue(m_repository);
		Property *dbuser = m_repository.getProperty("dbuser");
		Property *dbpass = m_repository.getProperty("dbpass");
		user = dbuser ? dbuser->decryptedValue(m_repository) : NULL;
		pass = dbpass ? dbpass->decryptedValue(m_repository) : NULL;
	} else {
		const char *hh = getenv("CA_SCM_HOME");
		hh = hh ? hh : getenv("HARVESTHOME");
		if(!hh) {
			throw RuntimeError("CA_SCM_HOME or HARVESTHOME not set");
		}
		CharPtr filename = (char*) malloc(strlen(hh) + 15);
#ifdef WIN32
		sprintf(filename, "%s\\hsvr.dfo", hh);
#else
		sprintf(filename, "%s/.hsvr.dfo", hh);
#endif /*WIN32*/
		decryptHarvestDfo(filename, &user, &pass);
		debug3("user = '%s'; pass = '%s'", (user ? user : "(null)"), (pass ? pass : "(null)"));

		// Now get the DSN from HServer.arg
		CharPtr hsa = (char*) malloc(strlen(hh) + 15);
		sprintf(hsa, "%s%sHServer.arg", hh, DIR_SEP_STR);

		FILE *in = fopen(hsa, "r");
		if(!in) {
			throw RuntimeError("Could not find or open %s", (const char*) hsa);
		}

		CharPtr broker = NULL;
		char buffer[256];
		while(fgets(buffer, sizeof(buffer), in)) {
			while((buffer[strlen(buffer)-1] == '\r') || (buffer[strlen(buffer)-1] == '\n')) {
				buffer[strlen(buffer)-1] = '\0';
			}
			if(!broker && strstr(buffer,"-broker")) {
				char *ep = strchr(buffer,'=');
				if(ep) {
					broker = strdup(ep + 1);
				}
			} else if(!DSN && strstr(buffer,"-datasource")) {
				char *ep = strchr(buffer,'=');
				if(ep) {
					DSN = strdup(ep + 1);
				}
			}
		}
		fclose(in);

		if(!broker) {
			throw RuntimeError("Could not locate -broker entry in %s", (const char*) hsa);
		}
		if(!DSN) {
			throw RuntimeError("Could not locate -datasource entry in %s", (const char*) hsa);
		}

		if(STRCASECMP(m_broker, broker) != 0) {
			throw RuntimeError("Broker '%s' not found in %s", m_broker, (const char*) hsa);
		}
	}

	// Connect and read Harvest version
	triODBC *odbc = new triODBC();
	long conres = odbc->ConnectToDataSource(m_dm, DSN, user, pass);
	if(conres == SQL_ERROR) {
		throw RuntimeError("Failed to connect to database");
	}

	AutoPtr<triSQL> sql = odbc->GetSQL();
	long versind = 0;
	sql->BindColumn(1, SQL_INTEGER, &versind, sizeof(versind));
	SQLRETURN res = sql->ExecuteSQL("SELECT versionindicator FROM hartableinfo");
	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
		SAFE_DELETE(odbc);
		throw RuntimeError("Does not look like a Harvest database");
	}
	res = sql->FetchRow();
	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
		SAFE_DELETE(odbc);
		throw RuntimeError("Does not look like a Harvest database (2)");
	}
	sql->CloseSQL();

	debug1("Version Indicator = %ld", versind);

	m_odbc = odbc;
}


void HarvestPluginObject::disconnectFromDatabase()
{
	SAFE_DELETE(m_odbc);
}


HarvestPackage *HarvestPluginObject::findPackage(const char *pkg)
{
	if(!m_odbc) {
		connectToDatabase();
	}

	AutoPtr<triSQL> sql = m_odbc->GetSQL();

	long pkgobjid = 0;
	sql->BindColumn(1, SQL_INTEGER, &pkgobjid, sizeof(pkgobjid));
	SQLRETURN res = sql->ExecuteSQL(
			"SELECT p.packageobjid FROM harpackage p, harenvironment e "
			"WHERE p.packagename='%s' AND e.envobjid=p.envobjid "
			"AND e.environmentname='%s'", pkg, m_project);
	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
		return NULL;
	}
	res = sql->FetchRow();
	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
		return NULL;
	}
	sql->CloseSQL();

	debug3("Package '%s' has id %ld", pkg, pkgobjid);

	return new HarvestPackage(*this, pkgobjid, pkg);
}


void HarvestPluginObject::addPackage(HarvestPackage *pkg)
{
	m_packages.add(pkg);
}


List<HarvestPackage> &HarvestPluginObject::packages()
{
	return m_packages;
}


void HarvestPluginObject::getCreator(HarvestPackage &pkg)
{
	if(!m_odbc) {
		connectToDatabase();
	}

	AutoPtr<triSQL> sql = m_odbc->GetSQL();

	long usrobjid = 0;
	char username[33];
	char email[129];
	SQLLEN emailni = 0;
	sql->BindColumn(1, SQL_INTEGER, &usrobjid, sizeof(usrobjid));
	sql->BindColumn(2, SQL_CHAR, &username, sizeof(username));
	sql->BindColumn(3, SQL_CHAR, &email, sizeof(email), &emailni);
	SQLRETURN res = sql->ExecuteSQL(
			"SELECT u.usrobjid, u.username, u.email FROM harallusers u, harpackage p "
			"WHERE u.usrobjid=p.creatorid AND p.packageobjid=%ld", pkg.id());
	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
		throw RuntimeError("Failed to find creator for package '%s'", pkg.name());
	}
	res = sql->FetchRow();
	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
		throw RuntimeError("Failed to find creator for package '%s' (2)", pkg.name());
	}
	pkg.setCreator(new HarvestUser(usrobjid, username, ((emailni > 0) ? email : NULL)));

	sql->CloseSQL();

	debug3("Package '%s' has creator '%s'", pkg.name(), username);

	disconnectFromDatabase();
}


void HarvestPluginObject::getAssignee(HarvestPackage &pkg)
{
	if(!m_odbc) {
		connectToDatabase();
	}

	AutoPtr<triSQL> sql = m_odbc->GetSQL();

	long usrobjid;
	char username[33];
	char email[129];
	SQLLEN emailni = 0;
	sql->BindColumn(1, SQL_INTEGER, &usrobjid, sizeof(usrobjid));
	sql->BindColumn(2, SQL_CHAR, &username, sizeof(username));
	sql->BindColumn(3, SQL_CHAR, &email, sizeof(email), &emailni);
	SQLRETURN res = sql->ExecuteSQL(
			"SELECT u.usrobjid, u.username, u.email FROM harallusers u, harpackage p "
			"WHERE u.usrobjid=p.assigneeid AND p.packageobjid=%ld", pkg.id());
	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
		throw RuntimeError("Failed to find assignee for package '%s'", pkg.name());
	}
	res = sql->FetchRow();
	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
		throw RuntimeError("Failed to find assignee for package '%s' (2)", pkg.name());
	}
	pkg.setAssignee(new HarvestUser(usrobjid, username, ((emailni > 0) ? email : NULL)));

	sql->CloseSQL();

	debug3("Package '%s' has assignee '%s'", pkg.name(), username);

	disconnectFromDatabase();
}


void HarvestPluginObject::getContributors(HarvestPackage &pkg)
{
	if(!m_odbc) {
		connectToDatabase();
	}

	AutoPtr<triSQL> sql = m_odbc->GetSQL();

	long usrobjid;
	char username[33];
	char email[129];
	SQLLEN emailni = 0;
	sql->BindColumn(1, SQL_INTEGER, &usrobjid, sizeof(usrobjid));
	sql->BindColumn(2, SQL_CHAR, &username, sizeof(username));
	sql->BindColumn(3, SQL_CHAR, &email, sizeof(email), &emailni);
	SQLRETURN res = sql->ExecuteSQL(
			"SELECT DISTINCT u.usrobjid, u.username, u.email FROM harallusers u, harversions v "
			"WHERE u.usrobjid=v.creatorid AND v.packageobjid=%ld", pkg.id());
	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
		throw RuntimeError("Failed to find assignee for package '%s'", pkg.name());
	}
	for(res = sql->FetchRow(); ((res == SQL_SUCCESS) || (res == SQL_SUCCESS_WITH_INFO)); res = sql->FetchRow()) {
		debug3("Package '%s' has contributor '%s'", pkg.name(), username);
		pkg.addContributor(new HarvestUser(usrobjid, username, ((emailni > 0) ? email : NULL)));
	}
	//if(res != NO_DATA) {
	//	throw RuntimeError("Failed to find assignee for package '%s' (2)", pkg.name());
	//}

	sql->CloseSQL();

	disconnectFromDatabase();
}


///////////////////////////////////////////////////////////////////////////////

extern "C" HARVEST_EXPORT int PluginStart(DM &dm)
{
	dm.writeToLogFile("Harvest plugin V" HARVEST_PLUGIN_VERSION);
	
	RepositoryImplRegistry::instance()
		.registerFactory(HARVEST_REPO_NAME, new HarvestRepositoryImplFactory());
	ExtendedStmtImplRegistry::instance()
		.registerFactory(HARVEST_PKGLOOP_NAME, new Harvest_PackageLoopStmtImplFactory());
	ExtendedStmtImplRegistry::instance()
		.registerFactory(HARVEST_SETPKGS_NAME, new Harvest_SetPackagesStmtImplFactory());
	ExtendedStmtImplRegistry::instance()
		.registerFactory(HARVEST_CRTPKG_NAME, new Harvest_CreatePackageStmtImplFactory());
	CredentialsProviderRegistry::instance()
		.registerFactory(HARVEST_CRED_NAME, new HarvestCredentialsProviderFactory());
	return 0;
}


extern "C" HARVEST_EXPORT int PluginInstall(DM &dm)
{
	// name, required, appendable
	List<PropertyDef> *propdefs = new List<PropertyDef>(true);
	propdefs->add(new PropertyDef("broker",     true, false ));
	propdefs->add(new PropertyDef("project",    false, false));
	propdefs->add(new PropertyDef("state",      false, false));
	propdefs->add(new PropertyDef("viewpath",   false, true ));
	propdefs->add(new PropertyDef("options",    false, false));
	propdefs->add(new PropertyDef("package",    false, false));
	propdefs->add(new PropertyDef("process",    false, false));
	propdefs->add(new PropertyDef("coprocess",  false, false));
	propdefs->add(new PropertyDef("datasource", false, false));
	propdefs->add(new PropertyDef("dbuser",     false, false));
	propdefs->add(new PropertyDef("dbpass",     false, false));

	dm.installProviderImpl(HARVEST_REPO_NAME, HARVEST_PLUGIN_NAME, OBJ_KIND_REPOSITORY, propdefs);
	return 0;
}
