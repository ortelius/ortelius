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
