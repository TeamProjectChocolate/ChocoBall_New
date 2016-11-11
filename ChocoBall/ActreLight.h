#pragma once
#include "Light.h"
class CActreLight :
	public CLight
{
public:
	CActreLight();
	~CActreLight();
	void Initialize();
	void Update(D3DXMATRIX);
	void SetOrigDiffuseLightDirection(int lightNum,D3DXVECTOR3 dir){
		CH_ASSERT(lightNum < NUM_DIFFUSE_LIGHT);
		D3DXVec3Normalize(&dir, &dir);
		m_OrigDiffuseLightDirection[lightNum] = dir;
	}
private:
	D3DXVECTOR3 m_OrigDiffuseLightDirection[NUM_DIFFUSE_LIGHT];	// 座標変換前のライトの向き。
};

