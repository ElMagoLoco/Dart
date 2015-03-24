// LocParams.cpp : implementation file
//

#include "stdafx.h"
#include "LocParams.h"
#include "afxdialogex.h"


// LocParams dialog

IMPLEMENT_DYNAMIC(LocParams, CDialog)

LocParams::LocParams(CWnd* pParent /*=NULL*/)
	: CDialog(LocParams::IDD, pParent)
{

}

LocParams::~LocParams()
{
}

void LocParams::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LocParams, CDialog)
END_MESSAGE_MAP()

BOOL LocParams::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_nX = m_nY = m_nZ = 0;

	SetDlgItemInt(IDC_EDIT_LOCX, m_nX);
	SetDlgItemInt(IDC_EDIT_LOCY, m_nY);
	SetDlgItemInt(IDC_EDIT_LOCZ, m_nZ);

	return TRUE;
}

void LocParams::OnOK()
{
	m_nX = GetDlgItemInt(IDC_EDIT_LOCX);
	m_nY = GetDlgItemInt(IDC_EDIT_LOCY);
	m_nZ = GetDlgItemInt(IDC_EDIT_LOCZ);

	CDialog::OnOK();
}


// LocParams message handlers
