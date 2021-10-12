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
