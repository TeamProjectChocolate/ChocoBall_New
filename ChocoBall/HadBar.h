#pragma once
#include "GameObject.h"
class CHadBar :
	public CGameObject
{
public:
	CHadBar();
	~CHadBar();
	void Initialize()override;
	void Update()override;
	void Draw()override;
};

