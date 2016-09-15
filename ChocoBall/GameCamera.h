#pragma once
#include "GameObject.h"
#include "Camera.h"


class CGameCamera :
	public CGameObject
{
public:
	CGameCamera();
	~CGameCamera();
	virtual void Initialize()override;
	virtual void Update()override;
	virtual void Draw()override;
	D3DXMATRIX GetCameraRotation(){
		return m_camera.GetCameraRotation();
	}
	D3DXMATRIX GetProj(){
		return m_camera.GetProj();
	}
	D3DXMATRIX GetView(){
		return m_camera.GetView();
	}
	CCamera* GetCamera(){
		return &m_camera;
	}
	CCamera m_camera;
};

