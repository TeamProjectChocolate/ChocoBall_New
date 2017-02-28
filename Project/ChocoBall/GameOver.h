#pragma once
#include "GameObject.h"
class CGameOver :
	public CGameObject
{
public:
	CGameOver();
	~CGameOver();
	void Initialize()override;
	void Update()override;
	void Draw()override;
	bool GetIsEnd(){
		return m_IsEnd;
	}
private:
	bool m_IsEnd;
};