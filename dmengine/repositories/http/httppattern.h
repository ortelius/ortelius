#ifndef _httppattern_

class HttpPattern
{
private:
	char	*m_pattern;		// Original pattern
	char	*prefix;		// Prefix (bit before the varying fields)
	char	*postfix;		// Postfix (bit after the varying fields)
	int		m_NumFields;	// total number of fields in pattern
	int		*m_curfv;			// array of current field values
	int		*m_minfv;			// array of current field values
	int		*m_maxfv;			// array of current field values

public:
	HttpPattern(char *pattern);
	~HttpPattern();
	char	*getTestFilename();
	void	chop(int field,bool tob);
	void	setMidPoint(int field);
	int		getFieldCount();
	int		getRange(int field);
	int		getFieldValue(int field);
	bool	incrField(int field);
	void	setField(int field,int newval);
	char	*getVersionString();
};

#endif
