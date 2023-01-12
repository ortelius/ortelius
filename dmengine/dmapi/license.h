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

//
// (C) 2004 Trinem Consulting Ltd
//
// --------------------------------------------------------
// Version  Author  Date        Notes
// -------  ------  -----       -----
//    1      PAG    2004        Initial Release
// --------------------------------------------------------


#ifndef _LICENSE_H_
#define _LICENSE_H_

#include <stdio.h>
#include <time.h>

#ifdef THIS_IS_NOT_DEFINED
#  if defined(__sun)
     typedef int bool;
#    define true 1
#    define false 0
#  endif
#endif


 typedef enum _licerror_tag {
	 LicenceOkay = 0,
	 CouldNotOpenLicenseFile,
	 CorruptLicenseFile
 } LicenseError;


class License
{
private:
    int    Day;
    int    Month;
    int    Year;
    int    NodeCount;
    time_t OurLicenseTimeStamp;
    char   *LicenseFileName;
    char   *HostName;
    int CheckKeyAgainstDetails(
        char *Key,char *Customer,char *ExpiryDate,char *Nodes,char *HostName);
    int ValidateLicenseFile(
        FILE *in,int *Day,int *Month,int *Year,int *NodeCount,char **pHostName);

public:
    License();
    virtual ~License();

    // Member Functions
    void   OpenLicenseFile(const char *BaseDir,const char *FileName);
    LicenseError   LoadLicenseFile();
    void   GetExpiryDate(int *pDay,int *pMonth,int *pYear);
    int	   GetAllowedNodeCount();
    bool   CheckForExpiredLicense();
    bool   ReadRequired();
    time_t CurrentLicenseTimeStamp();
    bool   CheckForInvalidHostName();
}; //end class

#endif /* _LICENSE_H_ */
