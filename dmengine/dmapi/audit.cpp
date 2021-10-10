// Copyright (c) 2021 Linux Foundation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#ifndef WIN32
#include <pthread.h>
#endif

#include "platform.h"

#include "audit.h"

#include "autoptr.h"
#include "charptr.h"
#include "dm.h"
#include "model.h"
#include "exceptions.h"
#include "triodbc.h"
#include "transfer.h"
#include "dropzone.h"
#include "repository.h"
#include "thread.h"			// for currentThreadId()
#include "expr.h"

#pragma runtime_checks( "sc", restore )
///////////////////////////////////////////////////////////////////////////////
// Audit
///////////////////////////////////////////////////////////////////////////////

Audit::Audit()
	: m_deployId(0), m_currStepId(0), m_status(NULL)
{}


Audit::~Audit()
{
	SAFE_FREE(m_status);
}

AuditEntry *Audit::newAuditEntry(const char *type)
{
	return new AuditEntry(*this, ++m_currStepId, type);
}


void Audit::startAudit(Environment *env, Application *app, int eventid)
{}


void Audit::finishAudit(int exitCode)
{}


void Audit::recordAction(class Action &action)
{}


void Audit::writevToAuditLog(int stream, long threadId, const char* buffer)
{}


void Audit::writeBufferToAuditLog(int stream, long threadId, const char* buffer, int len)
{}

int Audit::getDeploymentLog(char *logstr, long buflen)
{}


void Audit::setStatus(const char *status)
{
	SAFE_FREE(m_status);
	m_status = strdup(status);
}


void Audit::startAuditEntry(AuditEntry &entry)
{}


void Audit::finishAuditEntry(AuditEntry &entry)
{}


void Audit::recordInstance(AuditEntry &entry, ProviderObjectImpl &inst, Context &ctx)
{}


void Audit::recordTransfer(AuditEntry &entry, Server &target, TransferResult &tr)
{}


void Audit::recordRemoteScript(AuditEntry &entry, Server &target, const char *cmd, int exitCode)
{}


int DatabaseAudit::getDeploymentLog(char *logstr,long buflen)
{
	int deployid = this->deploymentId();
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	int stream;
	char line[2049];
	SQLLEN ni_line = 0;
	sql->BindColumn(1, SQL_INTEGER, &stream, sizeof(stream));
	sql->BindColumn(2, SQL_CHAR, line, sizeof(line), &ni_line);

	SQLRETURN res = sql->ExecuteSQL(
		"SELECT stream, line FROM dm_deploymentlog "
		"WHERE deploymentid=%d ORDER BY runtime", deployid);
	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
		fprintf(stderr, "Deployment log %d not found", deployid);
		return 1;
	}

    strcat(logstr,"<pre><code>");
	for(res = sql->FetchRow(); (res == SQL_SUCCESS) || (res == SQL_SUCCESS_WITH_INFO); res = sql->FetchRow()) {
		char *output = NULL_IND(line, NULL);
		if(output && (strlen(logstr) + strlen(output) < buflen-20)) 
		  strcat(logstr,output);
	}
	strcat(logstr,"</code></pre>");
	sql->CloseSQL();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// DatabaseAudit
///////////////////////////////////////////////////////////////////////////////

DatabaseAudit::DatabaseAudit(int userId, triODBC &odbc)
	: Audit(), m_userId(userId), m_odbc(odbc),
	  m_auditStarted(false), m_auditLog(NULL), m_lineno(0)
{}


DatabaseAudit::~DatabaseAudit()
{
	if(m_auditStarted) {
		finishAudit(-1);
	}

	SAFE_DELETE(m_auditLog);
}


