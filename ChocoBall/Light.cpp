#include "stdafx.h"
#include "Light.h"


void CLight::SetLight(LPD3DXEFFECT pEffect){
	// ディフューズライトの向きをシェーダーに転送
	pEffect->SetVectorArray("diffuseLightDirection", m_diffuseLightDirection, NUM_DIFFUSE_LIGHT);
	// ディフューズライトのカラーをシェーダーに転送
	pEffect->SetVectorArray("diffuseLightColor", m_diffuseLightColor, NUM_DIFFUSE_LIGHT);
	// 環境光をシェーダーに転送
	pEffect->SetVector("ambientLight", &m_ambientLight);
}
