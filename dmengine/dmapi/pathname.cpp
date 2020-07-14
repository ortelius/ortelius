#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "pathname.h"
//#include "exceptions.h"
//#include "context.h"
//#include "dm.h"
#include "charptr.h"
#include "autoptr.h"


class UnixPathNameImpl : public PathNameImpl
{
private:
	char *m_path;

public:
	UnixPathNameImpl(const char *path);
	~UnixPathNameImpl();

	const char *path();
	const char *stdpath();

	bool isAbsolute();
	PathNameImpl *basename();
	PathNameImpl *dirname();
	PathNameImpl *parent();
	PathNameImpl *append(const char *path);
	PathNameImpl *appendstd(const char *stdpath);
};


class UnixPathNameImplFactory : public PathNameImplFactory
{
public:
	UnixPathNameImplFactory();

	UnixPathNameImpl *create(const char *path);
};


///////////////////////////////////////////////////////////////////////////////


class WindowsPathNameImpl : public PathNameImpl
{
private:
	char *m_path;
	char *m_stdpath;

public:
	WindowsPathNameImpl(const char *path);
	~WindowsPathNameImpl();

	const char *path();
	const char *stdpath();

	bool isAbsolute();
	PathNameImpl *basename();
	PathNameImpl *dirname();
	PathNameImpl *parent();
	PathNameImpl *append(const char *path);
	PathNameImpl *appendstd(const char *stdpath);
};


class WindowsPathNameImplFactory : public PathNameImplFactory
{
public:
	WindowsPathNameImplFactory();

	WindowsPathNameImpl *create(const char *path);
};


///////////////////////////////////////////////////////////////////////////////


class VmsPathNameImpl : public PathNameImpl
{
private:
	char *m_path;
	char *m_stdpath;

public:
	VmsPathNameImpl(const char *path);
	~VmsPathNameImpl();

	const char *path();
	const char *stdpath();

	bool isAbsolute();
	PathNameImpl *basename();
	PathNameImpl *dirname();
	PathNameImpl *parent();
	PathNameImpl *append(const char *path);
	PathNameImpl *append(const char *path, bool isDir);
	PathNameImpl *appendstd(const char *stdpath);
};


class VmsPathNameImplFactory : public PathNameImplFactory
{
public:
	VmsPathNameImplFactory();

	VmsPathNameImpl *create(const char *path);
};



///////////////////////////////////////////////////////////////////////////////
// PathNameImpl
///////////////////////////////////////////////////////////////////////////////

/*virtual*/ PathNameImpl::~PathNameImpl()
{}


///////////////////////////////////////////////////////////////////////////////
// PathNameImplFactory
///////////////////////////////////////////////////////////////////////////////

/*virtual*/ PathNameImplFactory::~PathNameImplFactory()
{}


///////////////////////////////////////////////////////////////////////////////
// PathNameImplRegistry
///////////////////////////////////////////////////////////////////////////////

PathNameImplRegistry *PathNameImplRegistry::s_instance = NULL;


PathNameImplRegistry::PathNameImplRegistry()
	: m_factories(false, true)
{}


/*static*/ PathNameImplRegistry &PathNameImplRegistry::instance()
{
	if(!s_instance) {
		s_instance = new PathNameImplRegistry();
	}
	return *s_instance;
}


/*static*/ void PathNameImplRegistry::cleanup()
{
	SAFE_DELETE(s_instance);
}


void PathNameImplRegistry::registerFactory(const char *name, PathNameImplFactory *factory)
{
	m_factories.put(name, factory);
}


void PathNameImplRegistry::registerBuiltIns()
{
	registerFactory("unix",    new UnixPathNameImplFactory());
	registerFactory("windows", new WindowsPathNameImplFactory());
	registerFactory("vms",     new VmsPathNameImplFactory());
}


PathNameImplFactory *PathNameImplRegistry::getFactory(const char *name)
{
	return m_factories.get(name);
}


