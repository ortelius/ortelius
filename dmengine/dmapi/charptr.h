/*
 * Copyright (c) 2021 Linux Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
