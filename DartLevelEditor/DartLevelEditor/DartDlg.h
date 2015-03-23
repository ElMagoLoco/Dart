#pragma once
#include "D3DUtils.h"

class D3DPicWindow : public CWnd
{
public:
	afx_msg BOOL OnEraseBkgnd(CDC *pDC) { return TRUE; }

	DECLARE_MESSAGE_MAP()
};

class DartDlg : public CDialog
{
public:
	D3DPicWindow	m_d3dWnd;

	virtual BOOL OnInitDialog();
	virtual void OnOk();
	virtual void OnCancel() { PostQuitMessage(0); }

	void insertToPawnList(const wchar_t* pawnName);

	int getFloorLength() const { return m_nFloorLength; }
	void setFloorLength(int val) { m_nFloorLength = val; }
	int getFloorWidth() const { return m_nFloorWidth; }
	void setFloorWidth(int val) { m_nFloorWidth = val; }
	bool getFloorCreated() const { return m_bFloorCreated; }
	void setFloorCreated(bool val) { m_bFloorCreated = val; }
	bool getBartCreated() const { return m_bBartCreated; }
	void setBartCreated(bool val) { m_bBartCreated = val; }
	bool getDartCreated() const { return m_bDartCreated; }
	void setDartCreated(bool val) { m_bDartCreated = val; }
	int getWallWidth() const { return m_nWallWidth; }
	void setWallWidth(int val) { m_nWallWidth = val; }
	int getWallHeigth() const { return m_nWallHeigth; }
	void setWallHeigth(int val) { m_nWallHeigth = val; }
	int getWallLength() const { return m_nWallLength; }
	void setWallLength(int val) { m_nWallLength = val; }
	int getSelectedPowerUp() const { return m_nSelectedPowerUp; }

	DartDlg();
	~DartDlg();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBtnCreatebasefloor();
	afx_msg void OnBnClickedBtnSavelevel();
	afx_msg void OnBnClickedBtnMakedartspawn();
	afx_msg void OnBnClickedBtnBartspawn();
	afx_msg void OnCbnSelchangeListPawnselector();
	afx_msg void OnBnClickedBtnAntzspawn();
	afx_msg void OnBnClickedBtnLoadlevel();
	afx_msg void OnBnClickedBtnCreatewall();
	afx_msg void OnBnClickedBtnDeleteobject();
	afx_msg void OnBnClickedBtnPickupspawn();

protected:
	int			m_nWallWidth;
	int			m_nWallHeigth;
	int			m_nWallLength;
	int			m_nFloorLength;
	int			m_nFloorWidth;
	int			m_nPawnCount;
	int			m_nSelectedPowerUp;

	bool		m_bFloorCreated;
	bool		m_bBartCreated;
	bool		m_bDartCreated;
};

