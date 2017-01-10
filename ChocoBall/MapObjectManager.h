#pragma once
#include "stdafx.h"
#include "MapTable.h"
#include "GameObject.h"

// �}�b�v�ɔz�u����I�u�W�F�N�g�Ǘ��N���X�B
// �����݂̓{�X��p�}�b�v���\�z���邱�Ƃɂ̂ݎg�p���Ă���B

class CMapObjectManager : public CGameObject{
public:
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void DrawShadow(CCamera*)override;
	void Draw_EM(CCamera*)override;
private:
	vector<CGameObject*> m_MapObjects;
};