///////////////////////////////////////////////////////////////////////////////
// UnixPathNameImpl
///////////////////////////////////////////////////////////////////////////////

UnixPathNameImpl::UnixPathNameImpl(const char *path)
	: m_path(DUP_NULL(path))
{}


UnixPathNameImpl::~UnixPathNameImpl()
{
	SAFE_FREE(m_path);
}


const char *UnixPathNameImpl::path()
{
	return m_path;
}


const char *UnixPathNameImpl::stdpath()
{
	return m_path;
}


bool UnixPathNameImpl::isAbsolute()
{
	return (m_path && (m_path[0] == '/'));
}


/**
 * basename("/tmp/file.txt") => "file.txt"
 * basename("/tmp)           => "tmp"
 * basename("/")             => "/"
 *
 * Last slash must be followed by a character.
 */
PathNameImpl *UnixPathNameImpl::basename()
{
	char *lastslash = strrchr(m_path, '/');
	return new UnixPathNameImpl((lastslash && lastslash[1]) ? ++lastslash : m_path);
}


/**
 * dirname("/tmp/file.txt") => "/tmp"
 * dirname("/tmp)           => "/"
 * dirname("/")             => "/"
 *
 * Last slash must not be the start of the path.
 */
PathNameImpl *UnixPathNameImpl::dirname()
{
	CharPtr path = strdup(m_path);
	char *lastslash = strrchr((char*) path, '/');
	if(lastslash && (lastslash > path)) {
		*lastslash = '\0';
		return new UnixPathNameImpl(path);
	}
	return new UnixPathNameImpl("/");
}


PathNameImpl *UnixPathNameImpl::parent()
{
	return NULL;
}


PathNameImpl *UnixPathNameImpl::append(const char *path)
{
	char *lastslash = strrchr((char*) m_path, '/');
	if(lastslash && !lastslash[1]) {
		CharPtr newpath = (char*) malloc(strlen(m_path) + strlen(path) + 1);
		sprintf(newpath, "%s%s", m_path, path);
		return new UnixPathNameImpl(newpath);
	} else {
		CharPtr newpath = (char*) malloc(strlen(m_path) + strlen(path) + 2);
		sprintf(newpath, "%s/%s", m_path, path);
		return new UnixPathNameImpl(newpath);
	}
}


PathNameImpl *UnixPathNameImpl::appendstd(const char *stdpath)
{
	return append(stdpath);
}


///////////////////////////////////////////////////////////////////////////////
// UnixPathNameImplFactory
///////////////////////////////////////////////////////////////////////////////

UnixPathNameImplFactory::UnixPathNameImplFactory()
{}


UnixPathNameImpl *UnixPathNameImplFactory::create(const char *path)
{
	return new UnixPathNameImpl(path);
}


///////////////////////////////////////////////////////////////////////////////
// WindowsPathNameImpl
///////////////////////////////////////////////////////////////////////////////

WindowsPathNameImpl::WindowsPathNameImpl(const char *path)
	: m_path(DUP_NULL(path)), m_stdpath(NULL)
{}


WindowsPathNameImpl::~WindowsPathNameImpl()
{
	SAFE_FREE(m_path);
	SAFE_FREE(m_stdpath);
}


const char *WindowsPathNameImpl::path()
{
	return m_path;
}


const char *WindowsPathNameImpl::stdpath()
{
	if(!m_stdpath) {
		m_stdpath = strdup(m_path);
		for(char *x = m_stdpath; x && *x; x++) {
			if(*x == '\\') { *x = '/'; }
		}
	}
	return m_stdpath;
}


bool WindowsPathNameImpl::isAbsolute()
{
	return (m_path && (strlen(m_path) >= 3) && (m_path[1] == ':') && (m_path[2] == '\\'));
}


PathNameImpl *WindowsPathNameImpl::basename()
{
	char *lastslash = strrchr(m_path, '\\');
	return new WindowsPathNameImpl((lastslash && lastslash[1]) ? ++lastslash : m_path);
}


