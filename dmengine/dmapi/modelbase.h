#ifndef __modelbase_h

#define __modelbase_h


#include "dmapiexport.h"


/**
 * Never insert new entries into this enum - the integer values are used in
 * the database in dm_providerdef.
 */
typedef enum object_kind_tag {
	OBJ_KIND_NONE			=  0,
	//OBJ_KIND_GROUP			=  1,
	OBJ_KIND_ENVIRONMENT	=  2,
	OBJ_KIND_DOMAIN         =  3,
	OBJ_KIND_SERVER			=  4,
	OBJ_KIND_APPLICATION	=  5,
	OBJ_KIND_USER			=  6,
	OBJ_KIND_USERGROUP		=  7,
	OBJ_KIND_REPOSITORY		=  8,
	OBJ_KIND_NOTIFY			=  9,
	OBJ_KIND_DATASOURCE		= 10,
	OBJ_KIND_ACTION         = 11,
	OBJ_KIND_DROPZONE       = 12,
	OBJ_KIND_COMPONENT   	= 13,
	OBJ_KIND_COMPONENTITEM 	= 14,
	OBJ_KIND_DROPZONEFILE   = 15,
	OBJ_KIND_TRANSFER       = 16,
	OBJ_KIND_APPVERSION	    = 17,
	OBJ_KIND_TASK		    = 18,
	OBJ_KIND_CREDENTIALS    = 19,
	OBJ_KIND_DEFECT			= 20,
	OBJ_KIND_BUILDENGINE	= 21,

	OBJ_KIND_REPOSITORY_IMPL  = 108,
	OBJ_KIND_NOTIFY_IMPL      = 109,
	OBJ_KIND_DATASOURCE_IMPL  = 110,
	OBJ_KIND_STATEMENT_IMPL   = 111,
	OBJ_KIND_TASK_IMPL	      = 118,

	OBJ_KIND_PLUGIN_OBJECT   = 200
} OBJECT_KIND;


class DMAPI_API IObject
{
public:
	virtual OBJECT_KIND kind() = 0;
	virtual class Expr *getAttribute(const char *name, class Context &ctx) = 0;
	virtual class Object *toObject() = 0;
};


class DMAPI_API ILocatable
{
public:
	virtual int fileno() = 0;
	virtual int lineno() = 0;
	virtual int fragno() = 0;
};


#endif /*__modelbase_h*/