int DatabaseAudit::allocateNewDeployId()
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	sql->SetAutoCommitMode(false);

	long deploymentId;
	sql->BindColumn(1, SQL_INTEGER, &deploymentId, sizeof(deploymentId));

	// Lock the row in our id table so that we can update it
	SQLRETURN res = sql->ExecuteSQL("SELECT nextval FROM dm_sequence WHERE seqname='DEPLOYID' FOR UPDATE");
	if((res == SQL_SUCCESS) || (res == SQL_SUCCESS_WITH_INFO)) {
		res = sql->FetchRow();
		if((res == SQL_SUCCESS) || (res == SQL_SUCCESS_WITH_INFO)) {
			deploymentId++;
			char stmt[256];
			// Increment the id in the table
			AutoPtr<triSQL> upsql = m_odbc.GetSQL();
			sprintf(stmt, "UPDATE dm_sequence SET nextval=%d WHERE seqname='DEPLOYID'", deploymentId);
			res = upsql->ExecuteSQL(stmt);

			if((res == SQL_SUCCESS) || (res == SQL_SUCCESS_WITH_INFO)) {
				// Release the lock
				res = sql->EndTransaction(true);
				sql->CloseSQL();
				upsql->CloseSQL();

				sql->SetAutoCommitMode(true);

				if((res == SQL_SUCCESS) || (res == SQL_SUCCESS_WITH_INFO)) {
					return deploymentId;
				} else {
					throw RuntimeError("EndTransaction failed");
				}
			} else {
				throw RuntimeError("Update failed");
			}
		} else  {
			// No rows - must be the first time we have been run - release the lock
			res = sql->EndTransaction(true);
			// Revert to auto-commit mode
			sql->SetAutoCommitMode(true);
			// Insert starting id in the table
			AutoPtr<triSQL> insql = m_odbc.GetSQL();
			res = insql->ExecuteSQL("INSERT INTO dm_sequence(seqname,nextval) VALUES('DEPLOYID',1)");
			if((res == SQL_SUCCESS) || (res == SQL_SUCCESS_WITH_INFO)) {
				insql->CloseSQL();
				return 1;
			} else {
				throw RuntimeError("Insert failed");
			}
		}
	}
	
	sql->SetAutoCommitMode(true);
	throw RuntimeError("Select for update failed");
}


void DatabaseAudit::startAudit(Environment *env, Application *app, int eventid)
{
	m_deployId = allocateNewDeployId();

	char eventstr[128];

	if (eventid>0) {
		sprintf(eventstr,"%d",eventid);
	} else {
		sprintf(eventstr,"NULL");
	}

	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	//TODO: Remove these when we get auditing working
	/*
	sql->ExecuteSQLIgnoringErrors("DELETE FROM dm_deploymentxfer");
	sql->ExecuteSQLIgnoringErrors("DELETE FROM dm_deploymentscript");
	sql->ExecuteSQLIgnoringErrors("DELETE FROM dm_deploymentprops");
	sql->ExecuteSQLIgnoringErrors("DELETE FROM dm_deploymentstep");
	sql->ExecuteSQLIgnoringErrors("DELETE FROM dm_deploymentlog");
	sql->ExecuteSQLIgnoringErrors("DELETE FROM dm_deploymentaction");
	sql->ExecuteSQLIgnoringErrors("DELETE FROM dm_deployment");
	*/

	SQLRETURN res;
	const char *sessionid = getenv("trisessionid");
	if(sessionid) {
		res = sql->ExecuteSQL("INSERT INTO dm_deployment("
			"deploymentid, userid, startts, started, envid, appid, sessionid,eventid) VALUES ("
			"%d, %d, %s, %ld, %d, %d, '%s', %s)",
			m_deployId, m_userId, m_odbc.getNowCol(), (long) time(NULL),
			(env ? env->id() : 0), (app ? app->id() : 0), sessionid, eventstr);
	} else {
		res = sql->ExecuteSQL("INSERT INTO dm_deployment("
			"deploymentid, userid, startts, started, envid, appid,eventid) VALUES ("
			"%d, %d, %s, %ld, %d, %d, %s)",
			m_deployId, m_userId, m_odbc.getNowCol(), (long) time(NULL),
			(env ? env->id() : 0), (app ? app->id() : 0), eventstr);
	}
	sql->CloseSQL();
	if(res != SQL_SUCCESS) {
		throw RuntimeError("Failed to start audit recording");
	}

	m_auditStarted = true;
}


