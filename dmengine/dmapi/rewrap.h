#ifndef __rewrap_h


class RegExpWrapper
{
private:
	struct regexp *m_re;

public:
	RegExpWrapper(const char *re, bool isPattern = false);
	~RegExpWrapper();

	int match(const char *str);
	char *getMatch(int sub);

	char *replaceAll(const char *str, const char *replace, bool noIterate=false);
};


#define __rewrap_h

#endif /*__rewrap_h*/
