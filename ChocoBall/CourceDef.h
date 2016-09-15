#pragma once
#include "stdafx.h"
#include "Infomation.h"
#include "StageTable.h"

typedef struct COURCE_BLOCK{
	D3DXVECTOR3 startPosition;
	D3DXVECTOR3 endPosition;
	int blockNo;
	COURCE_BLOCK* nextBlock;
};

class CCourceDef{
public:
	CCourceDef(){ m_courceNum = 0; };
	~CCourceDef(){};
	void Initialize();
	COURCE_BLOCK FindCource(D3DXVECTOR3);
	COURCE_BLOCK FindCource(int);
	D3DXVECTOR3 EndCource(){
		return m_courceEnd;
	}
	int GetCourceMax(){
		return m_courceNum;
	}
	void SetStageID(STAGE_ID id){
		m_StageID = id;
	}
private:
	COURCE_BLOCK m_cource;
	int m_courceNum;
	D3DXVECTOR3 m_courceEnd;
	STAGE_ID m_StageID;
};
