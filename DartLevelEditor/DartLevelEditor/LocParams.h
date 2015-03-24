#pragma once
#include "resource.h"

// LocParams dialog

class LocParams : public CDialog
{
	DECLARE_DYNAMIC(LocParams)

public:
	int		m_nX;
	int		m_nY;
	int		m_nZ;

	LocParams(CWnd* pParent = NULL);   // standard constructor
	virtual ~LocParams();

// Dialog Data
	enum { IDD = IDD_DLG_LOCATIONPARAMS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual void OnOK();

};
