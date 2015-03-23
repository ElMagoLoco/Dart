// PickUpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PickUpDlg.h"
#include "afxdialogex.h"


// PickUpDlg dialog

IMPLEMENT_DYNAMIC(PickUpDlg, CDialog)

PickUpDlg::PickUpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(PickUpDlg::IDD, pParent)
{

}

PickUpDlg::~PickUpDlg()
{
}

void PickUpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PickUpDlg, CDialog)
END_MESSAGE_MAP()

BOOL PickUpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_nSelected = -1;

	return TRUE;
}

void PickUpDlg::OnOK()
{
	m_nSelected = GetCheckedRadioButton(IDC_RADIO_HEAL, IDC_RADIO_AMMO_FIRE);

	CDialog::OnOK();
}


// PickUpDlg message handlers
