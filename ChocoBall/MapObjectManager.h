#pragma once
#include "stdafx.h"
#include "MapTable.h"
#include "GameObject.h"

// マップに配置するオブジェクト管理クラス。
// ※現在はボス戦用マップを構築することにのみ使用している。

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
