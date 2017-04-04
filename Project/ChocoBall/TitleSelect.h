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
	inline void SetsFlashing(bool flg) {
		m_IsFlashing = flg;
	}
	inline void SetFlashingSpeed(float s) {
		m_FlashingSpeed = s;
	}
private:
	CTitleCursor* m_cursor = nullptr;	//カーソル。
	//CDirectInput* m_Input = nullptr;
	CInterface*	m_pInput = nullptr;
	float t;
	short dir;
	bool m_IsFlashing = false;
	float m_FlashingSpeed;	// 点滅速度。
};
