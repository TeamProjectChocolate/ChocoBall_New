#pragma once
#include "GameObject.h"
#include "Audio.h"

class CGameAudio :
	public CGameObject
{
public:
	CGameAudio();
	~CGameAudio();
	void Initialize()override;
	void Initialize(const char* EngineName, const char* WBName, const char* SBName);
	void Update()override;
	void Draw()override;
	inline void Play(const char* cue,bool flg,void* p) {
		m_pAudio->PlayCue(cue,flg,p);
	}
	inline void Stop(const char* cue, bool flg, void* p) {
		m_pAudio->StopCue(cue, flg, p);
	}
private:
	unique_ptr<CAudio> m_pAudio;
};

