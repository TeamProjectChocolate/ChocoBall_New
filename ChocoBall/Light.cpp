#include "stdafx.h"
#include "Light.h"


void CLight::SetLight(LPD3DXEFFECT pEffect){
	//���C�g
	pEffect->SetValue(
		"g_light",
		this,
		sizeof(CLight)
		);
}
