#ifndef __modifiers_text_h

#define __modifiers_text_h


#include "../../dmapi/modify.h"


class TextModifyProviderImpl : public ModifyProviderImpl
{
private:
	char *m_data;
	long m_size;

public:
	TextModifyProviderImpl(ExtendedStmt &parent);
	~TextModifyProviderImpl();

	void loadFile(const char *infile, class Context &ctx); 
	void saveFile(const char *outfile, class Context &ctx);

	void replace(class ExtendedStmt &stmt, class Context &ctx);

	void executeSubStmt(class ExtendedStmt &stmt, class Context &ctx);
};


class TextModifyProviderImplFactory : public ModifyProviderImplFactory
{
public:
	TextModifyProviderImplFactory();
	~TextModifyProviderImplFactory();

	ModifyProviderImpl *create(ExtendedStmt &parent);
};


#endif /*__modifiers_text_h*/
