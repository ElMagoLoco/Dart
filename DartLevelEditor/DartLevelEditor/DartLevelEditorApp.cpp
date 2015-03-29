#include <fstream>
#include "DartLevelEditorApp.h"
#include "DXInput.h"
#include "Vertex.h"
#include "Timer.h"
#include "BartPawn.h"
#include "DartPawn.h"
#include "AntzPawn.h"
#include "WallPawn.h"
#include "PickUpPawn.h"
#include "FleePointPawn.h"
#include "resource.h"

using namespace Fugui;
using std::vector;
using std::wofstream;
using std::wifstream;

#define		MY_FVF	D3DFVF_XYZ | D3DFVF_DIFFUSE

DartLevelEditorApp::DartLevelEditorApp() :
m_fFarZ(10000.0f),
m_fNearZ(0.1f),
m_fFOV(45.0f),
m_nMovePawnID(0)
{
}


DartLevelEditorApp::~DartLevelEditorApp()
{
}

BOOL DartLevelEditorApp::InitInstance()
{
	if (FALSE == CWinApp::InitInstance())
		return FALSE;
 	if (FALSE == m_dlg.Create(IDD_LEVEL_EDITOR_DLG))
 		return FALSE;

	InitCommonControls();

	m_bFloorCreated = false;

	SetRegistryKey(L"Dart Level Editor");

	m_pMainWnd = &m_dlg;

	m_hD3DWnd = GetDlgItem(m_dlg, IDC_D3DWINDOW);

	mhMainWnd = m_hD3DWnd;

	initDirect3D();

	InitAllVertexDeclarations();

	g_input->init(mhMainWnd, AfxGetInstanceHandle(), DISCL_EXCLUSIVE | DISCL_FOREGROUND, DISCL_EXCLUSIVE | DISCL_FOREGROUND);

	m_camera.pos() = D3DXVECTOR3(0.0f, 3000.0f, -50.0f);
	RECT client;
	GetClientRect(mhMainWnd, &client);
	m_fAspect = (float)client.right / (float)client.bottom;
	m_camera.setLens(D3DXToRadian(m_fFOV), m_fAspect, m_fNearZ, m_fFarZ);
	m_camera.lookAt(m_camera.pos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	m_fxs.createFxPool();
	m_nLevelShaderID = m_fxs.loadFx(L"Effects\\LevelEditorShader.fx", L"LevelEditorTech", L"gWorld");

	D3DXMatrixIdentity(&m_matWorld);

	m_nBoxMeshID = createCube(1.0f, 1.0f, 1.0f);
	m_nSphereMeshID = createSphere(3.0f, 12, 10);
	m_nTorusMeshID = createTorus(2.0f, 4.0f, 10, 20);

	Timer::Get()->setTimer();

	return TRUE;
}

BOOL DartLevelEditorApp::OnIdle(LONG lCount)
{
	CWinApp::OnIdle(lCount);

	Timer::Get()->updateTime();

	float dt = Timer::Get()->getDeltaTime();

	updateScene(dt);
	drawScene();

	return TRUE;
}

int DartLevelEditorApp::ExitInstance()
{
	m_dlg.DestroyWindow();

	g_input->shutdown();

	for (UINT i = 0; i < m_meshList.size(); ++i)
		SAFE_RELEASE(m_meshList[i]);

	SAFE_RELEASE(m_pFloorVB);
	SAFE_RELEASE(m_pFloorIB);
	
	CleanUp();

	DestroyAllVertexDeclarations();

	return CWinApp::ExitInstance();
}

void DartLevelEditorApp::drawScene()
{
	HR(gD3DDevice->Clear(0, 0, D3DCLEAR_TARGET/* | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL*/,
		0xff0f0f0f, m_fNearZ, 0));
	HR(gD3DDevice->BeginScene());

 	m_fxs.getFx(m_nLevelShaderID).pFx->SetTechnique(m_fxs.getFx(m_nLevelShaderID).hTech);

	if (m_bFloorCreated)
		drawFloor(D3DFILL_SOLID);

	if (m_pawnList.size() > 0)
		drawPawns();

	HR(gD3DDevice->EndScene());
	HR(gD3DDevice->Present(0, 0, 0, 0));
}

void DartLevelEditorApp::updateScene(float dt)
{
	g_input->pollBothTypes();

	m_camera.update(dt, 0.0f);

	m_camera.setLens(D3DXToRadian(m_fFOV), m_fAspect, m_fNearZ, m_fFarZ);

	movePawn(m_nMovePawnID, dt);
}

void DartLevelEditorApp::createFloor()
{
	if (m_bFloorCreated) // TODO: disable the button instead
		return;

	if (m_dlg.getFloorLength() < 1 || m_dlg.getFloorWidth() < 1) {
		MessageBox(NULL, L"Floor Width and Length most be > 0", L"INPUT ERROR", MB_OK | MB_ICONERROR);
		return;
	}

	m_nNumFloorTris = UINT((m_dlg.getFloorLength() - 1) * (m_dlg.getFloorWidth() - 1) * 2);
	m_nNumFloorVerts = UINT(m_dlg.getFloorLength() * m_dlg.getFloorWidth());

	genTriGrid(m_dlg.getFloorLength(), m_dlg.getFloorWidth(), 1.0f, 1.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_floorVerts, m_floorIdxs);

	HR(gD3DDevice->CreateVertexBuffer(sizeof(VertexCol) * m_floorVerts.size(), D3DUSAGE_WRITEONLY, MY_FVF, D3DPOOL_MANAGED, &m_pFloorVB, NULL));
	 
	VertexCol* pVB = NULL;
	HR(m_pFloorVB->Lock(0, 0, (void**)&pVB, D3DLOCK_READONLY));
	for (UINT i = 0; i < m_floorVerts.size(); ++i) {
		pVB[i].mPosition		= m_floorVerts[i];
		pVB[i].mColor = 0xff000100;
	}
	HR(m_pFloorVB->Unlock());

	HR(gD3DDevice->CreateIndexBuffer(sizeof(DWORD) * m_floorIdxs.size(), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pFloorIB, NULL));
	 	
	DWORD* pIDXS = NULL;
	HR(m_pFloorIB->Lock(0, 0, (void**)&pIDXS, D3DLOCK_READONLY));
	for (UINT i = 0; i < m_floorIdxs.size(); ++i)
		pIDXS[i] = m_floorIdxs[i];

	HR(m_pFloorIB->Unlock());

	m_bFloorCreated = true;
}

void DartLevelEditorApp::drawFloor(D3DFILLMODE fillMode)
{
	D3DXMATRIX iden;
	D3DXMatrixIdentity(&iden);

	m_fxs.setWVP(iden * m_camera.viewProj());
	m_fxs.getFx(m_nLevelShaderID).pFx->CommitChanges();
	m_fxs.setMtrlDiffuse(D3DXVECTOR4(0.0f, 0.750f, 0.0f, 1.0f));

	HR(gD3DDevice->SetIndices(m_pFloorIB));
	HR(gD3DDevice->SetFVF(MY_FVF));
	HR(gD3DDevice->SetStreamSource(0, m_pFloorVB, 0, sizeof(VertexCol)));
	HR(m_fxs.getFx(m_nLevelShaderID).pFx->CommitChanges());

	UINT numpasses = 0;
	m_fxs.getFx(m_nLevelShaderID).pFx->Begin(&numpasses, 0);
	for (UINT i = 0; i < numpasses; ++i) {
		m_fxs.getFx(m_nLevelShaderID).pFx->BeginPass(i);
		
		HR(gD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nNumFloorVerts, 0, m_nNumFloorTris));

		m_fxs.getFx(m_nLevelShaderID).pFx->EndPass();
	}
	m_fxs.getFx(m_nLevelShaderID).pFx->End();
}

