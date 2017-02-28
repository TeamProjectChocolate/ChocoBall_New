#pragma once
#include "GameObject.h"

class CResultBack :
	public CGameObject
{
public:
	CResultBack();
	~CResultBack();
	void Initialize()override;
	void Update()override;
	void Draw()override;
};