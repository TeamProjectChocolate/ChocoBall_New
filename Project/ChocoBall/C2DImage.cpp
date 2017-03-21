#include "stdafx.h"
#include "C2DImage.h"
#include "GraphicsDevice.h"
#include "ImageManager.h"
#include "RenderContext.h"
#include "Camera.h"


void C2DImage::SetImage()
{
	m_pImage = SINSTANCE(CImageManager)->Find2DImage(m_pFileName);
}


void C2DImage::Initialize(){
	m_Now = D3DXVECTOR2(0.0f, 0.0f);
	m_Split = D3DXVECTOR2(1.0f, 1.0f);
	m_luminance = 0.0f;
	m_alpha = 1.0f;
	CModel::Initialize();
}

void C2DImage::Update(const SH_ENGINE::TRANSFORM& transform)
{
	D3DXVECTOR3 Scale;
	Scale.x = transform.scale.x / WINDOW_WIDTH;
	Scale.y = transform.scale.y / WINDOW_HEIGHT;
	D3DXVECTOR3 Trans;
	Trans.x = transform.position.x / WINDOW_WIDTH;
	Trans.y = transform.position.y / WINDOW_HEIGHT;
	Trans.x = -1.0f + Trans.x * 2.0f;
	Trans.y = 1.0f - Trans.y * 2.0f;

	D3DXMATRIX matScale;
	D3DXMATRIX matTrans;
	D3DXMATRIX matRota;
	D3DXMatrixIdentity(&this->m_World);	//ƒ[ƒ‹ƒhs—ñ‰Šú‰»
	D3DXMatrixScaling(&matScale, Scale.x, Scale.y, Scale.z);
	D3DXMatrixRotationQuaternion(&matRota, &transform.angle);
	D3DXMatrixTranslation(&matTrans, Trans.x, Trans.y, 0);
	m_World = matScale * matRota * matTrans;
}