int DartLevelEditorApp::createCube(FLOAT width, FLOAT height, FLOAT depth)
{
	ID3DXMesh* pMesh = nullptr;
	ID3DXBuffer* pAdj = nullptr;

	HR(D3DXCreateBox(gD3DDevice, width, height, depth, &pMesh, &pAdj));
	if (NULL == pMesh)
		return -1;

// 	DWORD fvf = pMesh->GetFVF();
// 	if (!(fvf & D3DFVF_DIFFUSE)) {
// 		fvf |= D3DFVF_DIFFUSE;
// 		ID3DXMesh* clone = nullptr;
// 
// 		HR(pMesh->CloneMeshFVF(0, fvf, gD3DDevice, &clone));
// 
// 		SAFE_RELEASE(pMesh);
// 		pMesh = clone;
// 	}
// 
// 	VertexCol *pVerts = nullptr;
// 	HR(pMesh->LockVertexBuffer(D3DLOCK_DISCARD, (VOID**)&pVerts));
// 	
// 	for (DWORD i = 0; i < pMesh->GetNumVertices(); ++i){
// 		pVerts[i].mPosition = pVerts[i].mPosition;
// 		pVerts[i].mColor = 0xffffffff;
// 	}
// 	HR(pMesh->UnlockVertexBuffer());

	SAFE_RELEASE(pAdj);
	
	m_meshList.push_back(pMesh);

	return m_meshList.size() - 1;
}


