#ifndef __repositories_svn_h

#define __repositories_svn_h


#include "../../dmapi/repository.h"


class SvnRepositoryImpl : public virtual RepositoryImpl
{
private:
	char *m_url;
	char *m_path;

public:
	SvnRepositoryImpl(
		class RepositoryImplFactory &factory, DMINT32 implId,
		class Repository &respository, const char *url, const char *path);

	virtual ~SvnRepositoryImpl();

	bool testProvider(class Context &ctx);

	void checkout(
		const char *dzbase, const char *dzpath, bool recursive,
		class StringList *pattern, class ExtendedStmt &stmt,
		class DropzoneCallback &callback, class Context &ctx);

	void checkin(
		const char *dzbase, const char *dzpath, class ExtendedStmt &stmt,
		class DropzoneCallback &callback, class Context &ctx);

	char *dirname(const char *filename);

	// RepositoryImpl
	class Expr *getAttribute(const char *name, class Context &ctx);
};


class SvnRepositoryImplFactory : public virtual RepositoryImplFactory
{
public:
	SvnRepositoryImplFactory();

	RepositoryImpl *create(
		DMINT32 implId, class Repository &parent, class ExtendedStmt &stmt,
		class Context &ctx);
};


#endif /*__repositories_svn_h*/
