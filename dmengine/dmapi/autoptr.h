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
