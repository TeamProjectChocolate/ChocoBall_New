#include "stdafx.h"
#include "CourceDef.h"
#include "StageTable.h"

void CCourceDef::Initialize(){
	m_courceNum = 0;
	D3DXVECTOR3* CourceDef = CourceDefineArray[m_StageID];
	int arraySize =CourceDefineNumArray[m_StageID];
	COURCE_BLOCK* itr = &m_cource;
	for(int num = 0; num + 1 < arraySize; num++){
		itr->startPosition = CourceDef[num];
		itr->endPosition = CourceDef[num + 1];
		itr->startPosition.x *= -1.0f;
		itr->startPosition.z *= -1.0f;
		itr->endPosition.x *= -1.0f;
		itr->endPosition.z *= -1.0f;
		itr->blockNo = num;
		COURCE_BLOCK* node = new COURCE_BLOCK;
		itr->nextBlock = node;
		m_courceNum++;
		if (num + 2 >= arraySize){
			m_courceEnd = itr->endPosition;
			itr->nextBlock = nullptr;
		}
		else{
			itr = node;
		}
	}
}

COURCE_BLOCK CCourceDef::FindCource(D3DXVECTOR3 pos){
    float Min = FLT_MAX;
	COURCE_BLOCK* ret = nullptr;
	COURCE_BLOCK* itr = &m_cource;
	for (int num = 0; num < m_courceNum; num++){
		D3DXVECTOR3 endvector = itr->endPosition - itr->startPosition;
		D3DXVECTOR3 Playervector = pos - itr->startPosition;
		D3DXVECTOR3 Dir;
		float Distance = D3DXVec3Length(&endvector);
		D3DXVec3Normalize(&Dir, &endvector);
		float PlayerDist = D3DXVec3Dot(&Dir, &Playervector);
		if (PlayerDist <= Distance + 5.0f && PlayerDist > -5.0f){
			D3DXVECTOR3 VecToVecDist/*ベクトルとベクトルの距離*/ = (Dir * PlayerDist) - Playervector;
			if (D3DXVec3Length(&VecToVecDist) < Min){
				ret = itr;
				Min = D3DXVec3Length(&VecToVecDist);
			}
		}
		itr = itr->nextBlock;
	}
	if (ret == nullptr){
		COURCE_BLOCK ret2;
		ret2.blockNo = -1;
		//abort();
		return ret2;
	}
	return *ret;
}

COURCE_BLOCK CCourceDef::FindCource(int num){
	if (num < 0){
		COURCE_BLOCK block;
		block.blockNo = -1;
		return block;
	}
	COURCE_BLOCK* itr = &m_cource;
	for (int idx = 0; idx < m_courceNum; idx++){
		if (itr->blockNo == num){
			return *itr;
		}
		itr = itr->nextBlock;
	}
	return *itr;
}

