#ifndef IMP_IDISPATCH_HEAD_FILE
#define IMP_IDISPATCH_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

class CImpIDispatch : public IDispatch
{
protected:
	ULONG               m_cRef;

public:
	CImpIDispatch(void);
	~CImpIDispatch(void);

	STDMETHODIMP QueryInterface(REFIID, void **);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	//IDispatch
	STDMETHODIMP GetTypeInfoCount(UINT* pctinfo);
	STDMETHODIMP GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo * * ppTInfo);
	STDMETHODIMP GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId);
	STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS  *pDispParams, VARIANT  *pVarResult,EXCEPINFO *pExcepInfo, UINT *puArgErr);
};

//////////////////////////////////////////////////////////////////////////////////

#endif
