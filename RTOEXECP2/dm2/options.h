typedef struct options_tag{
char	*option;
char	argument;
char	*envname;
} OPTIONS;

#ifdef __cplusplus
extern "C"{
#endif 
int ScanOptions(OPTIONS *,int,int,char **);
void SetOptionsFromFile(OPTIONS *,int,FILE *);
#ifdef __cplusplus
}
#endif
