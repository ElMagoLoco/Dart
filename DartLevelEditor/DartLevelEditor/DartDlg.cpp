#include "DartLevelEditorApp.h"
#include "DartDlg.h"
#include "resource.h"

extern DartLevelEditorApp theApp;

BEGIN_MESSAGE_MAP(D3DPicWindow, CWnd)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(DartDlg, CDialog)	
	ON_BN_CLICKED(IDC_BTN_CREATEBASEFLOOR, &DartDlg::OnBnClickedBtnCreatebasefloor)
	ON_BN_CLICKED(IDC_BTN_SAVELEVEL, &DartDlg::OnBnClickedBtnSavelevel)
	ON_BN_CLICKED(IDC_BTN_MAKEDARTSPAWN, &DartDlg::OnBnClickedBtnMakedartspawn)
	ON_BN_CLICKED(IDC_BTN_BARTSPAWN, &DartDlg::OnBnClickedBtnBartspawn)
	ON_CBN_SELCHANGE(IDC_LIST_PAWNSELECTOR, &DartDlg::OnCbnSelchangeListPawnselector)
	ON_BN_CLICKED(IDC_BTN_ANTZSPAWN, &DartDlg::OnBnClickedBtnAntzspawn)
	ON_BN_CLICKED(IDC_BTN_LOADLEVEL, &DartDlg::OnBnClickedBtnLoadlevel)
ON_BN_CLICKED(IDC_BTN_CREATEWALL, &DartDlg::OnBnClickedBtnCreatewall)
ON_BN_CLICKED(IDC_BTN_DELETEOBJECT, &DartDlg::OnBnClickedBtnDeleteobject)
END_MESSAGE_MAP()

DartDlg::DartDlg() :
m_nFloorWidth(0),
m_nFloorLength(0),
m_bFloorCreated(false),
m_bBartCreated(false),
m_bDartCreated(false),
m_nWallWidth(0),
m_nWallLength(0),
m_nWallHeigth(0)
{
}


DartDlg::~DartDlg()
{
}

BOOL DartDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_d3dWnd.SubclassDlgItem(IDC_D3DWINDOW, this);

	SetDlgItemInt(IDC_EDIT_FLOORLENGHT, m_nFloorLength, TRUE);
	SetDlgItemInt(IDC_EDIT_FLOORWIDTH, m_nFloorWidth, TRUE);

	SetDlgItemInt(IDC_EDIT_WALLWIDTH, m_nWallWidth, TRUE);
	SetDlgItemInt(IDC_EDIT_WALLHEIGHT, m_nWallHeigth, TRUE);
	SetDlgItemInt(IDC_EDIT_WALLLENGTH, m_nWallLength, TRUE);

	return TRUE;
}

void DartDlg::OnOk()
{
	CDialog::OnOK(); 
}


void DartDlg::OnBnClickedBtnCreatebasefloor()
{
	m_nFloorWidth = GetDlgItemInt(IDC_EDIT_FLOORWIDTH, NULL, TRUE);
	m_nFloorLength = GetDlgItemInt(IDC_EDIT_FLOORLENGHT, NULL, TRUE);

	m_bFloorCreated = true;

	theApp.createFloor();
}


void DartDlg::OnBnClickedBtnSavelevel()
{
	CFileDialog dlg(FALSE, L"dlvl", L"NewDartLevel");

	CString filename;

	if (IDOK == dlg.DoModal()) {

		filename = dlg.GetFolderPath();
		filename += L"\\";
		filename += dlg.GetFileName();
	}

	theApp.saveLevel(filename);
}

void DartDlg::OnBnClickedBtnLoadlevel()
{
	CFileDialog dlg(TRUE, L"dlvl");

	CString filename;;

	if (IDOK == dlg.DoModal()){
		filename = dlg.GetFolderPath();
		filename += L"\\";
		filename += dlg.GetFileName();
	}

	theApp.loadLevel(filename);
}


void DartDlg::OnBnClickedBtnMakedartspawn()
{
	if (false == m_bDartCreated) {
		theApp.createDartSpawn();

		CComboBox* list = (CComboBox*)GetDlgItem(IDC_LIST_PAWNSELECTOR);
		list->InsertString(m_nPawnCount, L"Dart Spawn Point");
		list->SetCurSel(m_nPawnCount++);

		m_bDartCreated = true;
	}
	else {
		MessageBox(L"Dart Spawn Point already created", L"ERROR!", MB_OK | MB_ICONERROR);
	}
}


void DartDlg::OnBnClickedBtnBartspawn()
{
	if (false == m_bBartCreated) {
		theApp.createBartSpawn();

		CComboBox* list = (CComboBox*)GetDlgItem(IDC_LIST_PAWNSELECTOR);
		list->InsertString(m_nPawnCount, L"Bart Spawn Point");
		list->SetCurSel(m_nPawnCount++);

		m_bBartCreated = true;
	}
	else {
		MessageBox(L"Bart Spawn Point already created", L"ERROR!", MB_OK | MB_ICONERROR);
	}
}

void DartDlg::OnBnClickedBtnAntzspawn()
{
	static int count = -1;

	theApp.createAntzSpawn();

	CComboBox* list = (CComboBox*)GetDlgItem(IDC_LIST_PAWNSELECTOR);
	CString temp = L"Antz Spawn Point";
	wchar_t temp1[32];
	_itow_s(++count, temp1, 10);
	temp += temp1;
	list->InsertString(m_nPawnCount, temp);
	list->SetCurSel(m_nPawnCount++);
}

void DartDlg::OnCbnSelchangeListPawnselector()
{
	CComboBox* list = (CComboBox*)GetDlgItem(IDC_LIST_PAWNSELECTOR);

	int selected = list->GetCurSel();

	theApp.setPawnToMove(selected);

	CWnd::OnKillFocus(list);
}

void DartDlg::insertToPawnList(const wchar_t* pawnName)
{
	CComboBox* list = (CComboBox*)GetDlgItem(IDC_LIST_PAWNSELECTOR);
	list->InsertString(m_nPawnCount, pawnName);
	list->SetCurSel(m_nPawnCount++);
}

void DartDlg::OnBnClickedBtnCreatewall()
{
	static int count = -1;

	m_nWallHeigth = GetDlgItemInt(IDC_EDIT_WALLHEIGHT);
	m_nWallLength = GetDlgItemInt(IDC_EDIT_WALLLENGTH);
	m_nWallWidth = GetDlgItemInt(IDC_EDIT_WALLWIDTH);

	if (m_nWallWidth < 1 || m_nWallLength < 1 || m_nWallHeigth < 1) {
		MessageBox(L"Wall dimensions must be > 0", L"ERROR", MB_OK | MB_ICONERROR);
		return;
	}

	theApp.createWall(D3DXVECTOR3((float)m_nWallWidth, (float)m_nWallHeigth, (float)m_nWallLength));

	CComboBox* list = (CComboBox*)GetDlgItem(IDC_LIST_PAWNSELECTOR);

	CString temp = L"Wall";
	wchar_t temp1[32];
	_itow_s(++count, temp1, 10);
	temp += temp1;
	list->InsertString(m_nPawnCount, temp);
	list->SetCurSel(m_nPawnCount++);
}


void DartDlg::OnBnClickedBtnDeleteobject()
{
	CComboBox* list = (CComboBox*)GetDlgItem(IDC_LIST_PAWNSELECTOR);

	int selected = list->GetCurSel();
	list->DeleteString(selected);

	theApp.deletePawn(selected);

	--m_nPawnCount;
}
