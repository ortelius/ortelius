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
