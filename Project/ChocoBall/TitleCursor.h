#pragma once
#include "GameObject.h"
#include "DirectInput.h"
#include "Audio.h"

class CTitleCursor :
	public CGameObject
{
public:
	CTitleCursor();
	~CTitleCursor();
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Move();
	void Release();
	void SetAudio(CAudio* audio){
		m_pAudio = audio;
	}
private:
	CAudio* m_pAudio = nullptr;
	CInterface*	m_pInput = nullptr;
	short dir;
	bool isup;
	bool isdown;
};