int DartLevelEditorApp::createSphere(float radius, UINT slices, UINT stacks)
{
	ID3DXMesh* pMesh = nullptr;
	ID3DXBuffer* pAdj = nullptr;

	HR(D3DXCreateSphere(gD3DDevice, radius, slices, stacks, &pMesh, &pAdj));

	SAFE_RELEASE(pAdj);

	m_meshList.push_back(pMesh);

	return m_meshList.size() - 1;
}


int DartLevelEditorApp::createTorus(float innerRadius, float outerRadius, UINT sides, UINT rings)
{
	ID3DXMesh* pMesh = nullptr;
	ID3DXBuffer* pAdj = nullptr;

	HR(D3DXCreateTorus(gD3DDevice, innerRadius, outerRadius, sides, rings, &pMesh, &pAdj));
	if (NULL == pMesh)
		return -1;

	SAFE_RELEASE(pAdj);

	m_meshList.push_back(pMesh);

	return m_meshList.size() - 1;
}

void DartLevelEditorApp::saveLevel(CString filename)
{
	wofstream ofs(filename, std::ios::out | std::ios::trunc);

	if (ofs.fail())
		return;
	
	ofs << L"PawnNumTypes:" << EditorPawn::PawnType::PT_End << '\n';
	ofs << L"EnemyType1SpawnLocale:" << EditorPawn::PawnType::PT_EnemyType1SpawnLocale << L'\n';
	ofs << L"EnemyType2SpawnLocale:" << EditorPawn::PawnType::PT_EnemyType2SpawnLocale << L'\n';
	ofs << L"DartSpawnLocale:" << EditorPawn::PawnType::PT_DartSpawnLocale << L'\n';
	ofs << L"BartSpawnLocale:" << EditorPawn::PawnType::PT_BartSpawnLocale << L'\n';
	ofs << L"PointOfLevelLocale:" << EditorPawn::PawnType::PT_PointOfLevelLocale << L'\n';
	ofs << L"RockLocale:" << EditorPawn::PawnType::PT_RockLocale << L'\n';
	ofs << L"Wall:" << EditorPawn::PawnType::PT_Wall << L'\n';
	ofs << L"PickUpHeal:" << EditorPawn::PawnType::PT_Pickup_Heal << L'\n';
	ofs << L"PickUpBonus:" << EditorPawn::PawnType::PT_Pickup_Bonus << '\n';
	ofs << L"PickUpAmmoSeed:" << EditorPawn::PawnType::PT_Pickup_Ammo_Seed << '\n';
	ofs << L"PickUpAmmoFire:" << EditorPawn::PawnType::PT_Pickup_Ammo_Fire << '\n';
	ofs << L"FleePoint:" << EditorPawn::PawnType::PT_FleePoint << '\n';

	ofs << L"PawnCount:" << m_pawnList.size() << L'\n';
	for (UINT i = 0; i < m_pawnList.size(); ++i) {
		switch (m_pawnList[i].getPawnType())
		{
		case EditorPawn::PawnType::PT_EnemyType1SpawnLocale:
		{
			ofs << i << L":" << L"Type:" << m_pawnList[i].getPawnType() << L'\n';
			ofs << i << L":" << L"Solid:" << m_pawnList[i].getSolid() << L'\n';
			ofs << i << L":" << L"Position:" << m_pawnList[i].getPos().x << L"," << m_pawnList[i].getPos().y << L"," << m_pawnList[i].getPos().z << L'\n';
		}
		break;
		case EditorPawn::PawnType::PT_EnemyType2SpawnLocale:
		{

		}
		break;
		case EditorPawn::PawnType::PT_DartSpawnLocale:
		{
			ofs << i << L":" << L"Type:" << m_pawnList[i].getPawnType() << L'\n';
			ofs << i << L":" << L"Solid:" << m_pawnList[i].getSolid() << L'\n';
			ofs << i << L":" << L"Position:" << m_pawnList[i].getPos().x << L"," << m_pawnList[i].getPos().y << L"," << m_pawnList[i].getPos().z << L'\n';
		}
		break;
		case EditorPawn::PawnType::PT_BartSpawnLocale:
		{
			ofs << i << L":" << L"Type:" << m_pawnList[i].getPawnType() << L'\n';
			ofs << i << L":" << L"Solid:" << m_pawnList[i].getSolid() << L'\n';
			ofs << i << L":" << L"Position:" << m_pawnList[i].getPos().x << L"," << m_pawnList[i].getPos().y << L"," << m_pawnList[i].getPos().z << L'\n';
		}
		break;
		case EditorPawn::PawnType::PT_PointOfLevelLocale:
		{

		}
		break;
		case EditorPawn::PawnType::PT_RockLocale:
		{

		}
		break;
		case EditorPawn::PawnType::PT_Wall:
		{
			ofs << i << L":" << L"Type:" << m_pawnList[i].getPawnType() << L'\n';
			ofs << i << L":" << L"Solid:" << m_pawnList[i].getSolid() << L'\n';
			ofs << i << L":" << L"Position:" << m_pawnList[i].getPos().x << L"," << m_pawnList[i].getPos().y << L"," << m_pawnList[i].getPos().z << L'\n';
			ofs << i << L":" << L"Scale:" << m_pawnList[i].getScale().x << L"," << m_pawnList[i].getScale().y << L"," << m_pawnList[i].getScale().z << L'\n';
		}
		break;
		case EditorPawn::PawnType::PT_Pickup_Heal:
		{
			ofs << i << L":" << L"Type:" << m_pawnList[i].getPawnType() << L'\n';
			ofs << i << L":" << L"Solid:" << m_pawnList[i].getSolid() << L'\n';
			ofs << i << L":" << L"Position:" << m_pawnList[i].getPos().x << L"," << m_pawnList[i].getPos().y << L"," << m_pawnList[i].getPos().z << L'\n';
		}
		break;
		case EditorPawn::PawnType::PT_Pickup_Bonus:
		{
			ofs << i << L":" << L"Type:" << m_pawnList[i].getPawnType() << L'\n';
			ofs << i << L":" << L"Solid:" << m_pawnList[i].getSolid() << L'\n';
			ofs << i << L":" << L"Position:" << m_pawnList[i].getPos().x << L"," << m_pawnList[i].getPos().y << L"," << m_pawnList[i].getPos().z << L'\n';
		}
		break;
		case EditorPawn::PawnType::PT_Pickup_Ammo_Seed:
		{
			ofs << i << L":" << L"Type:" << m_pawnList[i].getPawnType() << L'\n';
			ofs << i << L":" << L"Solid:" << m_pawnList[i].getSolid() << L'\n';
			ofs << i << L":" << L"Position:" << m_pawnList[i].getPos().x << L"," << m_pawnList[i].getPos().y << L"," << m_pawnList[i].getPos().z << L'\n';
		}
		break;
		case EditorPawn::PawnType::PT_Pickup_Ammo_Fire:
		{
			ofs << i << L":" << L"Type:" << m_pawnList[i].getPawnType() << L'\n';
			ofs << i << L":" << L"Solid:" << m_pawnList[i].getSolid() << L'\n';
			ofs << i << L":" << L"Position:" << m_pawnList[i].getPos().x << L"," << m_pawnList[i].getPos().y << L"," << m_pawnList[i].getPos().z << L'\n';
		}
		break;
		case EditorPawn::PawnType::PT_FleePoint:
		{
			ofs << i << L":" << L"Type:" << m_pawnList[i].getPawnType() << L'\n';
			ofs << i << L":" << L"Solid:" << m_pawnList[i].getSolid() << L'\n';
			ofs << i << L":" << L"Position:" << m_pawnList[i].getPos().x << L"," << m_pawnList[i].getPos().y << L"," << m_pawnList[i].getPos().z << L'\n';
		}
		break;
		} // end switch		
	} // end for

	ofs << L"FloorCreated:" << m_bFloorCreated << L'\n';
	if (m_bFloorCreated) {
		ofs << L"Width:" << m_dlg.getFloorWidth() << L'\n';
		ofs << L"Length:" << m_dlg.getFloorLength();
	}

	ofs.close();
}

