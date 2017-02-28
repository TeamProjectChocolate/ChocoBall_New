#pragma once
#include "GameObject.h"
class CIcon :
	public CGameObject
{
public:
	CIcon();
	~CIcon();
	void Initialize()override;
	void Update()override;
	void Draw()override;
	inline void SetFileName(LPCSTR name)override{
		strcpy(m_pFileName,name);
	}
};

