#pragma once
#include "GameObject.h"
#include "DirectInput.h"

class CResultCursor;

class CResultSelect :
	public CGameObject
{
public:
	CResultSelect();
	~CResultSelect();
	void Initialize()override;
	void Update()override;
	void Draw()override;
private:
	CResultCursor* m_resultcursor;	//カーソル。
	CDirectInput* m_Input = nullptr;
	float t;
	short dir;
};