void DartLevelEditorApp::loadLevel(CString filename)
{
	wifstream ifs(filename, std::ios::in);

	if (ifs.fail())
		return;

	if (m_bFloorCreated) {
		SAFE_RELEASE(m_pFloorIB);
		SAFE_RELEASE(m_pFloorVB);
	}

	m_pawnList.clear();
	m_pawnList.resize(0);
	
	wchar_t token[64];
	ifs.getline(token, 64, L':');
	ifs.getline(token, 64);

	int numPawnTypes = _wtoi(token);

	// skip the descriptions and enums
	for (int i = 0; i < numPawnTypes; ++i) {
		ifs.getline(token, 64, L':');
		ifs.getline(token, 64);
	}

	ifs.getline(token, 64, L':');
	ifs.getline(token, 64);

	int numPawns = _wtoi(token);
	int numAntz = 0;
	int numWalls = 0;
	for (int i = 0; i < numPawns; ++i) {
		ifs.getline(token, 64, L':');
		ifs.getline(token, 64, L':');
		ifs.getline(token, 64);

		int type = _wtoi(token);

		ifs.getline(token, 64, L':');
		ifs.getline(token, 64, L':');
		ifs.getline(token, 64);

		int solid = _wtoi(token);

		switch (type)
		{
		case EditorPawn::PawnType::PT_EnemyType1SpawnLocale:
		{
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L',');

			float x, y, z;

			x = (float)_wtof(token);
			ifs.getline(token, 64, L',');
			y = (float)_wtof(token);
			ifs.getline(token, 64);
			z = (float)_wtof(token);
			createAntzSpawn(D3DXVECTOR3(x, y, z));
			CString temp = L"Antz Spawn Point_";
			wchar_t temp1[32];
			_itow_s(numAntz++, temp1, 10);
			temp += temp1;
			m_dlg.insertToPawnList(temp.GetString());
		}
		break;
		case EditorPawn::PawnType::PT_EnemyType2SpawnLocale:
		{

		}
		break;
		case EditorPawn::PawnType::PT_DartSpawnLocale:
		{
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L',');

			float x, y, z;

			x = (float)_wtof(token);
			ifs.getline(token, 64, L',');
			y = (float)_wtof(token);
			ifs.getline(token, 64);
			z = (float)_wtof(token);
			createDartSpawn(D3DXVECTOR3(x, y, z));
			m_dlg.insertToPawnList(L"Dart Spawn Point");
			m_dlg.setDartCreated(true);
		}
		break;
		case EditorPawn::PawnType::PT_BartSpawnLocale:
		{
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L',');

			float x, y, z;

			x = (float)_wtof(token);
			ifs.getline(token, 64, L',');
			y = (float)_wtof(token);
			ifs.getline(token, 64);
			z = (float)_wtof(token);
			createBartSpawn(D3DXVECTOR3(x, y, z));
			m_dlg.insertToPawnList(L"Bart Spawn Point");
			m_dlg.setBartCreated(true);
		}
		break;
		case EditorPawn::PawnType::PT_PointOfLevelLocale:
		{

		}
		break;
		case EditorPawn::PawnType::PT_RockLocale:
		{

		}
		break;
		case EditorPawn::PawnType::PT_Wall:
		{
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L',');

			float x, y, z;
			float dx, dy, dz;

			x = (float)_wtof(token);
			ifs.getline(token, 64, L',');
			y = (float)_wtof(token);
			ifs.getline(token, 64);
			z = (float)_wtof(token);

			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L',');

			dx = (float)_wtof(token);
			ifs.getline(token, 64, L',');
			dy = (float)_wtof(token);
			ifs.getline(token, 64);
			dz = (float)_wtof(token);

			createWall(D3DXVECTOR3(dx, dy, dz), D3DXVECTOR3(x, y, z));

			CString temp = L"Wall_";
			wchar_t temp1[32];
			_itow_s(numWalls++, temp1, 10);
			temp += temp1;
			m_dlg.insertToPawnList(temp.GetString());
		}
		break;
		case EditorPawn::PawnType::PT_Pickup_Heal:
		{
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L',');

			float x, y, z;

			x = (float)_wtof(token);
			ifs.getline(token, 64, L',');
			y = (float)_wtof(token);
			ifs.getline(token, 64);
			z = (float)_wtof(token);

			createPickUp(EditorPawn::PT_Pickup_Heal, D3DXVECTOR3(x, y, z));

			CString temp = L"PickUpHeal_";
			wchar_t temp1[32];
			_itow_s(numWalls++, temp1, 10);
			temp += temp1;
			m_dlg.insertToPawnList(temp.GetString());
		}
		break;
		case EditorPawn::PawnType::PT_Pickup_Bonus:
		{
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L',');

			float x, y, z;

			x = (float)_wtof(token);
			ifs.getline(token, 64, L',');
			y = (float)_wtof(token);
			ifs.getline(token, 64);
			z = (float)_wtof(token);

			createPickUp(EditorPawn::PT_Pickup_Bonus, D3DXVECTOR3(x, y, z));

			CString temp = L"PickUpBonus_";
			wchar_t temp1[32];
			_itow_s(numWalls++, temp1, 10);
			temp += temp1;
			m_dlg.insertToPawnList(temp.GetString());
		}
		break;
		case EditorPawn::PawnType::PT_Pickup_Ammo_Seed:
		{
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L',');

			float x, y, z;

			x = (float)_wtof(token);
			ifs.getline(token, 64, L',');
			y = (float)_wtof(token);
			ifs.getline(token, 64);
			z = (float)_wtof(token);

			createPickUp(EditorPawn::PT_Pickup_Ammo_Seed, D3DXVECTOR3(x, y, z));

			CString temp = L"PickUpAmmoSeed_";
			wchar_t temp1[32];
			_itow_s(numWalls++, temp1, 10);
			temp += temp1;
			m_dlg.insertToPawnList(temp.GetString());
		}
		break;
		case EditorPawn::PawnType::PT_Pickup_Ammo_Fire:
		{
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L',');

			float x, y, z;

			x = (float)_wtof(token);
			ifs.getline(token, 64, L',');
			y = (float)_wtof(token);
			ifs.getline(token, 64);
			z = (float)_wtof(token);

			createPickUp(EditorPawn::PT_Pickup_Ammo_Fire, D3DXVECTOR3(x, y, z));

			CString temp = L"PickUpAmmoFire_";
			wchar_t temp1[32];
			_itow_s(numWalls++, temp1, 10);
			temp += temp1;
			m_dlg.insertToPawnList(temp.GetString());
		}
		break;
		case EditorPawn::PawnType::PT_FleePoint:
		{
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L':');
			ifs.getline(token, 64, L',');

			float x, y, z;

			x = (float)_wtof(token);
			ifs.getline(token, 64, L',');
			y = (float)_wtof(token);
			ifs.getline(token, 64);
			z = (float)_wtof(token);

			createFleePointSpawn(D3DXVECTOR3(x, y, z));

			CString temp = L"FleePoint_";
			wchar_t temp1[32];
			_itow_s(numWalls++, temp1, 10);
			temp += temp1;
			m_dlg.insertToPawnList(temp.GetString());
		}
		break;
		} // end switch
	} // end for

	ifs.getline(token, 64, L':');
	ifs.getline(token, 64);
	int floorCreated = _wtoi(token);
	if (floorCreated) {
		ifs.getline(token, 64, L':');
		ifs.getline(token, 64);
		m_dlg.setFloorWidth(_wtoi(token));
		ifs.getline(token, 64, L':');
		ifs.getline(token, 64);
		m_dlg.setFloorLength(_wtoi(token));
		createFloor();
	}

	m_camera.lookAt(m_camera.pos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));

}