void DatabaseAudit::finishAudit(int exitCode)
{
	m_auditStarted = false;

	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	SQLRETURN res = sql->PrepareStatement("UPDATE dm_deployment "
		"SET finishts=%s, finished=?, exitcode=?, exitstatus=? "
		"WHERE deploymentid = ?",m_odbc.getNowCol());
	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
		throw RuntimeError("Failed to finish audit recording");
	}
	long finished = time(NULL);
	const char *status = m_status ? m_status : "";
	int statuslen = m_status ? strlen(m_status) : 0;
	debug3("finished=%ld; exitcode=%d; status='%s'; deploymentid=%d", finished, exitCode, status, m_deployId);
	sql->BindParameter(1, SQL_INTEGER, sizeof(finished), &finished, sizeof(finished));
	sql->BindParameter(2, SQL_INTEGER, sizeof(exitCode), &exitCode, sizeof(exitCode));
	sql->BindParameter(3, SQL_CHAR, statuslen, (char*) status, statuslen);
	sql->BindParameter(4, SQL_INTEGER, sizeof(m_deployId), &m_deployId, sizeof(m_deployId));
	res = sql->Execute();
	sql->CloseSQL();
	if(res != SQL_SUCCESS) {
		throw RuntimeError("Failed to finish audit recording");
	}

	//if(m_auditLog) {
	//	struct stat sb;
	//	if(stat(m_auditLog->filename(), &sb) != 0) {
	//		debug1/*WriteToLogFile*/("Failed to insert audit log into database - log not found");
	//		return;
	//	}

	//	// Don't try to load a zero length value into the blob - apparently the ODBC driver hangs/crashes
	//	if(sb.st_size == 0) {
	//		debug1/*WriteToLogFile*/("Failed to insert audit log into database - log has zero length");
	//		return;
	//	}

	//	// Fetch the progress log from its temporary file
	//	int len = sb.st_size;
	//	void *buf = malloc(len);

	//	FILE *log = m_auditLog->open(true);		// read
	//	if(!log) {
	//		debug1/*WriteToLogFile*/("Failed to insert audit log into database - failed to open log");
	//		return;
	//	}

	//	fread(buf, 1, len, log);
	//	fclose(log);

	//	res = sql->PrepareStatement("UPDATE dm_deployment SET log=? WHERE deploymentid=%d", m_deployId);
	//	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
	//		debug1/*WriteToLogFile*/("Failed to insert audit log into database - prepare failed");
	//		free(buf);
	//		return;
	//	}

	//	sql->BindParameter(1, SQL_LONGVARCHAR /*was SQL_BINARY*/, len, buf, len);
	//	SQLRETURN execres = sql->Execute();
	//	if(execres == SQL_NEED_DATA) {
	//		debug1/*WriteToLogFile*/("Insert audit log into database - need data");
	//		execres = sql->ParamData();		// Get to first BLOB column
	//		execres = sql->PutData(buf, len);
	//		execres = sql->ParamData();	// Execute insert
	//	}

	//	sql->CloseSQL();
	//	free(buf);

	//	if((execres != SQL_SUCCESS) && (execres != SQL_SUCCESS_WITH_INFO)) {
	//		debug1/*WriteToLogFile*/("Failed to insert audit log into database - execute failed");
	//	}
	//}
}


void DatabaseAudit::recordAction(class Action &action)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	bool changed = true;	// assume has changed if not recorded

	// Look for a record with the old checksum in it
	char oldchecksum[257];
	SQLLEN ni_oldchecksum = 0;
	sql->BindColumn(1, SQL_CHAR, &oldchecksum, sizeof(oldchecksum), &ni_oldchecksum);
	SQLRETURN res = sql->ExecuteSQL(
		"SELECT da.checksum FROM dm_deploymentaction da "
		"WHERE da.actionid=%d AND da.deploymentid=("
		" SELECT MAX(x.deploymentid) FROM dm_deploymentaction x "
		" WHERE x.actionid=%d)", action.id(), action.id());
	if((res == SQL_SUCCESS) || (res == SQL_SUCCESS_WITH_INFO)) {
		res = sql->FetchRow();
		if((res == SQL_SUCCESS) || (res == SQL_SUCCESS_WITH_INFO)) {
			const char *newcksum = action.getScript()->checksum();
			const char *oldcksum = NULL_IND(oldchecksum, NULL);
			if(oldcksum && newcksum && (strcmp(oldcksum, newcksum) == 0)) {
				// Found it and checksums match, so hasn't changed
				changed = false;
			}
		}
	}
	sql->CloseSQL();
	sql = m_odbc.GetSQL();
	
	res = sql->ExecuteSQL("INSERT INTO dm_deploymentaction("
		"deploymentid, actionid, runtime, changed, checksum) VALUES("
		"%d, %d, %s, '%c', '%s')",
		m_deployId, action.id(),m_odbc.getNowCol(),
		(changed ? 'Y' : 'N'), action.getScript()->checksum());
	sql->CloseSQL();
	if(res != SQL_SUCCESS) {
		throw RuntimeError("Failed to record action");
	}
}


