#pragma once
#include "GameObject.h"
#include "DirectInput.h"

class CTitleCursor;

class CTitleSelect :
	public CGameObject
{
public:
	CTitleSelect();
	~CTitleSelect();
	void Initialize()override;
	void Update()override;
	void Draw()override;
private:
	CTitleCursor* m_cursor;	//カーソル。
	//CDirectInput* m_Input = nullptr;
	CInterface*	m_pInput;
	float t;
	short dir;
};
