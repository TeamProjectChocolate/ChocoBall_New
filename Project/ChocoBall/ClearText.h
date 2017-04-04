#pragma once
#include "GameObject.h"
#include "StageTable.h"

class CClearText :
	public CGameObject
{
public:
	CClearText();
	~CClearText();
	void Initialize()override;
	void Update()override;
	void Draw()override;
	inline bool GetIsEnd()const{
		return m_IsEnd;
	}
	inline void SetStageID(STAGE_ID id) {
		m_StageID = id;
	}
private:
	bool m_IsEnd;
	float t;
	short dir;
	STAGE_ID m_StageID;
};
