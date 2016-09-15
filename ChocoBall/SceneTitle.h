#pragma once
#include "Scene.h"
#include "TitleBack.h"
#include "TitleSelect.h"
#include "TitleCursor.h"
#include "Number.h"

class CAudio;
class CSceneTitle :
	public CScene
{
public:
	CSceneTitle();
	~CSceneTitle();
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Release();
private:
	CNumber* num;
	CNumber* StageNum;
};