void DatabaseAudit::writevToAuditLog(int stream, long threadId, const char *buffer)
{
	// Keeps trying to print the string into a buffer and increasing the size
	// of the buffer until it fits.
	int size = 1024;
	int len = strlen(buffer);
	char *temp, *np;

	if((temp = (char*) malloc(len+2)) == NULL) {
		return;
	}
	
    *temp = '\0';
	strcpy(temp,buffer);
	// Append a newline as this is implied
	temp[len] = '\n';
	temp[len+1] = '\0';
	len++;

	writeBufferToAuditLog(stream, threadId, temp, len);
	free(temp);
}


void DatabaseAudit::writeBufferToAuditLog(int stream, long threadId, const char* buffer, int len)
{
	// Mod - make this thread safe
	Thread::lock(__LINE__,__FILE__,"writeBufferToAuditLog");
	m_lineno++;
	// Ensure buffer has no unprintable characters in it
	char *p = (char *)buffer;
	for (int i=0;i<len;i++) {
		if (p[i]<=' ' && p[i]!='\r' && p[i]!='\n') p[i]=' ';
	}
	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	SQLRETURN res = sql->PrepareStatement(
		"INSERT INTO dm_deploymentlog(deploymentid,lineno,runtime,stream,thread,line) VALUES(%d,%d,%s,%d,%d,?)",
		m_deployId, m_lineno, m_odbc.getNowCol(), stream, threadId);
	if ((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
		debug1("Failed to insert audit log into database - prepare failed");
	} else {
		sql->BindParameter(1, SQL_LONGVARCHAR /*was SQL_BINARY*/, len, (char*) buffer, len);
		SQLRETURN execres = sql->Execute();
		if(execres == SQL_NEED_DATA) {
			debug1("Insert audit log into database - need data");
			execres = sql->ParamData();		// Get to first BLOB column
			execres = sql->PutData((char*) buffer, len);
			execres = sql->ParamData();	// Execute insert
		}
		sql->CloseSQL();
	}
	Thread::unlock(__LINE__,__FILE__,"writeBufferToAuditLog");
}


void DatabaseAudit::startAuditEntry(AuditEntry &entry)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	SQLRETURN res = sql->ExecuteSQL("INSERT INTO dm_deploymentstep("
		"deploymentid, stepid, type, startts, started, compid) VALUES ("
		"%d, %d, '%s', %s, %ld, %d)", m_deployId, entry.stepId(), entry.type(), m_odbc.getNowCol(), (long) time(NULL), entry.compId());
	sql->CloseSQL();
	if(res != SQL_SUCCESS) {
		throw RuntimeError("Failed to insert audit entry");
	}
}


void DatabaseAudit::finishAuditEntry(AuditEntry &entry)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	SQLRETURN res = sql->ExecuteSQL("UPDATE dm_deploymentstep "
		"SET finishts = %s, finished = %ld WHERE deploymentid = %d AND stepId = %d",
		m_odbc.getNowCol(), (long) time(NULL), m_deployId, entry.stepId());
	sql->CloseSQL();
	if(res != SQL_SUCCESS) {
		char *MsgPtr;
		SQLINTEGER errnum;
		sql->GetLastError(&MsgPtr,&errnum);
		throw RuntimeError("Failed to finish audit entry: %d: %s",errnum,MsgPtr?MsgPtr:"(null)");
	}
}


