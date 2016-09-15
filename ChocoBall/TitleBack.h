#pragma once
#include "GameObject.h"

class CTitleBack :
	public CGameObject
{
public:
	CTitleBack();
	~CTitleBack();
	void Initialize()override;
	void Update()override;
	void Draw()override;
};