PathNameImpl *WindowsPathNameImpl::dirname()
{
	CharPtr path = strdup(m_path);
	char *lastslash = strrchr((char*) path, '\\');
	if(lastslash) {
		char *firstslash = strchr((char*) path, '\\');
		if(lastslash != firstslash) {
			*lastslash = '\0';
			return new WindowsPathNameImpl(path);
		}
		if((strlen(path) >= 3) && (path[1] == ':') && (path[2] == '\\')) {
			path[3] = '\0';
			return new WindowsPathNameImpl(path);
		}
		*lastslash = '\0';
		return new WindowsPathNameImpl(path);
	}
	return new WindowsPathNameImpl("\\");
}


PathNameImpl *WindowsPathNameImpl::parent()
{
	return NULL;
}


PathNameImpl *WindowsPathNameImpl::append(const char *path)
{
	char *lastslash = strrchr((char*) m_path, '\\');
	if(lastslash && !lastslash[1]) {
		CharPtr newpath = (char*) malloc(strlen(m_path) + strlen(path) + 1);
		sprintf(newpath, "%s%s", m_path, path);
		return new UnixPathNameImpl(newpath);
	} else {
		CharPtr newpath = (char*) malloc(strlen(m_path) + strlen(path) + 2);
		sprintf(newpath, "%s\\%s", m_path, path);
		return new UnixPathNameImpl(newpath);
	}
}


PathNameImpl *WindowsPathNameImpl::appendstd(const char *stdpath)
{
	CharPtr path = strdup(stdpath);
	for(char *x = path; x && *x; x++) {
		if(*x == '/') { *x = '\\'; }
	}
	return append(path);
}


///////////////////////////////////////////////////////////////////////////////
// WindowsPathNameImplFactory
///////////////////////////////////////////////////////////////////////////////

WindowsPathNameImplFactory::WindowsPathNameImplFactory()
{}


WindowsPathNameImpl *WindowsPathNameImplFactory::create(const char *path)
{
	return new WindowsPathNameImpl(path);
}


///////////////////////////////////////////////////////////////////////////////
// VmsPathNameImpl
///////////////////////////////////////////////////////////////////////////////

VmsPathNameImpl::VmsPathNameImpl(const char *path)
	: m_path(DUP_NULL(path)), m_stdpath(NULL)
{}


VmsPathNameImpl::~VmsPathNameImpl()
{
	SAFE_FREE(m_path);
	SAFE_FREE(m_stdpath);
}


const char *VmsPathNameImpl::path()
{
	return m_path;
}


const char *VmsPathNameImpl::stdpath()
{
	if(!m_stdpath) {
		CharPtr path = strdup(m_path);
		char *firstbrac = strchr((char*) path, '[');
		char *lastbrac = strrchr((char*) path, ']');
		if(firstbrac && lastbrac) {
			*lastbrac = '\0';
			++firstbrac;
			++lastbrac;
			for(char *x = firstbrac; x && *x; x++) {
				if(*x == '.') { *x = '/'; }
			}
			m_stdpath = (char*) malloc(strlen(firstbrac) + strlen(lastbrac) + 3);
			sprintf(m_stdpath, "/%s/%s", firstbrac, lastbrac);
		} else {
			m_stdpath = strdup("oops!");
		}
	}
	return m_stdpath;
}


bool VmsPathNameImpl::isAbsolute()
{
	return (m_path && strstr(m_path, ":["));
}


/**
  * basename("$2$DKA200:[FERDU01.TEST]file.txt") => "file.txt"
  * basename("$2$DKA200:[FERDU01.TEST]")         => "TEST"
  * basename("$2$DKA200:[FERDU01]")              => "$2$DKA200:[FERDU01]"
  *
  * Filename follows closing ']', otherwise is a directory and is part
  * following last '.' before closing ']'
 */