void DatabaseAudit::recordInstance(AuditEntry &entry, ProviderObjectImpl &inst, Context &ctx)
{
	debug3("In record instance");
	ProviderObjectDef &def = inst.provider().getDef(ctx);

	AutoPtr<triSQL> selsql = m_odbc.GetSQL();
	AutoPtr<triSQL> inssql = m_odbc.GetSQL();
	char propName[DB_NAME_LEN];
	selsql->BindColumn(1, SQL_CHAR, propName, sizeof(propName));
	SQLRETURN res = selsql->ExecuteSQL("SELECT pd.name FROM dm_propertydef pd WHERE pd.defid = %d", def.id());
	debug3("res=%d",res);
	if((res == SQL_SUCCESS) || (res == SQL_SUCCESS_WITH_INFO)) {
		debug3("success, fetching data");
		for(res = selsql->FetchRow(); (res == SQL_SUCCESS) || (res == SQL_SUCCESS_WITH_INFO); res = selsql->FetchRow()) {
			ExprPtr ePropVal = inst.getAttribute(propName, ctx);
	//		debug3("ePropVal=0x%lx",ePropVal);
			ConstCharPtr propVal = ePropVal ? ePropVal->stringify() : NULL;
//			debug3("propVal=0x%lx",propVal);
			//printf("AUDIT: property '%s' = '%s'\n", propName, (propVal ? propVal : "(null)"));

			if(propVal) {
				debug3("About to insert into dm_deploymentprops");
				debug3("propVal=[%s]",(const char *)propVal);
				SQLRETURN insres = inssql->ExecuteSQL("INSERT INTO dm_deploymentprops("
					"deploymentid, stepid, instanceid, name, value) VALUES("
					"%d, %d, %d, '%s', '%s')", m_deployId, entry.stepId(), inst.implId(), propName, (const char*) propVal);
				inssql->CloseSQL();
			}
		}
	}
	selsql->CloseSQL();
}


void DatabaseAudit::recordTransfer(AuditEntry &entry, Server &target, TransferResult &tr)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();

	DropzoneFile *dzf = tr.dropzoneFile();
	if(dzf) {
		char comp[1024];
		if(dzf->item()) {
			ComponentItem &item = *(dzf->item());
			sprintf(comp, "%d, '%s', %d, %d",
				item.component().id(), item.component().name(), item.id(), item.component().buildid());
		} else {
			sprintf(comp, "NULL, NULL, NULL, NULL");
		}

		char repos[1024];
		if(dzf->repoImpl() && dzf->repopath()) {
			RepositoryImpl &repoImpl = *(dzf->repoImpl());
			Repository &repo = repoImpl.repository();
			sprintf(repos, "%d, '%s',%d, '%s', '%s'",
				repo.id(), repo.name(), repoImpl.implId(),
				dzf->repopath(), dzf->version());

			debug1("recordTransfer('%s', %ld, '%s', '%s', '%s')",
				repoImpl.repository().name(), repoImpl.implId(),
				dzf->repopath(), target.name(), tr.targetFilename());
		} else if(dzf->basedOn()) {
			// Find the original file that we are based on
			DropzoneFile *parent;
			for(parent = dzf; parent && parent->basedOn(); parent = parent->basedOn()) { /*empty*/ }
			if(parent->repoImpl() && parent->repopath()) {
				RepositoryImpl &repoImpl = *(parent->repoImpl());
				Repository &repo = repoImpl.repository();
				sprintf(repos, "%d, '%s',%d, '%s', '%s'",
					repo.id(), repo.name(), repoImpl.implId(),
					parent->repopath(), parent->version());

				debug1("recordTransfer('%s', %ld, '%s', '%s', '%s')",
					repoImpl.repository().name(), repoImpl.implId(),
					parent->repopath(), target.name(), tr.targetFilename());
			} else {
				sprintf(repos, "NULL, NULL, NULL, NULL, NULL");

				debug1("'%s' is not based on a repository item", dzf->dropzoneFilename());
			}
		} else {
			sprintf(repos, "NULL, NULL, NULL, NULL, NULL");

			debug1("recordTransfer(NULL, NULL, NULL, '%s', '%s')",
				target.name(), tr.targetFilename());
		}

		SQLRETURN res = sql->ExecuteSQL("INSERT INTO dm_deploymentxfer("
			"deploymentid, stepid, "
			"repoid, reponame, repoinstanceid, repopath, repover, "
			"componentid, componentname, componentitemid, buildnumber, "
			"serverid, servername, targetfilename, "
			"serverhostname, serverprotocol, size1, "
			"size2, checksum2, istext, "
			"created, modified, renamed) VALUES("
			"%d, %d, "
			"%s, %s, "	// repoid to repover, componentid to buildnumber
			"%d, '%s', '%s', "
			"'%s', '%s', %ld, "
			"%ld, '%s', '%c', "
			"'%c', '%c', '%c')",
			m_deployId, entry.stepId(),
			repos, comp,
			target.id(), target.name(), tr.targetFilename(),
			target.hostname(), target.protocol(), dzf->size(),
			tr.size(), tr.md5sum(), (tr.isText() ? 'Y' : 'N'),
			(dzf->isCreated() ? 'Y' : 'N'),
			(dzf->isModified() ? 'Y' : 'N'),
			(dzf->isRenamed() ? 'Y' : 'N'));
	} else {
		debug1("dropzone file was not set on '%s'", tr.dropzoneFilename());
	}

	sql->CloseSQL();
}


