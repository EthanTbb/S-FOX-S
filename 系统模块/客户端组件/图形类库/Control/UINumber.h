#ifndef UINUMBER_HEAD_FILE
#define UINUMBER_HEAD_FILE

#pragma once

class FGUI_LIB_CLASS CNumberUI : public CLabelUI
{
public:
	CNumberUI();
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	void SetNumber(const double number);
	float GetNumber() const;
	void SetNumberImage(LPCTSTR pstrImage);
	LPCTSTR GetNumberImage();
	void SetNumberDecimal(bool bDecimal);
	bool IsNumberDecimal();
	void SetNumberSign(bool bSign);
	bool IsNumberSign();
	void SetNumberSeg(bool bSeg);
	bool IsNumberSeg();
	void SetReserved(const int reserved);
	int  GetReserved() const;
	void SetNumberFomart(LPCTSTR pstrFomart);
	LPCTSTR GetNumberFomart();
	void SetMinNumber(const int nMinValue);
	int  GetMinNumber();
	void SetMaxNumber(const int nMaxValue);
	int  GetMaxNumber();

	void DoInit();
	void SetPos(RECT rc);
	void PaintText(HDC hDC);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	//工厂函数
public:
	static CNumberUI * Create(CPaintManagerUI * pManager,CControlUI * pParent,LPCTSTR pszName,bool bInit=true);

private:
	virtual void SetText(LPCTSTR pstrText);
	LPCTSTR analysis(const double number);

protected:
	CStdString m_sNumberImage;
	CStdString m_sNumberFomart;
	CStdString m_sAnalysisText;
	double m_number;
	int m_nReserved;
	bool m_bDecimal; //小数输出
	bool m_bSign;
	bool m_bSeg;
	bool m_bNeedUpdateNumber;
	std::map<TCHAR, int> m_mapFormat;
	SIZE m_AutoSize;
	int	 m_nMinNumberValue;
	int	 m_nMaxNumberValue;
};

#endif