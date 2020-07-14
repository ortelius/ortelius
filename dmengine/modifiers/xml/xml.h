#ifndef __modifiers_xml_h

#define __modifiers_xml_h


#include "../../dmapi/modify.h"


class XmlModifyProviderImpl : public ModifyProviderImpl
{
private:
	class TiXmlDocument *m_doc;

public:
	XmlModifyProviderImpl(ExtendedStmt &parent);
	~XmlModifyProviderImpl();

	void loadFile(const char *infile, class Context &ctx); 
	void saveFile(const char *outfile, class Context &ctx); 

	void set_attribute(class ExtendedStmt &stmt, class Context &ctx);
	void set_text(class ExtendedStmt &stmt, class Context &ctx);
	void remove_element(class ExtendedStmt &stmt, class Context &ctx);
	void add_element(class ExtendedStmt &stmt, class Context &ctx);

	void executeSubStmt(class ExtendedStmt &stmt, class Context &ctx);
};

class XmlModifyProviderImplFactory : public ModifyProviderImplFactory
{
public:
	XmlModifyProviderImplFactory();
	~XmlModifyProviderImplFactory();

	ModifyProviderImpl *create(ExtendedStmt &parent);
};


#endif /*__modifiers_xml_h*/
