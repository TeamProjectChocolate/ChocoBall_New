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
	CTitleCursor* m_cursor = nullptr;	//�J�[�\���B
	//CDirectInput* m_Input = nullptr;
	CInterface*	m_pInput = nullptr;
	float t;
	short dir;
};
