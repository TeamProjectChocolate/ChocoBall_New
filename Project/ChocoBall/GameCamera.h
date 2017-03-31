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
	virtual void Draw_EM(CCamera*)override;
	D3DXMATRIX GetCameraRotation(){
		return m_camera.GetRotation();
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
	inline const D3DXVECTOR3& GetPos()override {
		return m_camera.GetPos();
	}
	inline void SetPos(const D3DXVECTOR3& pos)override{
		m_camera.SetPos(pos);
	}
	inline void SetTarget(const D3DXVECTOR3& target) {
		m_camera.SetTarget(target);
	}
protected:
	CCamera m_camera;
};

