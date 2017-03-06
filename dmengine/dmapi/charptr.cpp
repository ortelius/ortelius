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
#include <stdio.h>
#include <stdlib.h>

#include "charptr.h"
#include "platform.h"


///////////////////////////////////////////////////////////////////////////////
// ConstCharPtr
///////////////////////////////////////////////////////////////////////////////

ConstCharPtr::ConstCharPtr()
	: m_ptr(NULL)
{}


ConstCharPtr::ConstCharPtr(char *ptr)
	: m_ptr(ptr)
{}


ConstCharPtr::~ConstCharPtr()
{
//printf("DEBUG: ConstCharPtr deleting '%s'\n", m_ptr);
	SAFE_FREE(m_ptr);
}


char *ConstCharPtr::operator =(char *ptr)
{
	SAFE_FREE(m_ptr);
	m_ptr = ptr;
	return m_ptr;
}




ConstCharPtr::operator const char*()
{
	return m_ptr;
}



bool ConstCharPtr::operator ==(const char *ptr) const
{
	return (m_ptr == ptr) ? true : false;
}


bool ConstCharPtr::operator !() const
{
	return m_ptr ? false : true;
}


ConstCharPtr::operator bool() const
{
	return m_ptr ? true : false;
}


bool ConstCharPtr::isNull()
{
	return m_ptr ? false : true;
}


///////////////////////////////////////////////////////////////////////////////
// CharPtr
///////////////////////////////////////////////////////////////////////////////

CharPtr::CharPtr()
	: m_ptr(NULL)
{}


CharPtr::CharPtr(char *ptr)
	: m_ptr(ptr)
{}


CharPtr::~CharPtr()
{
	SAFE_FREE(m_ptr);
}


char *CharPtr::operator =(char *ptr)
{
	SAFE_FREE(m_ptr);
	m_ptr = ptr;
	return m_ptr;
}


CharPtr::operator const char*()
{
	return m_ptr;
}


CharPtr::operator char*()
{
	return m_ptr;
}


char &CharPtr::operator [](const int index)
{
	return m_ptr[index];
}


const char CharPtr::operator [](const int index) const
{
	return m_ptr[index];
}


//char &CharPtr::operator *()
//{
//	return *m_ptr;
//}


bool CharPtr::operator ==(const char *ptr) const
{
	return (m_ptr == ptr) ? true : false;
}


bool CharPtr::operator !() const
{
	return m_ptr ? false : true;
}


CharPtr::operator bool() const
{
	return m_ptr ? true : false;
}


bool CharPtr::isNull()
{
	return m_ptr ? false : true;
}
