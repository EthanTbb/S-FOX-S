#ifndef DLG_GAME_RULE_HEAD_FILE2
#define DLG_GAME_RULE_HEAD_FILE2

#pragma once

//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class  CDlgSetGameRule : public CSkinDialog
{
  //�ؼ�����
public:
  CWebBrowser *         m_pHtmlBrowser;           //����ؼ�

  //��������
public:
  //���캯��
  CDlgSetGameRule();
  //��������
  virtual ~CDlgSetGameRule();

  //���غ���
protected:
  //�ؼ���
  virtual VOID DoDataExchange(CDataExchange * pDX);
  //��ʼ������
  virtual BOOL OnInitDialog();
  //ȷ����Ϣ
  virtual VOID OnOK();
  //ȡ����Ϣ
  virtual VOID OnCancel();

  //��Ϣӳ��
public:
  //������Ϣ
  VOID OnNcDestroy();
  //λ����Ϣ
  VOID OnSize(UINT nType, INT cx, INT cy);

  DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif