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
#ifndef __charptr_h

#define __charptr_h


#include "dmapiexport.h"


/**
 * Autoptr to delete a string when the pointer it goes out of scope, whether
 * that be by normal function return or by an exception being raised.  This
 * version only provides a (const char *) cast operator and signifies that
 * the code will not change the string, only free it.
 */
class DMAPI_API ConstCharPtr {
private:
	char *m_ptr;

public:
	ConstCharPtr();
	ConstCharPtr(char *ptr);
	~ConstCharPtr();

	char *operator =(char *ptr);
	operator const char*();
	bool operator ==(const char *ptr) const;
	bool operator !() const;
	operator bool() const;
	bool isNull();
};


/**
 * Autoptr to delete a string when the pointer it goes out of scope, whether
 * that be by normal function return or by an exception being raised.  This
 * version only provides an additional (char *) cast operator and signifies
 * that the code will make changes to the string, as well as free it.
 */
class DMAPI_API CharPtr {
private:
	char *m_ptr;

public:
	CharPtr();
	CharPtr(char *ptr);
	~CharPtr();

	char *operator =(char *ptr);
	operator const char*();
	operator char*();
	char &operator [](const int index);
	const char operator [](const int index) const;
	bool operator ==(const char *ptr) const;
	bool operator !() const;
	operator bool() const;
	bool isNull();
};


#endif /*__charptr_h*/
