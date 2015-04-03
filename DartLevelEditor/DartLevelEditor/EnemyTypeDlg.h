#pragma once
#include "resource.h"

// EnemyTypeDlg dialog

class EnemyTypeDlg : public CDialog
{
	DECLARE_DYNAMIC(EnemyTypeDlg)

public:
	int		m_nSelected;

	EnemyTypeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~EnemyTypeDlg();

// Dialog Data
	enum { IDD = IDD_DLG_ENEMYTYPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual void OnOK();

};
