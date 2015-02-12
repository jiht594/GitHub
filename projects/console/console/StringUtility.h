#pragma once
class StringUtility
{
public:
    StringUtility(void);
    ~StringUtility(void);
public:
    //the return value is needed to free;
    static char* UnicodeToAnsi(const wchar_t *pStr);
     //the return value is needed to free;s
    static wchar_t* AnsiToUnicode(const char *pStr);

    static void freeArrayChar(const char *);
    static void freeArrayWChar(const wchar_t *);
};

