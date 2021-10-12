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

#ifndef __autoptr_h

#define __autoptr_h


template<class T> class AutoPtr {
private:
	T *m_ptr;

public:
	AutoPtr<T>();
	AutoPtr<T>(T *ptr);
	~AutoPtr<T>();

	T *operator =(T *ptr);
	T *operator ->();
	operator T*();
	T &operator *();
	bool operator ==(T *ptr) const;
	bool operator !() const;
	operator bool() const;
	bool isNull();
};


///////////////////////////////////////////////////////////////////////////////

#include "autoptr.impl"


#endif /*__autoptr_h*/
