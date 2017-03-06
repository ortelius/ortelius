#ifndef __FTPFILELIST
#define __FTPFILELIST
typedef struct ftp_folder_list_tag{
char			*FolderName;
struct ftp_folder_list_tag	*next;
} FTP_FOLDER_LIST;

typedef struct datetime_tag{
short	Year;
short	Month;
short	Day;
short	Hours;
short	Minutes;
 short	Seconds;
} DATETIME;

typedef struct ftp_file_list{
char					*FileName;
bool					Directory;
long					SizeInBytes;
DATETIME				ModifiedDate;
char					*szModifiedDate;
char					*Attributes;
char					*Owner;
char					*Group;
struct ftp_file_list	*next;
} FTP_FILE_LIST;

FTP_FILE_LIST *DecodeUnixListOutput(void *d,unsigned long NumBytes);

#endif
