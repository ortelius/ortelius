#include <stdio.h>

#include "notify.h"
#include "model.h"
#include "outputstream.h"


///////////////////////////////////////////////////////////////////////////////
// Attachment
///////////////////////////////////////////////////////////////////////////////

Attachment::Attachment(const char *name, OutputStream *stream, bool ownStream)
	: m_name(DUP_NULL(name)), m_stream(stream), m_ownStream(ownStream)
{}


Attachment::~Attachment()
{
	SAFE_FREE(m_name);
	if(m_ownStream) { 
		SAFE_DELETE(m_stream);
	} else {
		m_stream = NULL;	// don't own
	}
}


const char *Attachment::name()
{
	return m_name;
}


unsigned char *Attachment::data()
{
	return m_stream ? (unsigned char*) m_stream->buffer() : NULL;
}


long Attachment::datasize()
{
	return m_stream ? m_stream->size() : 0;
}


///////////////////////////////////////////////////////////////////////////////
// NotifyProviderImpl
///////////////////////////////////////////////////////////////////////////////

NotifyProviderImpl::NotifyProviderImpl(
		NotifyProviderImplFactory &factory, DMINT32 implId, Notify &notify
	)
	: ProviderObjectImpl(implId, notify), m_factory(factory), m_notify(notify)
{}

/*virtual*/ NotifyProviderImpl::~NotifyProviderImpl()
{}


Expr *NotifyProviderImpl::getAttribute(const char *name, class Context &ctx)
{
	return ProviderObjectImpl::getAttribute(name, ctx);
}


///////////////////////////////////////////////////////////////////////////////
// NotifyProviderImplFactory
///////////////////////////////////////////////////////////////////////////////

NotifyProviderImplFactory::NotifyProviderImplFactory()
{}


/*virtual*/ NotifyProviderImplFactory::~NotifyProviderImplFactory()
{}


///////////////////////////////////////////////////////////////////////////////
// NotifyImplRegistry
///////////////////////////////////////////////////////////////////////////////

NotifyProviderImplRegistry *NotifyProviderImplRegistry::s_instance = NULL;


NotifyProviderImplRegistry::NotifyProviderImplRegistry()
	: m_factories(false, true)
{}


/*static*/ NotifyProviderImplRegistry &NotifyProviderImplRegistry::instance()
{
	if(!s_instance) {
		s_instance = new NotifyProviderImplRegistry();
	}
	return *s_instance;
}


/*static*/ void NotifyProviderImplRegistry::cleanup()
{
	SAFE_DELETE(s_instance);
}


void NotifyProviderImplRegistry::registerFactory(const char *name, NotifyProviderImplFactory *factory)
{
	m_factories.put(name, factory);
}

extern "C" int smtpemail_PluginStart(DM &dm);
extern "C" int txtlocal_PluginStart(DM &dm);
extern "C" int slack_PluginStart(DM &dm);
extern "C" int hipchat_PluginStart(DM &dm);

void NotifyProviderImplRegistry::registerBuiltIns(DM &dm)
{
	//registerFactory("smtpemail", new SmtpEmailNotifyImplFactory());

	smtpemail_PluginStart(dm);
	txtlocal_PluginStart(dm);
	slack_PluginStart(dm);
	hipchat_PluginStart(dm);
}


NotifyProviderImplFactory *NotifyProviderImplRegistry::getFactory(ProviderObjectDef &def)
{
	return m_factories.get(def.name());
}