PathNameImpl *VmsPathNameImpl::basename()
{
	CharPtr path = strdup(m_path);
	char *lastbrac = strrchr((char*) path, ']');
	if(lastbrac && lastbrac[1]) {
		// file - return part after ']'
		return new VmsPathNameImpl(++lastbrac);
	}
	// directory - return part between last '.' and ']' or complete path
	char *lastdot = strrchr((char*) path, '.');
	if(lastdot) {
		if(lastbrac) {
			*lastbrac = '\0';
		}
		return new VmsPathNameImpl(++lastdot);
	}
	// no subdir so return entire path
	return new VmsPathNameImpl(m_path);
}


/**
  * basename("$2$DKA200:[FERDU01.TEST]file.txt") => "$2$DKA200:[FERDU01.TEST]"
  * basename("$2$DKA200:[FERDU01.TEST]")         => "$2$DKA200:[FERDU01]"
  * basename("$2$DKA200:[FERDU01]")              => "$2$DKA200:[FERDU01]"
  *
  * Directory precedes and includes closing ']' if a filename follows,
  * otherwise parent directory and is part following before last '.' with a ']'
  * appended or complete path if no '.'
 */
PathNameImpl *VmsPathNameImpl::dirname()
{
	CharPtr path = strdup(m_path);
	char *lastbrac = strrchr((char*) path, ']');
	if(lastbrac && lastbrac[1]) {
		// file - truncate after ']'
		lastbrac[1] = '\0';
		return new VmsPathNameImpl(path);
	}
	// directory - return part between last '.' and ']' or complete path
	char *lastdot = strrchr((char*) path, '.');
	if(lastdot && lastdot[1]) {
		lastdot[0] = ']';
		lastdot[1] = '\0';
		return new VmsPathNameImpl(path);
	}
	// no subdir so return entire path
	return new VmsPathNameImpl(m_path);
}


PathNameImpl *VmsPathNameImpl::parent()
{
	return NULL;
}


PathNameImpl *VmsPathNameImpl::append(const char *path)
{
	return append(path, false);
}


PathNameImpl *VmsPathNameImpl::append(const char *path, bool isDir)
{
	if(isDir) {
		CharPtr oldpath = strdup(m_path);
		char *lastbrac = strrchr((char*) oldpath, ']');
		// If this is a dir and currently has no filename
		if(lastbrac && !lastbrac[1]) {
			*lastbrac = '\0';
			CharPtr newpath = (char*) malloc(strlen(oldpath) + strlen(path) + 3);
			sprintf(newpath, "%s.%s]", (const char*) oldpath, path);
			return new VmsPathNameImpl(newpath);
		}
	} else {
		char *lastbrac = strrchr((char*) m_path, ']');
		// If this is a dir and currently has no filename
		if(lastbrac && !lastbrac[1]) {
			CharPtr newpath = (char*) malloc(strlen(m_path) + strlen(path) + 1);
			sprintf(newpath, "%s%s", m_path, path);
			return new VmsPathNameImpl(newpath);
		}
	}
	return NULL;
}


PathNameImpl *VmsPathNameImpl::appendstd(const char *stdpath)
{
	CharPtr path = strdup(stdpath);
	char *lastslash = strrchr((char*) path, '/');
	if(lastslash) {
		// Split on last slash
		*lastslash = '\0';
		// Convert remaining slashes to dots
		for(char *x = path; x && *x; x++) {
			if(*x == '/') { *x = '.'; }
		}
		// Ends in slash, so whole thing is a directory
		if(!lastslash[1]) {
			return append(path, true);
		} else {
			// Append dir first, then file
			AutoPtr<VmsPathNameImpl> temp = (VmsPathNameImpl*) append(path, true);
			return temp->append(++lastslash, false);
		}
	}

	// No slashes, so simple append of filename
	return append(stdpath, false);
}


///////////////////////////////////////////////////////////////////////////////
// VmsPathNameImplFactory
///////////////////////////////////////////////////////////////////////////////

VmsPathNameImplFactory::VmsPathNameImplFactory()
{}


VmsPathNameImpl *VmsPathNameImplFactory::create(const char *path)
{
	return new VmsPathNameImpl(path);
}
