#ifndef _DMAPIEXPORT_H

#define _DMAPIEXPORT_H

#ifdef WIN32
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DMAPI_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DMAPI_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef DMAPI_EXPORTS
#define DMAPI_API __declspec(dllexport)
#define DMAPI_API_EXPORT __declspec(dllexport)
#else
#define DMAPI_API __declspec(dllimport)
#define DMAPI_API_EXPORT
#endif
#else
#define DMAPI_API
#define DMAPI_API_EXPORT
#endif /*WIN32*/

#endif /*_DMAPIEXPORT_H*/
