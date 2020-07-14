#ifndef __pathname_h

#define __pathname_h


#include "hashtable.h"


class PathNameImpl
{
public:
	virtual ~PathNameImpl();

	virtual const char *path() = 0;
	virtual const char *stdpath() = 0;

	virtual bool isAbsolute() = 0;
	virtual PathNameImpl *basename() = 0;
	virtual PathNameImpl *dirname() = 0;
	virtual PathNameImpl *parent() = 0;
	virtual PathNameImpl *append(const char *path) = 0;
	virtual PathNameImpl *appendstd(const char *stdpath) = 0;
};


class PathNameImplFactory
{
public:
	virtual ~PathNameImplFactory();

	virtual PathNameImpl *create(const char *path) = 0;
};


class PathNameImplRegistry
{
private:
	Hashtable<PathNameImplFactory> m_factories;

	static PathNameImplRegistry *s_instance;

public:
	PathNameImplRegistry();

	static PathNameImplRegistry &instance();
	static void cleanup();

	void registerFactory(const char *name, PathNameImplFactory *factory);
	void registerBuiltIns();

	PathNameImplFactory *getFactory(const char *name);
};


#endif /*__pathname_h*/
