#include "Stdafx.h"
#include "Resource.h"
#include "DlgGameRule.h"
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgSetGameRule, CSkinDialog)
  ON_WM_SIZE()
  ON_WM_NCDESTROY()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgSetGameRule::CDlgSetGameRule() : CSkinDialog(IDD_GAME_RULE)
{
  //���ñ���
  m_pHtmlBrowser=NULL;

  return;
}

//��������
CDlgSetGameRule::~CDlgSetGameRule()
{
  //ɾ���ؼ�
  SafeDelete(m_pHtmlBrowser);

  return;
}

//�ؼ���
VOID CDlgSetGameRule::DoDataExchange(CDataExchange* pDX)
{
  __super::DoDataExchange(pDX);
}

//��ʼ������
BOOL CDlgSetGameRule::OnInitDialog()
{
  __super::OnInitDialog();

  //��ȡλ��
  CRect rcClient;
  GetClientRect(&rcClient);

  //��������
  INT nTViewBorder=m_SkinAttribute.m_EncircleInfoView.nTBorder;
  INT nBViewBorder=m_SkinAttribute.m_EncircleInfoView.nBBorder;
  INT nLViewBorder=m_SkinAttribute.m_EncircleInfoView.nLBorder;
  INT nRViewBorder=m_SkinAttribute.m_EncircleInfoView.nRBorder;

  //��������
  CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();
  tagGameAttribute * pGameAttribute=pGlobalUnits->m_ClientKernelModule->GetGameAttribute();

  //�����ؼ�
  try
  {
    //λ�ö���
    CRect rcBrowse;
    rcBrowse.left=nLViewBorder;
    rcBrowse.top=nTViewBorder;
    rcBrowse.right=rcClient.Width()-nLViewBorder;
    rcBrowse.bottom=rcClient.Height()-nBViewBorder;

    //�����ؼ�
    m_pHtmlBrowser=new CWebBrowser;
    m_pHtmlBrowser->Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcBrowse,this,101);

    //�����ַ
    TCHAR szRuleUrl[256]=TEXT("");
    _sntprintf(szRuleUrl,CountArray(szRuleUrl),TEXT("%s/GameRules.aspx?KindID=%ld"),szPlatformLink,pGameAttribute->wKindID);

    //�������
    m_pHtmlBrowser->Navigate(szRuleUrl,NULL,NULL,NULL,NULL);
  }
  catch(...)
  {
    ASSERT(FALSE);
  }

  //�������
  TCHAR szTitle[128]=TEXT("");
  _sntprintf(szTitle,CountArray(szTitle),TEXT("��%s������˵��"),pGameAttribute->szGameName);

  //���ô���
  SetWindowText(szTitle);
//  EnableButton(WS_MAXIMIZEBOX|WS_MINIMIZEBOX);

  return TRUE;
}

//ȷ����Ϣ
VOID CDlgSetGameRule::OnOK()
{
  return;
}

//ȡ����Ϣ
VOID CDlgSetGameRule::OnCancel()
{
  //���ٴ���
  DestroyWindow();

  return;
}

//������Ϣ
VOID CDlgSetGameRule::OnNcDestroy()
{
  //���ٿؼ�
  if(m_pHtmlBrowser && m_pHtmlBrowser->GetSafeHwnd())
  {
    m_pHtmlBrowser->DestroyWindow();
  }

  //ɾ���ؼ�
  SafeDelete(m_pHtmlBrowser);

  __super::OnNcDestroy();
}

//λ����Ϣ
VOID CDlgSetGameRule::OnSize(UINT nType, INT cx, INT cy)
{
  __super::OnSize(nType, cx, cy);

  //�����ؼ�
  if(m_pHtmlBrowser->GetSafeHwnd()!=NULL)
  {
    m_pHtmlBrowser->MoveWindow(m_SkinAttribute.m_EncircleInfoView.nLBorder,m_SkinAttribute.m_EncircleInfoView.nTBorder,
                               cx-m_SkinAttribute.m_EncircleInfoView.nLBorder*2,cy-m_SkinAttribute.m_EncircleInfoView.nTBorder-m_SkinAttribute.m_EncircleInfoView.nRBorder);
  }

  return;
}

//////////////////////////////////////////////////////////////////////////
