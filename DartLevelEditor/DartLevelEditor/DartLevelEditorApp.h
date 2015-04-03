#pragma once
#include <vector>
#include "D3DApp.h"
#include "DartDlg.h"
#include "Camera.h"
#include "FxManager.h"
#include "EditorPawn.h"

class DartLevelEditorApp : public CWinApp, public D3DApp
{
public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	virtual int ExitInstance();
	virtual void drawScene();
	virtual void updateScene(float dt);

	void createFloor();
	void drawFloor(D3DFILLMODE fillMode);
	void drawPawns();
	int createCube(FLOAT width, FLOAT height, FLOAT depth);
	int createSphere(float radius, UINT slices, UINT stacks);
	int createTorus(float innerRadius, float outerRadius, UINT sides, UINT rings);
	void saveLevel(CString filename);
	void loadLevel(CString filename);
	void createWall(D3DXVECTOR3& scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3& initPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void createDartSpawn(D3DXVECTOR3& initPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void createBartSpawn(D3DXVECTOR3& initPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void createGoalSpawn(D3DXVECTOR3& initPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void createEnemySpawn(const EditorPawn::PawnType type, D3DXVECTOR3& initPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void createFleePointSpawn(D3DXVECTOR3& initPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void createPickUp(const EditorPawn::PawnType type, D3DXVECTOR3& initPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void movePawn(UINT idx, float dt);
	void setPawnToMove(UINT id);
	void setPawnLocation(UINT id, D3DXVECTOR3& pos);
	void deletePawn(UINT id);

	DartLevelEditorApp();
	~DartLevelEditorApp();

protected:
	std::vector<ID3DXMesh*>		m_meshList;
	std::vector<D3DXVECTOR3>		m_floorVerts;
	std::vector<DWORD>				m_floorIdxs;
	std::vector<EditorPawn>			m_pawnList;

	FxManager	m_fxs;
	Camera	m_camera;
	DartDlg	m_dlg;

	D3DXMATRIX		m_matWorld;

	IDirect3DVertexBuffer9*			m_pFloorVB;
	IDirect3DIndexBuffer9*			m_pFloorIB;

	HWND	m_hD3DWnd;

	float		m_fAspect;
	float		m_fFOV;
	float		m_fFarZ;
	float		m_fNearZ;

	int			m_nLevelShaderID;
	int			m_nBoxMeshID;
	int			m_nSphereMeshID;
	int			m_nTorusMeshID;
	int			m_nMovePawnID;

	UINT		m_nNumFloorVerts;
	UINT		m_nNumFloorTris;

	bool		m_bFloorCreated;
};

