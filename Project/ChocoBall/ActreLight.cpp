#include "stdafx.h"
#include "ActreLight.h"


CActreLight::CActreLight()
{
}


CActreLight::~CActreLight()
{
}

void CActreLight::Update(D3DXMATRIX mat) {
	mat.m[3][0] = 0.0f;
	mat.m[3][1] = 0.0f;
	mat.m[3][2] = 0.0f;
	mat.m[3][3] = 1.0f;
	D3DXVECTOR4 dir[NUM_DIFFUSE_LIGHT];
	// 受け取ったワールド行列でライトの向きを更新。
	for (short idx = 0; idx < NUM_DIFFUSE_LIGHT; idx++) {
		D3DXVec3Transform(&dir[idx], &m_OrigDiffuseLightDirection[idx], &mat);
		this->SetDiffuseLightDirection(idx, static_cast<D3DXVECTOR3>(dir[idx]));
	}
}