void DatabaseAudit::recordRemoteScript(AuditEntry &entry, Server &target, const char *cmd, int exitCode)
{
	AutoPtr<triSQL> sql = m_odbc.GetSQL();
	SQLRETURN res = sql->PrepareStatement("INSERT INTO dm_deploymentscript("
		"deploymentid, stepid, serverid, servername, "
		"serverhostname, serverprotocol, cmd, exitcode) VALUES("
		"?, ?, ?, ?, "
		"?, ?, ?, ?)");
	if((res != SQL_SUCCESS) && (res != SQL_SUCCESS_WITH_INFO)) {
		debug1("Failed to record remote script audit entry");
		return;
	}
	int stepId = entry.stepId();
	int serverId = target.id();
	sql->BindParameter(1, SQL_INTEGER, sizeof(m_deployId), &m_deployId, sizeof(m_deployId));
	sql->BindParameter(2, SQL_INTEGER, sizeof(stepId), &stepId, sizeof(stepId));
	sql->BindParameter(3, SQL_INTEGER, sizeof(serverId), &serverId, sizeof(serverId));
	sql->BindParameter(4, SQL_CHAR, strlen(target.name()), (char*) target.name(), strlen(target.name()));
	sql->BindParameter(5, SQL_CHAR, strlen(target.hostname()), (char*) target.hostname(), strlen(target.hostname()));
	sql->BindParameter(6, SQL_CHAR, strlen(target.protocol()), (char*) target.protocol(), strlen(target.protocol()));
	sql->BindParameter(7, SQL_CHAR, strlen(cmd), (char*) cmd, strlen(cmd));
	sql->BindParameter(8, SQL_INTEGER, sizeof(exitCode), &exitCode, sizeof(exitCode));
	res = sql->Execute();
	sql->CloseSQL();
}


///////////////////////////////////////////////////////////////////////////////
// AuditEntry
///////////////////////////////////////////////////////////////////////////////

AuditEntry::AuditEntry(Audit &audit, int stepId, const char *type)
	: m_audit(audit), m_stepId(stepId), m_type(DUP_NULL(type))
{}


AuditEntry::~AuditEntry()
{
	SAFE_FREE(m_type);
}

void AuditEntry::start()
{
	m_audit.startAuditEntry(*this);
}


void AuditEntry::finish()
{
	m_audit.finishAuditEntry(*this);
}


void AuditEntry::recordInstance(class ProviderObjectImpl &inst, Context &ctx)
{
	m_audit.recordInstance(*this, inst, ctx);
}


void AuditEntry::recordTransferResults(Server &target, List<TransferResult> &results)
{
	ListIterator<TransferResult> iter(results);
	for(TransferResult *tr = iter.first(); tr; tr = iter.next()) {
		m_audit.recordTransfer(*this, target, *tr);
	}
}


void AuditEntry::recordRemoteScript(Server &target, const char *cmd, int exitCode)
{
	m_audit.recordRemoteScript(*this, target, cmd, exitCode);
}
