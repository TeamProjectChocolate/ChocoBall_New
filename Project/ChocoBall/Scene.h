#pragma once

#include "DirectInput.h"

class CAudio;

class CScene
{
public:
	CScene(){
		m_ManagerNewFlg = false;
		m_pAudio = nullptr;
		m_pInput = nullptr;
	};
	virtual ~CScene(){};
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Release(){};
	inline void ActiveManagerNewFlg(){
		m_ManagerNewFlg = true;
	}
	void SetAudio(CAudio* audio){
		m_pAudio = audio;
	}
protected:
	CAudio* m_pAudio = nullptr;
	CInterface* m_pInput = nullptr;
private:
	bool m_ManagerNewFlg;	// ObjectManagerƒNƒ‰ƒX‚Ånew‚³‚ê‚½‚à‚Ì‚©”»’è‚·‚é•Ï”
};

