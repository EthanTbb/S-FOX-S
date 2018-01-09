#ifndef UIRESLOADER_HEAD_FILE
#define UIRESLOADER_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////

class FGUI_LIB_CLASS CResourceLoader
{
protected:
	CMarkup m_xml;
    LPCTSTR m_pstrtype;

public:
    CResourceLoader();
    bool LoadResource(STRINGorID xml,LPCTSTR type);
	bool LoadResource();

    CMarkup* GetMarkup();

    void GetLastErrorMessage(LPTSTR pstrMessage, SIZE_T cchMax) const;
    void GetLastErrorLocation(LPTSTR pstrSource, SIZE_T cchMax) const;
};

//////////////////////////////////////////////////////////////////////////////////////////////

#endif
