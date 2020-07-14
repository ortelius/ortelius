#ifndef __modify_h

#define __modify_h


#include "hashtable.h"
#include "extended.h"


class DMAPI_API ModifyProviderImpl
{
protected:
	class ExtendedStmt &m_parent;

public:
	ModifyProviderImpl(class ExtendedStmt &parent);
	virtual ~ModifyProviderImpl();

	virtual void loadFile(const char *infile, class Context &ctx) = 0; 
	virtual void saveFile(const char *outfile, class Context &ctx) = 0;

	virtual void executeSubStmt(class ExtendedStmt &stmt, class Context &ctx) = 0;
};


class DMAPI_API ModifyProviderImplFactory
{
public:
	virtual ~ModifyProviderImplFactory();

	virtual ModifyProviderImpl *create(
		class ExtendedStmt &parent) = 0;
};


class DMAPI_API ModifyProviderImplRegistry
{
private:
	Hashtable<ModifyProviderImplFactory> m_factories;

	static ModifyProviderImplRegistry *s_instance;

public:
	ModifyProviderImplRegistry();

	static ModifyProviderImplRegistry &instance();
	static void cleanup();

	void registerFactory(const char *name, ModifyProviderImplFactory *factory);
	void registerBuiltIns(class DM &dm);

	ModifyProviderImplFactory *getFactory(const char *name);
};


class DMAPI_API ModifyProviderStmtImpl : public ExtendedStmtImpl
{
protected:
	ExtendedStmt &m_parent;

public:
	ModifyProviderStmtImpl(class ExtendedStmt &parent);
	virtual ~ModifyProviderStmtImpl();

	void execute(class Context &ctx); 
};


class DMAPI_API ModifyProviderStmtImplFactory : public ExtendedStmtImplFactory
{
public:
	virtual ~ModifyProviderStmtImplFactory();

	bool allowsBody()     { return false; }
	bool allowsPrePost()  { return false; }
	bool isThreaded()     { return false; }
	ExtendedStmtImpl *create(class DM &dm, class ExtendedStmt &parent);
};


#endif /*__modify_h*/
