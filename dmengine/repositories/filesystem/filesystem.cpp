#ifdef WIN32
#include "windows.h"
#define FILESYSTEM_EXPORT __declspec(dllexport)
#else
#define FILESYSTEM_EXPORT
#endif /*WIN32*/

#include <stdio.h>
#include <sys/stat.h>
#ifdef WIN32
#include <direct.h>
#include <sys/utime.h>
#else
#include <sys/time.h>
#endif /*WIN32*/

#include "filesystem.h"

#include "../../dmapi/properties.h"
#include "../../dmapi/model.h"
#include "../../dmapi/dropzone.h"
#include "../../dmapi/expr.h"
#include "../../dmapi/charptr.h"
#include "../../dmapi/node.h"
#include "../../dmapi/exceptions.h"
#include "../../dmapi/filefind.h"
#include "../../dmapi/dm.h"
#include "../../dmapi/context.h"


#define FILESYSTEM_PLUGIN_VERSION "1.00"


///////////////////////////////////////////////////////////////////////////////
// FilesystemRepositoryImpl
///////////////////////////////////////////////////////////////////////////////

FilesystemRepositoryImpl::FilesystemRepositoryImpl(
		RepositoryImplFactory &factory, DMINT32 implId,
		Repository &repository, const char *filepath, const char *version
	)
	: RepositoryImpl(factory, implId, repository),
	  m_filepath(DUP_NULL(filepath)), m_version(DUP_NULL(version))
{
	int i=0;
	int len=0;

		if (m_filepath != NULL)
		{
		 #ifdef WIN32
			 len = strlen(m_filepath);
			 for (i=0;i<len;i++)
				{
					if (m_filepath[i] == '/')
						m_filepath[i] = '\\';
				}
			#else
			 len = strlen(m_filepath);
			 for (i=0;i<len;i++)
				{
					if (m_filepath[i] == '\\')
						m_filepath[i] = '/';
				}				
			#endif		
		}
		debug1("FilesystemRepositoryImpl('%s', '%s')",
			NULL_CHECK(m_filepath), NULL_CHECK(m_version));
}


/**
 * Very simple test to make sure that the repository is correctly configured
 * with all the mandatory properties.  Just tests to make sure that the folder
 * specified by the "filepath" property exists.
 */
bool FilesystemRepositoryImpl::testProvider(class Context &ctx)
{
	if(!m_filepath) {
		ctx.writeToStdErr("\"filepath\" property must be specified");
		return false;
	}

	struct stat buf;
	if(stat(m_filepath, &buf) == -1) {
		ctx.writeToStdErr("The folder '%s' specified by \"filepath\" does not exist", m_filepath);
		return false;
	}

	return true;
}


