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

#ifdef WIN32
#include "windows.h"
#define XMLMODIFY_EXPORT __declspec(dllexport)
#else
#define XMLMODIFY_EXPORT
#endif /*WIN32*/

#include <stdio.h>

#include "xml.h"

#include "../../dmapi/context.h"
#include "../../dmapi/exceptions.h"
#include "../../dmapi/node.h"
#include "../../dmapi/tinyxml.h"
#include "../../dmapi/charptr.h"
#include "../../dmapi/dm.h"

#define XMLMODIFY_PLUGIN_VERSION "1.00"
#define XMLMODIFY_MODIFY_NAME    "xml"
#define XMLMODIFY_SETATTR_NAME   "set_attribute"
#define XMLMODIFY_SETTEXT_NAME   "set_text"
#define XMLMODIFY_REMELEM_NAME   "remove_element"
#define XMLMODIFY_ADDELEM_NAME   "add_element"


class ElementListEntry
{
private:
	TiXmlElement *m_ele;
	class ElementListEntry *m_next;

	ElementListEntry(TiXmlElement *ele, class ElementListEntry *next);

	friend class ElementList;
	friend class ElementListIterator;
};


class ElementList
{
private:
	ElementListEntry *m_head;
	ElementListEntry *m_tail;

public:
	ElementList();
	void add(TiXmlElement *element);
	void append(ElementList &list);

	friend class ElementListIterator;
};


class ElementListIterator
{
private:
	ElementListEntry *m_curr;

public:
	ElementListIterator(ElementList& list);
	TiXmlElement *next();
};

enum PositionOp {
	posop_nop=0,
	posop_eq=1,
	posop_lt=2,
	posop_le=3,
	posop_gt=4,
	posop_ge=5
};

class XPathPredicate
{
private:
	int m_index;
	const char *m_attribute;
	const char *m_value;
	int m_tgt;				// for position() operation
	PositionOp m_op;		// position() operation (<, <=, >, >=, =)

public:
	XPathPredicate(int index);
	XPathPredicate(const char *attribute, const char *value = NULL);
	XPathPredicate(int index,PositionOp op);
	bool evaluate(int count, TiXmlElement &element);
};


ElementListEntry::ElementListEntry(TiXmlElement *ele, class ElementListEntry *next)
		: m_ele(ele), m_next(next) {
}

ElementList::ElementList()
		: m_head(NULL), m_tail(NULL) {
}

void ElementList::add(TiXmlElement *element) {
	if(m_tail) {
		m_tail->m_next = new ElementListEntry(element, NULL);
		m_tail = m_tail->m_next;
	} else {
		m_head = m_tail = new ElementListEntry(element, NULL);
	}
}

void ElementList::append(ElementList &list) {
	if(m_tail) {
		m_tail->m_next = list.m_head;
		m_tail = m_tail->m_next;
	} else {
		m_head = list.m_head;
		m_tail = list.m_tail;
	}

}

ElementListIterator::ElementListIterator(ElementList& list)
	: m_curr(list.m_head) {
}

TiXmlElement *ElementListIterator::next() {
	TiXmlElement *ret = m_curr ? m_curr->m_ele : NULL;
	m_curr = m_curr ? m_curr->m_next : NULL;
	return ret;
}


bool isdigits(const char *str) {
	bool ret = true;
	for( ; *str; str++) {
		if(!isdigit(*str)) {
			ret = false;
			break;
		}
	}
	return ret;
}


XPathPredicate::XPathPredicate(int index)
	: m_index(index), m_op(posop_nop), m_attribute(NULL), m_value(NULL) {
}

XPathPredicate::XPathPredicate(const char *attribute, const char *value /* = NULL*/)
	: m_index(0), m_op(posop_nop), m_attribute(attribute), m_value(value) {
}

XPathPredicate::XPathPredicate(int target,PositionOp op)
	: m_index(target), m_op(op), m_attribute(NULL), m_value(NULL) {
}