void DartLevelEditorApp::drawPawns()
{
	for (UINT i = 0; i < m_pawnList.size(); ++i) {
// 		if (EditorPawn::PawnType::PT_FleePoint == m_pawnList[i].getPawnType()) {
// 			D3DXMATRIX rotMat;
// 			D3DXMatrixRotationX(&rotMat, D3DXToRadian(-90.0f));
// 			m_pawnList[i].setMatWorld(rotMat);
// 		}

		m_fxs.setWVP(m_pawnList[i].getMatWorld() * m_camera.viewProj());
		m_fxs.setMtrlDiffuse(m_pawnList[i].getDiffuseColor());
		m_fxs.getFx(m_nLevelShaderID).pFx->CommitChanges();

		UINT numpasses = 0;
		m_fxs.getFx(m_nLevelShaderID).pFx->Begin(&numpasses, 0);
		for (UINT j = 0; j < numpasses; ++j) {
			m_fxs.getFx(m_nLevelShaderID).pFx->BeginPass(j);
			m_meshList[m_pawnList[i].getMeshID()]->DrawSubset(0);
	 		m_fxs.getFx(m_nLevelShaderID).pFx->EndPass();
		}
		m_fxs.getFx(m_nLevelShaderID).pFx->End();
	}
}

void DartLevelEditorApp::movePawn(UINT idx, float dt)
{
	if (g_input->keyDown(DIK_LEFT) && (g_input->keyDown(DIK_LSHIFT) || g_input->keyDown(DIK_RSHIFT)))
		m_pawnList[idx].update(dt, D3DXVECTOR3(-200.0f, 0.0f, 0.0f));
	if (g_input->keyDown(DIK_RIGHT) && (g_input->keyDown(DIK_LSHIFT) || g_input->keyDown(DIK_RSHIFT)))
		m_pawnList[idx].update(dt, D3DXVECTOR3(200.0f, 0.0f, 0.0f));
	if (g_input->keyDown(DIK_UP) && (g_input->keyDown(DIK_LSHIFT) || g_input->keyDown(DIK_RSHIFT)))
		m_pawnList[idx].update(dt, D3DXVECTOR3(0.0f, 0.0f, 200.0f));
	if (g_input->keyDown(DIK_DOWN) && (g_input->keyDown(DIK_LSHIFT) || g_input->keyDown(DIK_RSHIFT)))
		m_pawnList[idx].update(dt, D3DXVECTOR3(0.0f, 0.0f, -200.0f));
}

