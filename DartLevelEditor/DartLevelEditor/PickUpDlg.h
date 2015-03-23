#pragma once
#include "resource.h"

// PickUpDlg dialog

class PickUpDlg : public CDialog
{
	DECLARE_DYNAMIC(PickUpDlg)

public:
	PickUpDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~PickUpDlg();

	int		m_nSelected;

// Dialog Data
	enum { IDD = IDD_DLG_PICKUPTYPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	virtual void OnOK();

};