bool XPathPredicate::evaluate(int count, TiXmlElement &element) {
//printf("evaluate %d, %d, %s\n", count, m_index, m_attribute ? m_attribute : "(null)");
	if(m_index > 0) {
		if (m_op) {
			switch(m_op) {
				case posop_eq:
					return (count == m_index);
				case posop_lt:
					return (count < m_index);
				case posop_le:
					return (count <= m_index);
				case posop_gt:
					return (count > m_index);
				case posop_ge:
					return (count >= m_index);
				default:
					return false;
			}
		} else {
			return (m_index == count);
		}
	}
	if(m_attribute) {
		if(m_value) {
			return (element.Attribute(m_attribute) && (strcmp(element.Attribute(m_attribute), m_value) == 0));
		} else {
			return (element.Attribute(m_attribute) ? true : false);
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////


/**
 * Recursively processes an xpath and returns a list of selected elements.
 */
ElementList *GetElementsByXPath(TiXmlNode &node, char **parsedPath)
{
	XPathPredicate *expression = NULL;

	ElementList *list = NULL;
	if(*parsedPath) {
		char *elementName = strdup(*parsedPath);

		// handle expressions
		char *expr = strstr(elementName, "[");
		if(expr && (expr[strlen(expr)-1] == ']')) {
			expr[strlen(expr)-1] = '\0';
			*expr = '\0';
			expr++;

			if(isdigits(expr)) {
				// simple index - count from 1
				expression = new XPathPredicate(atoi(expr));
			} else if(expr[0] == '@') {
				char *equals = strstr(expr, "=");
				if(equals) {
					// attribute = value (no equals just tests exists)
					*equals = '\0';
					equals++;
					if (*equals=='"' || *equals=='\'') {
						bool sq = (*equals=='\'');
						equals++;
						int n = strlen(equals);
						if (equals[n-1]==sq?'\'':'"') equals[n-1]='\0';
						else throw RuntimeError("quote mismatch: \"%s",equals);
					}
				}
				expression = new XPathPredicate(&expr[1], equals);
			} else if (strncmp(expr,"last()",6)==0) {
				int c=0;
				for(TiXmlElement *element = node.FirstChildElement(elementName); element; element = element->NextSiblingElement(elementName)) {
					c++;
				}
				char *p = &(expr[6]);	// first char after last()
				while (*p && *p==' ' || *p=='\t') p++;	// skip any whitespace
				if (*p=='-') {
					// taking an offset from last()
					int t=(int)atol(p+1);
					c=c-t;
				}
				expression = new XPathPredicate(c);
			} else if (strncmp(expr,"position()",10)==0) {
				PositionOp op = posop_nop;
				char *p = &(expr[10]);	// first char after position()
				while (*p && *p==' ' || *p=='\t') p++;	// skip any whitespace
				if (strncmp(p,"<=",2)==0) op = posop_le;
				else
				if (strncmp(p,">=",2)==0) op = posop_ge;
				else
				if (*p=='<') op = posop_lt;
				else
				if (*p=='>') op = posop_gt;
				else
				if (*p=='=') op = posop_eq;
				if (op != posop_nop) {
					// skip comparison operator
					while (*p=='>' || *p=='<' || *p=='=') p++;
					if (*p && isdigits(p)) {
						int v = (int)atol(p);
						expression = new XPathPredicate(v,op);
					} else {
						throw RuntimeError("position() offset is invalid - must be numeric (%s)",expr);
					}
				} else {
					// There was no offset relative to position()
					throw RuntimeError("position() should be followed by <, <=, >, >= or =");
				}
			} else {
				throw RuntimeError("unrecognised expression %s", expr);
			}
		}

// printf("%s, %s, %p\n", *parsedPath, elementName, expression);

		// now find any matches
		int count = 0;
		for(TiXmlElement *element = node.FirstChildElement(elementName); element; element = element->NextSiblingElement(elementName)) {
			count++;
			if(!expression || expression->evaluate(count, *element)) {
				if(parsedPath[1]) {
					ElementList *ret = GetElementsByXPath(*element, &parsedPath[1]);
					if(ret) {
						if(!list) list = new ElementList();
						list->append(*ret);
					}
				} else {
					if(!list) list = new ElementList();
					list->add(element);
				}
			}
		}
	}

	return list;
}


/**
 * Breaks down an xpath into its component parts (separated by '/') and returns it as an argv array.
 */
char **ParseElementXPath(const char *path) {
	// count the path components
	int slashes = 0;
	for(const char *p = path; *p; p++) {
		if(*p == '/') slashes++;
	}

	char **parsedPath = (char**) malloc((slashes+2) * sizeof(char*));
	if (parsedPath) {
		// parse the path into an array
		int n = 0;
		char *pc = strdup(path);
		char *temp = NULL;
		for(char *x = STRTOK_R(pc, "/", &temp);
			x; x = STRTOK_R(NULL, "/", &temp)) {
			parsedPath[n++] = strdup(x);
		}
		parsedPath[n++] = NULL;
	}

	return parsedPath;
}


/**
 * Breaks down an xpath into its component parts (separated by '/') and returns it as an argv array.
 * The path is terminated at the first attribute reference (beginning with '@') and the name of the
 * attribute is returned in the second param.
 */
char **ParseAttributeXPath(const char *path, char **attribute) {
	// count the path components
	int slashes = 0;
	for(const char *p = path; *p; p++) {
		if(*p == '/') slashes++;
	}

	char **parsedPath = (char**) malloc((slashes+2) * sizeof(char*));

	// parse the path into an array
	int n = 0;
	char *pc = strdup(path);
	char *temp = NULL;
	for(char *x = STRTOK_R(pc, "/", &temp);
		x; x = STRTOK_R(NULL, "/", &temp)) {
		if(x[0] == '@') {
			*attribute = strdup(&x[1]);
			break;
		}
		parsedPath[n++] = strdup(x);
	}
	parsedPath[n++] = NULL;

	return parsedPath;
}


/**
 * Sets value of the given attribute of all the elements selected by path.
 */
bool SetAttributeByPath(TiXmlDocument &doc, const char *path, const char *value)
{
	bool ret = false;

	char *attribute = NULL;
	char **parsedPath = ParseAttributeXPath(path, &attribute);
	if (!attribute) throw RuntimeError("xpath requires attribute definition @attr for set_attribute");

	ElementList *list = GetElementsByXPath(doc, parsedPath);
	if(list) {
		ElementListIterator iter(*list);

		for(TiXmlElement *element = iter.next(); element; element = iter.next()) {
			const char *oldValue = element->Attribute(attribute);
			debug1("%s: %s => %s", path, (oldValue ? oldValue : "(null)"), value);
			element->SetAttribute(attribute, value);
			ret = true;
		}
	} else {
		throw RuntimeError("parent element not found");
	}
	
	free(parsedPath);
	return ret;
}


/**
 * Sets the text of the elements matched by path.  Any children of the element will be removed.
 */
bool SetTextByPath(TiXmlDocument &doc, const char *path, const char *value)
{
	bool ret = false;

	char **parsedPath = ParseElementXPath(path);

	ElementList *list = GetElementsByXPath(doc, parsedPath);
	if(list) {
		ElementListIterator iter(*list);

		for(TiXmlElement *element = iter.next(); element; element = iter.next()) {
			const char *oldValue = element->GetText();
			debug1("%s: %s => %s", path, (oldValue ? oldValue : "(null)"), value);
			element->Clear();
			element->InsertEndChild(*new TiXmlText(value));
			ret = true;
		}
	} else {
		throw RuntimeError("element %s not found", path);
	}
	
	free(parsedPath);
	return ret;
}


/**
 * Removes the elements (and subtree) matched by path.
 */
bool RemoveElementByPath(TiXmlDocument &doc, const char *path)
{
	bool ret = false;

	char **parsedPath = ParseElementXPath(path);

	ElementList *list = GetElementsByXPath(doc, parsedPath);
	if(list) {
		ElementListIterator iter(*list);

		for(TiXmlElement *element = iter.next(); element; element = iter.next()) {
			if(!element->Parent()->RemoveChild(element)) {
				throw RuntimeError("Failed to remove element %s", path);
			} else {
				debug1("removed %s", path);
				ret = true;
			}
		}
	} else {
		throw RuntimeError("element %s not found", path);
	}
	
	free(parsedPath);
	return ret;
}


/**
 * Adds the given node (and subtree) before all the elements matched by path.
 */
bool AddElementBeforeByPath(TiXmlDocument &doc, const char *path, TiXmlNode &node)
{
	bool ret = false;

	char **parsedPath = ParseElementXPath(path);

	ElementList *list = GetElementsByXPath(doc, parsedPath);
	if(list) {
		ElementListIterator iter(*list);

		for(TiXmlElement *element = iter.next(); element; element = iter.next()) {
			if(!element->Parent()->InsertBeforeChild(element, node)) {
				throw RuntimeError("failed to insert element");
			} else {
				debug1("added before %s", path);
				ret = true;
			}
		}
	} else {
		throw RuntimeError("element %s not found", path);
	}
	
	free(parsedPath);
	return ret;
}


/**
 * Adds the given node (and subtree) after all the elements matched by path.
 */
bool AddElementAfterByPath(TiXmlDocument &doc, const char *path, TiXmlNode &node)
{
	bool ret = false;

	char **parsedPath = ParseElementXPath(path);

	ElementList *list = GetElementsByXPath(doc, parsedPath);
	if(list) {
		ElementListIterator iter(*list);

		for(TiXmlElement *element = iter.next(); element; element = iter.next()) {
			if(!element->Parent()->InsertAfterChild(element, node)) {
				throw RuntimeError("failed to insert element");
			} else {
				debug1("added after %s", path);
				ret = true;
			}
		}
	} else {
		throw RuntimeError("element %s not found", path);
	}
	
	free(parsedPath);
	return ret;
}


/**
 * Adds the given node (and subtree) inside all the elements matched by path.
 */
bool AddElementInsideByPath(TiXmlDocument &doc, const char *path, TiXmlNode &node)
{
	bool ret = false;

	char **parsedPath = ParseElementXPath(path);

	ElementList *list = GetElementsByXPath(doc, parsedPath);
	if(list) {
		ElementListIterator iter(*list);

		for(TiXmlElement *element = iter.next(); element; element = iter.next()) {
			if(!element->InsertEndChild(node)) {
				throw RuntimeError("failed to insert element");
			} else {
				debug1("added inside %s", path);
				ret = true;
			}
		}
	} else {
		throw RuntimeError("element %s not found", path);
	}
	
	free(parsedPath);
	return ret;
}


#if 0
/**
 * Performs the equivalent of a clone, but expands any DM variables.
 * Also for the purposes of the plugin DLL makes sure that we allocate the memory.
 */
TiXmlNode *deepCopy(TiXmlNode &original, ScopeStack &stack)
{
	switch(original.Type()) {
	case TiXmlNode::ELEMENT:
		{
		TiXmlElement &ele = (TiXmlElement&) original;
		TiXmlElement *copy = new TiXmlElement(api->expand(ele.Value(), stack));
		for(TiXmlAttribute* attribute = ele.FirstAttribute(); attribute; attribute = attribute->Next()) {
			copy->SetAttribute(attribute->Name(), api->expand(attribute->Value(), stack));
		}
		for(TiXmlNode* node = ele.FirstChild(); node; node = node->NextSibling() ) {
			TiXmlNode *child = deepCopy(*node, stack);
			if(child) {
				copy->LinkEndChild(child);
			}
		}
		return copy;
		}
	case TiXmlNode::COMMENT:
		return new TiXmlComment(api->expand(original.Value(), stack));
	case TiXmlNode::TEXT:
		return new TiXmlText(api->expand(original.Value(), stack));
	default:
		api->writeToStdOut("unknown node type = %d\n", original.Type());
		break;
	}

	return NULL;
}
#endif /*0*/



///////////////////////////////////////////////////////////////////////////////
// XmlModifyProviderImpl
///////////////////////////////////////////////////////////////////////////////

XmlModifyProviderImpl::XmlModifyProviderImpl(ExtendedStmt &parent)
	: ModifyProviderImpl(parent), m_doc(NULL)
{}


XmlModifyProviderImpl::~XmlModifyProviderImpl()
{
	SAFE_DELETE(m_doc);
}


void XmlModifyProviderImpl::loadFile(const char *infile, class Context &ctx)
{
	m_doc = new TiXmlDocument();

	if(!m_doc->LoadFile(infile)) {
		ctx.writeToStdOut("Failed to load XML file \"%s\"", infile);
		ctx.writeToStdOut("%s (row %d, col %d)", m_doc->ErrorDesc(), m_doc->ErrorRow(), m_doc->ErrorCol());
		throw RuntimeError(ctx.stack(), "Failed to load XML file \"%s\"", infile);
	}
}


void XmlModifyProviderImpl::saveFile(const char *outfile, class Context &ctx)
{
	if(m_doc) {
		if(debuglevel() >= 1) {
			debug1("Contents to be written to '%s' are:", outfile);
			m_doc->Print();
		}

		if(!m_doc->SaveFile(outfile)) {
			throw RuntimeError(ctx.stack(), "Failed to save XML file \"%s\"", outfile);
		}
		FILE *in = fopen(outfile,"r");
		if (in) {
			char buf[1024];
			debug1("FILE CONTENT IS:");
			while (!feof(in))
			{
				if (fgets(buf,sizeof(buf),in)) debug1(buf);
			}
			fclose(in);
		} else {
			debug1("Failed to open file %s for reading",outfile);
		}
	}
}


void XmlModifyProviderImpl::set_attribute(ExtendedStmt &stmt, Context &ctx)
{
	if(!m_doc) {
		throw RuntimeError(stmt, ctx.stack(), "Document not set - help!");
	}

	ConstCharPtr xpath = stmt.getArgAsString("xpath", ctx);
	if(!xpath) {
		throw SyntaxError(stmt, ctx.stack(), "set_attribute requires an xpath");
	}

	const char *value = stmt.getArgAsString("value", ctx);
	if(!value) {
		throw SyntaxError(stmt, ctx.stack(), "set_attribute requires a value");
	}

	SetAttributeByPath(*m_doc, xpath, value);
}


void XmlModifyProviderImpl::set_text(ExtendedStmt &stmt, Context &ctx)
{
	if(!m_doc) {
		throw RuntimeError(stmt, ctx.stack(), "Document not set - help!");
	}

	ConstCharPtr xpath = stmt.getArgAsString("xpath", ctx);
	if(!xpath) {
		throw SyntaxError(stmt, ctx.stack(), "set_text requires an xpath");
	}

	const char *value = stmt.getArgAsString("value", ctx);
	if(!value) {
		throw SyntaxError(stmt, ctx.stack(), "set_text requires a value");
	}

	SetTextByPath(*m_doc, xpath, value);
}


void XmlModifyProviderImpl::remove_element(ExtendedStmt &stmt, Context &ctx)
{
	if(!m_doc) {
		throw RuntimeError(stmt, ctx.stack(), "Document not set - help!");
	}

	ConstCharPtr xpath = stmt.getArgAsString("xpath", ctx);
	if(!xpath) {
		throw SyntaxError(stmt, ctx.stack(), "remove_element requires an xpath");
	}

	RemoveElementByPath(*m_doc, xpath);
}


void XmlModifyProviderImpl::add_element(ExtendedStmt &stmt, Context &ctx)
{
	if(!m_doc) {
		throw RuntimeError(stmt, ctx.stack(), "Document not set - help!");
	}

	ConstCharPtr xpath = stmt.getArgAsString("xpath", ctx);
	if(!xpath) {
		throw SyntaxError(stmt, ctx.stack(), "add_element: requires an xpath");
	}

	ConstCharPtr pos = stmt.getArgAsString("pos", ctx);
	if(!pos) {
		throw SyntaxError(stmt, ctx.stack(), "add_element requires a pos");
	}
	if((strcmp(pos, "before") != 0) && (strcmp(pos, "after") != 0) && (strcmp(pos, "inside") != 0)) {
		throw SyntaxError(stmt, ctx.stack(), "add_element: pos must be before/after/inside");
	}

	const char *value = stmt.getArgAsString("value", ctx);
	if(!value) {
		throw SyntaxError(stmt, ctx.stack(), "add_element: value not specified");
	}

	TiXmlDocument adddoc;
	adddoc.Parse(value);
	if (adddoc.Error()) throw SyntaxError(stmt, ctx.stack(), adddoc.ErrorDesc());

	if(strcmp(pos, "before") == 0) {
		AddElementBeforeByPath(*m_doc, xpath, *(adddoc.RootElement()));
	} else if(strcmp(pos, "after") == 0) {
		AddElementAfterByPath(*m_doc, xpath, *(adddoc.RootElement()));
	} else if(strcmp(pos, "inside") == 0) {
		AddElementInsideByPath(*m_doc, xpath, *(adddoc.RootElement()));
	}
}


void XmlModifyProviderImpl::executeSubStmt(ExtendedStmt &stmt, Context &ctx)
{
	if(strcmp(stmt.ident(), XMLMODIFY_SETATTR_NAME) == 0) {
		set_attribute(stmt, ctx);
	} else if(strcmp(stmt.ident(), XMLMODIFY_SETTEXT_NAME) == 0) {
		set_text(stmt, ctx);
	} else if(strcmp(stmt.ident(), XMLMODIFY_REMELEM_NAME) == 0) {
		remove_element(stmt, ctx);
	} else if(strcmp(stmt.ident(), XMLMODIFY_ADDELEM_NAME) == 0) {
		add_element(stmt, ctx);
	} else {
		throw RuntimeError(stmt, ctx.stack(),
			"\"%s\" is not valid with a modify of type \""
			XMLMODIFY_MODIFY_NAME "\"", stmt.ident());
	}
}


///////////////////////////////////////////////////////////////////////////////
// XmlModifyProviderImplFactory
///////////////////////////////////////////////////////////////////////////////

XmlModifyProviderImplFactory::XmlModifyProviderImplFactory()
{}


XmlModifyProviderImplFactory::~XmlModifyProviderImplFactory()
{}


ModifyProviderImpl *XmlModifyProviderImplFactory::create(ExtendedStmt &parent)
{
	return new XmlModifyProviderImpl(parent);
}


///////////////////////////////////////////////////////////////////////////////

// TODO: remove "xmlmodify_" prefix when moving to a DLL

extern "C" XMLMODIFY_EXPORT int xmlmodify_PluginStart(DM &dm)
{
	dm.writeToLogFile("Xml Modify plugin V" XMLMODIFY_PLUGIN_VERSION);
	
	ModifyProviderImplRegistry::instance().registerFactory(
		XMLMODIFY_MODIFY_NAME, new XmlModifyProviderImplFactory());

	ExtendedStmtImplRegistry::instance().registerFactory
		(XMLMODIFY_SETATTR_NAME, new ModifyProviderStmtImplFactory());
	ExtendedStmtImplRegistry::instance().registerFactory(
		XMLMODIFY_SETTEXT_NAME, new ModifyProviderStmtImplFactory());
	ExtendedStmtImplRegistry::instance().registerFactory(
		XMLMODIFY_REMELEM_NAME, new ModifyProviderStmtImplFactory());
	ExtendedStmtImplRegistry::instance().registerFactory(
		XMLMODIFY_ADDELEM_NAME, new ModifyProviderStmtImplFactory());
	return 0;
}


extern "C" XMLMODIFY_EXPORT int xmlmodify_PluginInstall(DM &dm)
{
	return 0;
}
