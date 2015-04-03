// EnemyTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EnemyTypeDlg.h"
#include "afxdialogex.h"


// EnemyTypeDlg dialog

IMPLEMENT_DYNAMIC(EnemyTypeDlg, CDialog)

EnemyTypeDlg::EnemyTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(EnemyTypeDlg::IDD, pParent)
{

}

EnemyTypeDlg::~EnemyTypeDlg()
{
}

void EnemyTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(EnemyTypeDlg, CDialog)
END_MESSAGE_MAP()

BOOL EnemyTypeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_nSelected = -1;

	return TRUE;
}

void EnemyTypeDlg::OnOK()
{
	m_nSelected = GetCheckedRadioButton(IDC_RADIO_ENEMYMELEE, IDC_RADIO_ENEMYSEED);

	CDialog::OnOK();
}


// EnemyTypeDlg message handlers
