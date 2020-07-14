typedef struct options_tag{
const char	*option;
char	argument;
const char	*envname;
} OPTIONS;

#ifdef __cplusplus
extern "C"{
#endif 
int ScanOptions(OPTIONS *,int,int,char **);
#ifdef __cplusplus
}
#endif