void DartLevelEditorApp::setPawnToMove(UINT id)
{
	m_nMovePawnID = id;
}


void DartLevelEditorApp::deletePawn(UINT id)
{
	vector<EditorPawn>::iterator itr;
	for (itr = m_pawnList.begin(); itr != m_pawnList.end(); ++itr) {
		if (*(itr) == m_pawnList[id]) {
			if (EditorPawn::PawnType::PT_BartSpawnLocale == m_pawnList[id].getPawnType())
				m_dlg.setBartCreated(false);
			if (EditorPawn::PawnType::PT_DartSpawnLocale == m_pawnList[id].getPawnType())
				m_dlg.setDartCreated(false);

			m_pawnList.erase(itr);

			break;
		}
	}	
}


void DartLevelEditorApp::createDartSpawn(D3DXVECTOR3& initPos /*= D3DXVECTOR3(0.0f, 0.0f, 0.0f)*/)
{
	DartPawn dart;

	dart.setMeshID(m_nBoxMeshID);
	dart.setPos(D3DXVECTOR3(initPos));
	dart.update(0.0f);
	m_pawnList.push_back(dart);
}

void DartLevelEditorApp::createBartSpawn(D3DXVECTOR3& initPos /*= D3DXVECTOR3(0.0f, 0.0f, 0.0f)*/)
{
	BartPawn bart;

	bart.setMeshID(m_nBoxMeshID);
	bart.setPos(initPos);
	bart.update(0.0f);
	m_pawnList.push_back(bart);
}

