#pragma once

#include "D3DUtils.h"

class Camera
{
public:
	enum ClipPlanes {
		NEAR_CLIP,
		FAR_CLIP,
		LEFT_CLIP,
		RIGHT_CLIP,
		TOP_CLIP,
		BOTTOM_CLIP,
		END_CLIPS
	};
	const D3DXMATRIX& view() const;
	const D3DXMATRIX& proj() const;
	const D3DXMATRIX& viewProj() const;

	const D3DXVECTOR3& right() const;
	const D3DXVECTOR3& up() const;
	const D3DXVECTOR3& look() const;

	D3DXVECTOR3& pos();

	void lookAt(D3DXVECTOR3& pos, D3DXVECTOR3& target, D3DXVECTOR3& up);
	void setLens(float fov, float aspect, float nearZ, float farZ);
	void setLinearSpeed(float s);
	void setRotSpeed(float s);

	void update(float dt, float offsetHeight);
	
	Camera();
	~Camera() {}

protected:
	D3DXMATRIX	m_view,
						m_proj,
						m_viewProj;

	D3DXPLANE mFrustumPlanes[6]; 

	D3DXVECTOR3	m_posW,
							m_rightW,
							m_upW,
							m_lookW;

	float	m_fRotSpeed;
	float	m_fLinearSpeed;

	void buildView();
	void buildFrustumPlanes();
};

//extern Camera* g_camera;