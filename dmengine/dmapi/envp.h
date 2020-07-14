#ifndef __envp_h

#define __envp_h

class Envp
{
private:
	char **m_envp;

#ifdef WIN32
	struct _RTL_CRITICAL_SECTION *m_criticalSection;
#endif /*WIN32*/

	void internalPut(const char* name, char *buf);

public:
	Envp();
	Envp(class ScopeStack &stack);
	Envp(Envp &copy);
	~Envp();
	void init(char **envp);
	char *get(const char *name);
	void put(const char* name, const char *value);
	void put(const char* name, int value, int digits);
	void putTri(const char* name, const char *value);
	void dump(const char *filename = 0);
	char **ptr();
};

#endif /*__environment_h*/