void DartLevelEditorApp::createAntzSpawn(D3DXVECTOR3& initPos /*= D3DXVECTOR3(0.0f, 0.0f, 0.0f)*/)
{
	AntzPawn antz;

	antz.setMeshID(m_nBoxMeshID);
	antz.setPos(initPos);
	antz.update(0.0f);
	m_pawnList.push_back(antz);
}

void DartLevelEditorApp::createWall(D3DXVECTOR3& scale/* = D3DXVECTOR3(1.0f, 1.0f, 1.0f)*/, D3DXVECTOR3& initPos /*= D3DXVECTOR3(0.0f, 0.0f, 0.0f)*/)
{
	WallPawn wall;

	initPos.y += m_dlg.getWallHeigth() >> 1;
	wall.setPos(initPos);
	wall.setMeshID(m_nBoxMeshID);
	wall.setScale(scale);
	wall.update(0.0f);
	m_pawnList.push_back(wall);
}

void DartLevelEditorApp::createPickUp(const EditorPawn::PawnType type, D3DXVECTOR3& initPos /*= D3DXVECTOR3(0.0f, 0.0f, 0.0f)*/)
{
	PickUpPawn pickup(type);

	pickup.setPos(initPos);
	pickup.setSolid(true);
	pickup.setMeshID(m_nSphereMeshID);
	pickup.update(0.0f);
	m_pawnList.push_back(pickup);
}

void DartLevelEditorApp::createFleePointSpawn(D3DXVECTOR3& initPos /*= D3DXVECTOR3(0.0f, 0.0f, 0.0f)*/)
{
	FleePointPawn fleePoint;

	fleePoint.setPos(initPos);
	fleePoint.setMeshID(m_nSphereMeshID);
	fleePoint.setSolid(true);
	fleePoint.update(0.0f);
	m_pawnList.push_back(fleePoint);
}

void DartLevelEditorApp::setPawnLocation(UINT id, D3DXVECTOR3& pos)
{
	m_pawnList[id].setPos(pos);
	m_pawnList[id].update(0.0f);
}

DartLevelEditorApp theApp; 