bool FilesystemRepositoryImpl::copyFile(const char *sourcePath, const char *targetPath)
{
	struct stat sbuf;
	if(stat(sourcePath, &sbuf) < 0) {
		debug1("failed to stat input file ('%s') - not copying", sourcePath);
		return false;
	}
#ifdef WIN32
	struct _utimbuf ubuf;
	ubuf.actime = sbuf.st_atime;
	ubuf.modtime = sbuf.st_mtime;
#else
	struct timeval ubuf[2];
	ubuf[0].tv_sec  = sbuf.st_atime;
	ubuf[0].tv_usec = 0;
	ubuf[1].tv_sec  = sbuf.st_mtime;
	ubuf[1].tv_usec = 0;
#endif /*WIN32*/

	char buf[8096];
	bool ret = true;
#ifdef WIN32
	FILE *fin = fopen(sourcePath, "rb");
	FILE *fout = fopen(targetPath, "wb");
#else
	FILE *fin = fopen(sourcePath, "r");
	FILE *fout = fopen(targetPath, "w");
#endif /*WIN32*/
	if(fin && fout) {
		int n = sizeof(buf);
		while((n > 0) && !feof(fin)) {
			n = fread(buf, 1, sizeof(buf), fin);
			if(fwrite(buf, 1, n, fout) != n) {
				debug1("failed to write all bytes to output file - aborting copy");
				ret = false;
				break;
			}
		}
	} else {
		if(!fin) { debug1("failed to open input file ('%s') - not copying", sourcePath); }
		if(!fout) { debug1("failed to open output file ('%s') - not copying", targetPath); }
		ret = false;
	}
	if(fin) { fclose(fin); }
	if(fout) { fclose(fout); }

	// Preserve timestamps
#ifdef WIN32
	if(_utime(targetPath, &ubuf) < 0) {
#else
	if(utimes(targetPath, ubuf) < 0) {
#endif /*WIN32*/
		debug1("failed set times on output file ('%s')", targetPath);
	}

	return ret;
}


void FilesystemRepositoryImpl::ensureFolder(
	const char *dzbase, const char *dzpath,
	DropzoneCallback &callback, Context &ctx)
{
	// Ensure that the target subfolder in the dropzone exists
	if(strcmp(dzbase, dzpath) != 0) {
		const char *relPath = &dzpath[strlen(dzbase)];
		// Ensure all slahes are the correct way around
		char *p = (char *)relPath;
		while (*p) {
			if (*p == '/') *p=DIR_SEP_CHAR;
			p++;
		}
		if(relPath[0] == DIR_SEP_CHAR) { relPath++; }
		debug1("Checking sub-folder '%s'", relPath);
		if(relPath[0] && !callback.dirExists(relPath)) {
			debug1("Creating sub-folder '%s'", relPath);
			if(!callback.mkdir(relPath)) {
				throw RuntimeError(ctx.stack(), "Failed to create sub-folder '%s'", relPath);
			}
		}
	}
}


void FilesystemRepositoryImpl::checkoutFolder(
	const char *dzbase, const char *dzpath, const char *repobase,
	const char *repopath, bool recursive, StringList *pattern,
	DropzoneCallback &callback, Context &ctx)
{
	int total = 0, success = 0, failed = 0, not_processed = 0;
	bool first = true;

	// Make sure that the target folder exists - doing it here will create all
	// the folders regardless of whether we copy anything from them
	// TODO: Add an option to turn this on
	//if(create_empty_directories) {
	//	ensureFolder(dzbase, dzpath, callback, ctx);
	//}

	// First process the files in the folder itself, ignoring sub-folders - the
	// patterns match the files to copy and may not select any sub-folders at all
	StringListIterator iter(*pattern);
	for(const char *patt = iter.first(); patt; patt = iter.next()) {
		debug3("pattern  = '%s'", patt);
		FileFind finder(repopath, patt);
		if(finder.find()) {
			do {
				if(!finder.isDirectory() && !finder.isHidden()) {
					// file
					CharPtr targetPath = (char*) malloc(strlen(dzpath) + strlen(finder.filename()) + 10);
					strcpy(targetPath, dzpath);
					if(dzpath[strlen(dzpath)-1] != DIR_SEP_CHAR) {
						strcat(targetPath, DIR_SEP_STR);
					}
					strcat(targetPath, finder.filename());
					++total;

					// Make sure that the target folder exists - doing it here only
					// creates the folders that contain items that match the patterns
					if(first /*&& !create_empty_directories*/) {
						ensureFolder(dzbase, dzpath, callback, ctx);
						first = false;
					}

					// Make sure the target file does not exist
					struct stat buf;
					if(stat(targetPath, &buf) == -1) {
						ConstCharPtr sourcePath = finder.fullpath();
						if(copyFile(sourcePath, targetPath)) {
							// Now strip the root filepath off the front
							CharPtr ffp = finder.fullpath();
							debug3("copied file ffp='%s'", (const char*) ffp);
							char *fn = ffp;
							if(strstr(fn, repobase)) {
								fn += strlen(repobase);
							}
							if((fn[0] == DIR_SEP_CHAR) && fn[1]) {
								fn++;
							}

							// Now strip the dropzone location off the front - note we use
							// the parent dropzone here and not the dzpath which could be
							// a subfolder
							const char *pn = targetPath;
							debug3("copied file targetPath='%s'", (const char*) targetPath);
							if(strstr(pn, dzbase)) {
								pn += strlen(dzbase);
							}
							if((pn[0] == DIR_SEP_CHAR) && pn[1]) {
								pn++;
							}

							debug3("checked out file fn='%s' pn='%s'", fn, pn);
							callback.checked_out_file(
								this, fn, pn, (m_version ? m_version : "0"));
							++success;

							// This may need to move
							ConstCharPtr fnfolder = dirname(fn);
							ConstCharPtr dzfolder = Dropzone::dirname(pn);
							callback.checked_out_folder(
								(fnfolder ? (const char*) fnfolder : DIR_SEP_STR), 
								(dzfolder ? (const char*) dzfolder : DIR_SEP_STR), false);
						} else {
							++failed;
						}
					} else {
						debug1("target file '%s' exists - not copying", (const char*) targetPath);
						++not_processed;
					}
				}
			} while(finder.next());
		}
	}

	callback.checkout_summary(total, success, failed, not_processed);

	// Now recurse into any subfolders, ignoring the file copying patterns
	if(recursive) {
		FileFind finder(repopath, "*");
		if(finder.find()) {
			do {
				if(finder.isDirectory() && !finder.isDots() && !finder.isHidden()) {
					// folder which is not . or ..
					CharPtr targetPath = (char*) malloc(strlen(dzpath) + strlen(finder.filename()) + 10);
					strcpy(targetPath, dzpath);
					if(dzpath[strlen(dzpath)-1] != DIR_SEP_CHAR) {
						strcat(targetPath, DIR_SEP_STR);
					}
					strcat(targetPath, finder.filename());

					ConstCharPtr sourcePath = finder.fullpath();

					checkoutFolder(
						dzbase, targetPath, repobase, sourcePath,
						recursive, pattern, callback, ctx);
				}
			} while(finder.next());
		}
	}
}

bool local_dirExists(const char *dirname)
{
	struct stat sb;
	if(stat(dirname, &sb) != -1) {
		if(sb.st_mode & STAT_IS_DIRECTORY) {
			return true;
		}
	}
	return false;
}

char *local_dirname(const char *filename)
{
	const char *ls = strrchr(filename, DIR_SEP_CHAR);
	if(!ls) {
		return strdup(DIR_SEP_STR);
	}

	int len = ls - filename;

	char *ret = (char*) malloc(len + 1);
	if (ret) {
		strncpy(ret, filename, len);
		ret[len] = '\0';
	}
	return ret;
}

int local_mkdir(const char *dirname)
{
	// Creates the local directory and all interim directories
#ifdef WIN32
	int ret = mkdir(dirname);
#else
		int ret = mkdir(dirname,0777);
#endif			

	if(ret != 0) {
		// mkdir failed - check to see if parent directory exists
		ConstCharPtr parentdir = local_dirname(dirname);
		if((strlen(parentdir) > 0) && (strcmp(parentdir, DIR_SEP_STR) != 0) && !local_dirExists(parentdir)) {
			if (local_mkdir(parentdir)) {
#ifdef WIN32				
				ret = mkdir(dirname);
#else
				ret = mkdir(dirname,0777);
#endif								
			}
		}
	}
	return (ret==0)?true:false;
}

void FilesystemRepositoryImpl::checkout(
	const char *dzbase, const char *dzpath,
	bool recursive, StringList *pattern,
	ExtendedStmt &stmt, DropzoneCallback &callback,
	Context &ctx)
{
	// Dropzone: $TEMP/$DEPID.$DEPMAJOR
	// Viewpath: \ or whatever is specified on the repsoitory
	// Username/Password: ???

	debug3("pattern=0x%lx\n",pattern);

	AutoPtr<StringList> pattern_ap;
	if(!pattern) {
		pattern_ap = pattern = new StringList();
		debug3("Adding * to pattern list");
		pattern->add("*");
	}

	debug2("FilesystemRepositoryImpl::checkout('%s','%s')", dzbase, dzpath);
	debug3("filepath = '%s'", m_filepath);

	// Find the root path of the repository - we will record all filenames relative to this
	char *repobase = strdup(m_filepath);
	Property *pfilepath = m_repository.getProperty("filepath");
	if(pfilepath) {
		char *propval = pfilepath->decryptedValue(m_repository);
		Node npropval(NODE_STR, propval, true);
		ExprPtr epropval = npropval.evaluate(ctx);
		if(epropval) {
			repobase = epropval->toString();
		}
	}
	int r=0;
	int len=0;

		if (repobase != NULL)
		{
		 #ifdef WIN32
			 len = strlen(repobase);
			 for (r=0;r<len;r++)
				{
					if (repobase[r] == '/')
						repobase[r] = '\\';
				}
			#else
			 len = strlen(repobase);
			 for (r=0;r<len;r++)
				{
					if (repobase[r] == '\\')
						repobase[r] = '/';
				}				
			#endif		
		}
	debug1("repobase = '%s'", (const char*) repobase);

	// Repobase must exist - someone may have misconfigured the repo
	// On Windows servers, trailing \ chars can break stuff - remove them
#ifdef WIN32
	char *p = &(m_filepath[strlen(m_filepath)-1]);
	while (*p=='\\' || *p=='/') p--;
	*(p+1)='\0';
#endif
	struct stat buf;
	if(stat(m_filepath, &buf) == -1) {
		// Repobase does not exist - let's try creating it (in case something
		// else is going to create it
		local_mkdir(m_filepath);
		// Retry and only throw exception if still missing
		if (stat(m_filepath, &buf) == -1) {
			throw RuntimeError(stmt, ctx.stack(),
				"Base directory '%s' (%s) of repository '%s' does not exist and could not be created",
				(const char*) repobase, m_filepath, m_repository.name());
		}
	}

	// Record the root filepath up front, as this maps to the info file for the repository
	callback.checked_out_folder(DIR_SEP_STR, DIR_SEP_STR, true);

	checkoutFolder(
		dzbase, dzpath, repobase, m_filepath,
		recursive, pattern, callback, ctx);
}


void FilesystemRepositoryImpl::checkin(
	const char *dzbase, const char *dzpath, ExtendedStmt &stmt,
	DropzoneCallback &callback, Context &ctx)
{
	throw RuntimeError(stmt, ctx.stack(), "Checkin not supported");
}


char *FilesystemRepositoryImpl::dirname(const char *filename)
{
	const char *ls = strrchr(filename, DIR_SEP_CHAR);
	if(!ls) {
		return NULL;
	}

	int len = ls - filename;

	char *ret = (char*) malloc(len + 1);
	if (ret) {
		strncpy(ret, filename, len);
		ret[len] = '\0';
	}
	return ret;
}


Expr *FilesystemRepositoryImpl::getAttribute(const char *name, Context &ctx)
{
	if(strcmp(name, "filepath") == 0) {
		return new Expr(m_filepath);
	} else if(strcmp(name, "version") == 0) {
		return m_version ? new Expr(m_version) : NULL;
	}

	return RepositoryImpl::getAttribute(name, ctx);
}


///////////////////////////////////////////////////////////////////////////////
// FilesystemRepositoryImplFactory
///////////////////////////////////////////////////////////////////////////////

FilesystemRepositoryImplFactory::FilesystemRepositoryImplFactory()
{}


RepositoryImpl *FilesystemRepositoryImplFactory::create(
	DMINT32 implId, Repository &parent, ExtendedStmt &stmt, Context &ctx)
{
	ConstCharPtr filepath = stmt.getOverridableAppendableArgAsString(
		"filepath", DIR_SEP_CHAR, DIR_SEP_CHAR, parent, true, ctx);
	ConstCharPtr version = stmt.getOverridableArgAsString(
		"version", parent, false, ctx);
	return new FilesystemRepositoryImpl(*this, implId, parent, filepath, version);
}


///////////////////////////////////////////////////////////////////////////////

// TODO: remove "filesystem_" prefix when moving to a DLL

extern "C" FILESYSTEM_EXPORT int filesystem_PluginStart(DM &dm)
{
	//
	// Meister is just a "filesystem" repository type for now.
	//
	dm.writeToLogFile("Filesystem plugin V" FILESYSTEM_PLUGIN_VERSION);
	dm.writeToLogFile("Meister plugin V" FILESYSTEM_PLUGIN_VERSION);

	RepositoryImplRegistry::instance().registerFactory("filesystem", new FilesystemRepositoryImplFactory());
	RepositoryImplRegistry::instance().registerFactory("meister", new FilesystemRepositoryImplFactory());
	return 0;
}


extern "C" FILESYSTEM_EXPORT int filesystem_PluginInstall(DM &dm)
{
	// name, required, appendable
	List<PropertyDef> *propdefs = new List<PropertyDef>(true);
	propdefs->add(new PropertyDef("filepath", true,  true));
	propdefs->add(new PropertyDef("version",  false, false));

	dm.installProviderImpl("filesystem", NULL, OBJ_KIND_REPOSITORY, propdefs);
	return 0;
}
