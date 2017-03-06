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
