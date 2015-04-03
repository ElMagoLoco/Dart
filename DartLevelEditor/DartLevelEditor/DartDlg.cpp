#include "DartLevelEditorApp.h"
#include "DartDlg.h"
#include "PickUpDlg.h"
#include "EnemyTypeDlg.h"
#include "LocParams.h"
#include "EditorPawn.h"
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
ON_BN_CLICKED(IDC_BTN_PICKUPSPAWN, &DartDlg::OnBnClickedBtnPickupspawn)
ON_BN_CLICKED(IDC_BTN_FLEEPOINT, &DartDlg::OnBnClickedBtnFleepoint)
ON_BN_CLICKED(IDC_BTN_SETLOCATION, &DartDlg::OnBnClickedBtnSetlocation)
ON_BN_CLICKED(IDC_BTN_GOALSPWAN, &DartDlg::OnBnClickedBtnGoalspwan)
END_MESSAGE_MAP()

DartDlg::DartDlg() :
m_nFloorWidth(0),
m_nFloorLength(0),
m_bFloorCreated(false),
m_bBartCreated(false),
m_bDartCreated(false),
m_bGoalCreated(false),
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

		insertToPawnList(L"Dart Spawn Point");

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

		insertToPawnList(L"Bart Spawn Point");

		m_bBartCreated = true;
	}
	else {
		MessageBox(L"Bart Spawn Point already created", L"ERROR!", MB_OK | MB_ICONERROR);
	}
}

void DartDlg::OnBnClickedBtnAntzspawn()
{
	// TODO: fix so that this count gets reduced when Antz is deleted
	static int count = -1;

	EnemyTypeDlg dlg;

	int selected = -1;
	if (IDOK == dlg.DoModal()) {
		selected = dlg.m_nSelected;
	}

	switch (selected) {
	case IDC_RADIO_ENEMYMELEE: 
	{
		theApp.createEnemySpawn(EditorPawn::PawnType::PT_EnemyMeleeSpawnLocale);
		CComboBox* list = (CComboBox*)GetDlgItem(IDC_LIST_PAWNSELECTOR);
		CString temp = L"Melee Spawn Point_";
		wchar_t temp1[32];
		_itow_s(++count, temp1, 10);
		temp += temp1;
		insertToPawnList(temp);
	}
	break;
	case IDC_RADIO_ENEMYFIRE:
	{
		theApp.createEnemySpawn(EditorPawn::PawnType::PT_EnemyFireSpawnLocale);
		CComboBox* list = (CComboBox*)GetDlgItem(IDC_LIST_PAWNSELECTOR);
		CString temp = L"Fire Spawn Point_";
		wchar_t temp1[32];
		_itow_s(++count, temp1, 10);
		temp += temp1;
		insertToPawnList(temp);
	}
	break;
	case IDC_RADIO_ENEMYSEED:
	{
		theApp.createEnemySpawn(EditorPawn::PawnType::PT_EnemySeedSpawnLocale);
		CComboBox* list = (CComboBox*)GetDlgItem(IDC_LIST_PAWNSELECTOR);
		CString temp = L"Seed Spawn Point_";
		wchar_t temp1[32];
		_itow_s(++count, temp1, 10);
		temp += temp1;
		insertToPawnList(temp);
	}
	break;
	}
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
	// TODO: fix so that this count gets decremented as walls are deleted.
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

	CString temp = L"Wall_";
	wchar_t temp1[32];
	_itow_s(++count, temp1, 10);
	temp += temp1;
	insertToPawnList(temp);
}


void DartDlg::OnBnClickedBtnDeleteobject()
{
	CComboBox* list = (CComboBox*)GetDlgItem(IDC_LIST_PAWNSELECTOR);

	int selected = list->GetCurSel();
	list->DeleteString(selected);

	theApp.deletePawn(selected);
	// TODO: fix so that objects counts are properly decremented so the strings like WallX in the combolist show up right
	--m_nPawnCount;
}


void DartDlg::OnBnClickedBtnPickupspawn()
{
	PickUpDlg dlg;

	int selected = -1;
	if (IDOK == dlg.DoModal()) {
		selected = dlg.m_nSelected;
	}

	static int count = -1;

	switch (selected) {
	case IDC_RADIO_HEAL: {
		theApp.createPickUp(EditorPawn::PawnType::PT_Pickup_Heal);

		CComboBox* list = (CComboBox*)GetDlgItem(IDC_LIST_PAWNSELECTOR);

		CString temp = L"PickUp_Heal_";
		wchar_t temp1[32];
		_itow_s(++count, temp1, 10);
		temp += temp1;
		insertToPawnList(temp);
	}
	break;
	
	case IDC_RADIO_BONUS: {
		theApp.createPickUp(EditorPawn::PawnType::PT_Pickup_Bonus);

		CComboBox* list = (CComboBox*)GetDlgItem(IDC_LIST_PAWNSELECTOR);

		CString temp = L"PickUp_Bonus_";
		wchar_t temp1[32];
		_itow_s(++count, temp1, 10);
		temp += temp1;
		insertToPawnList(temp);
	}
	break;
	case IDC_RADIO_AMMO_SEED: {
		theApp.createPickUp(EditorPawn::PawnType::PT_Pickup_Ammo_Seed);

		CComboBox* list = (CComboBox*)GetDlgItem(IDC_LIST_PAWNSELECTOR);

		CString temp = L"PickUp_Seed_";
		wchar_t temp1[32];
		_itow_s(++count, temp1, 10);
		temp += temp1;
		insertToPawnList(temp);
	}
	break;
	case IDC_RADIO_AMMO_FIRE: {
		theApp.createPickUp(EditorPawn::PawnType::PT_Pickup_Ammo_Fire);

		CComboBox* list = (CComboBox*)GetDlgItem(IDC_LIST_PAWNSELECTOR);

		CString temp = L"PickUp_Fire_";
		wchar_t temp1[32];
		_itow_s(++count, temp1, 10);
		temp += temp1;
		insertToPawnList(temp);
	}
	break;
	}
}

void DartDlg::OnBnClickedBtnFleepoint()
{
	static int count = -1;

	theApp.createFleePointSpawn();

	CComboBox* list = (CComboBox*)GetDlgItem(IDC_LIST_PAWNSELECTOR);

	CString temp = L"FleePoint_";
	wchar_t temp1[32];
	_itow_s(++count, temp1, 10);
	temp += temp1;
	insertToPawnList(temp);
}


void DartDlg::OnBnClickedBtnSetlocation()
{
	LocParams dlg;

	if (IDOK == dlg.DoModal()) {
		CComboBox* list = (CComboBox*)GetDlgItem(IDC_LIST_PAWNSELECTOR);

		int selected = list->GetCurSel();

		float x = (float)dlg.m_nX;
		float y = (float)dlg.m_nY;
		float z = (float)dlg.m_nZ;

		theApp.setPawnLocation(selected, D3DXVECTOR3(x, y, z));
	}
}


void DartDlg::OnBnClickedBtnGoalspwan()
{
	if (false == m_bGoalCreated) {
		theApp.createGoalSpawn();

		insertToPawnList(L"Goal Spawn Point");

		m_bGoalCreated = true;
	}
	else {
		MessageBox(L"Goal Spawn Point already created", L"ERROR!", MB_OK | MB_ICONERROR);
	}
}
