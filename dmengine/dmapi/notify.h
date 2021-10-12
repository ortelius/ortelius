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

#ifndef __notify_h

#define __notify_h


#include "list.h"
#include "hashtable.h"
#include "model.h"


class Attachment
{
private:
	char *m_name;
	class OutputStream *m_stream;
	bool m_ownStream;

public:
	Attachment(const char *name, class OutputStream *stream, bool ownStream);
	~Attachment();

	const char *name();
	unsigned char *data();
	long datasize();
};


class NotifyProviderImpl : public ProviderObjectImpl
{
protected:
	class NotifyProviderImplFactory &m_factory;
	class Notify &m_notify;

	NotifyProviderImpl(
		class NotifyProviderImplFactory &factory, DMINT32 implId, class Notify &notify);

public:
	virtual ~NotifyProviderImpl();

	virtual void notify(
		class ExtendedStmt &stmt, class OutputStream &body,
		List<class Attachment> *atts, class Context &ctx) = 0;

	// ProviderObjectImpl
	OBJECT_KIND kind()  { return OBJ_KIND_NOTIFY_IMPL; }
	virtual class Expr *getAttribute(const char *name, class Context &ctx);
	//virtual class DMArray *getArrayAttribute(const char *name, class Context &ctx)  { return NULL; }
	//virtual class Object *getObjectAttribute(const char *name, class Context &ctx)  { return NULL; }
	class Object *toObject() { return NULL; }
};


class NotifyProviderImplFactory
{
public:
	NotifyProviderImplFactory();
	virtual ~NotifyProviderImplFactory();

	virtual NotifyProviderImpl *create(
		DMINT32 implId, class Notify &parent, class ExtendedStmt &stmt,
		class Context &ctx) = 0;
};


class NotifyProviderImplRegistry
{
private:
	Hashtable<NotifyProviderImplFactory> m_factories;

	static NotifyProviderImplRegistry *s_instance;

public:
	NotifyProviderImplRegistry();

	static NotifyProviderImplRegistry &instance();
	static void cleanup();

	void registerFactory(const char *name, NotifyProviderImplFactory *factory);
	void registerBuiltIns(class DM &dm);

	NotifyProviderImplFactory *getFactory(class ProviderObjectDef &def);
};


#endif /*__notify_h*/
