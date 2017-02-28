#include "stdafx.h"
#include "Light.h"


void CLight::SetLight(LPD3DXEFFECT pEffect){
	//ƒ‰ƒCƒg
	pEffect->SetValue(
		"g_light",
		this,
		sizeof(CLight)
		);
}
