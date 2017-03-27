#pragma once
#include "stdafx.h"


enum GimmickType{
	GimmickType_Chocoball,
	GimmickType_Wall,
	GimmickType_FallFloor,
	GimmickType_MoveFloor,
	GimmickType_UpFloor,
	GimmickType_FireJet,
	GimmickType_SmokeJet,
	GimmickType_DivisionWall,
	GimmickType_BGM_Changer,	// ボス戦以外で使うな。
	GimmickType_None = 999
};

enum EnemyType{
	EnemyType_LR,
	EnemyType_FB,
	EnemyType_JUMP,
	EnemyType_BULLET,
	EnemyType_Boss,
	EnemyType_None = 999
};
struct SEnemyAndGimmickInfo{
	D3DXVECTOR3 pos;
	D3DXQUATERNION rot;
	D3DXVECTOR3 scale;
	D3DXVECTOR3 DirectionZ;
	int enemyType;
	int gimmickType;
	float MaxMove;
};
struct SCollisionInfo {
	D3DXVECTOR3 pos;
	D3DXQUATERNION rotation;
	D3DXVECTOR3 scale;
};

struct MapObject
{
	const char* FileName;
	D3DXVECTOR3 Position;
	D3DXQUATERNION Rotation;
	D3DXVECTOR3 Scale;
	int RType;	// 屈折率。
	float Alpha;
};

typedef struct BOSS_Cource_Block_ByUnity {
	D3DXVECTOR3 startPosition;
	D3DXVECTOR3 endPosition;
	D3DXVECTOR3 Range;	// コース範囲。
	D3DXVECTOR3 RangePos;	// コース範囲の中心点。
	D3DXQUATERNION Rotation;	// コースの範囲の回転。
	D3DXVECTOR3 ActionTargetDir;		// ボスがステートに従ってアクションする際の向き。
	D3DXVECTOR3 ActionPos;		// ボスがステートに従ってアクションする際の位置。
	int BlockType;	// このコースの属性(この値に従ってボスのステートが変化する)。
	bool IsEnd;	// 最後のコースか。
};


// フィールドで使用するXファイル名。
// ※上から順にステージ1,2,3……
static MapObject MapObjects_1[] = {
#include "MapChipOutputByUnityStage1.h"
};
static MapObject MapObjects_2[] = {
#include "MapChipOutputByUnityStage2.h"
};
static MapObject MapObjects_3[] = {
#include "MapChipOutputByUnityStage3.h"
};
#ifdef TEISYUTU_YOU
#else
static MapObject MapObjects_4[] = {
#include "MapChipOutputByUnityStage4.h"
};
static MapObject MapObjects_5[] = {
#include "MapChipOutputByUnityStage5.h"
};
static MapObject MapObjects_6[] = {
#include "MapChipOutputByUnityStage6.h"
};
static MapObject MapObjects_Final[] = {
#include "MapChipOutputByUnityStageFinal.h"
};
#endif
static MapObject MapObjects_Boss[] = {
#include "MapChipOutputByUnityStageBoss.h"
};

static MapObject* MapObjectsPointerArray[] = {
	MapObjects_1,
	MapObjects_2,
	MapObjects_3,
#ifdef TEISYUTU_YOU
#else
	MapObjects_4,
	MapObjects_5,
	MapObjects_6,
	MapObjects_Final,
#endif
	MapObjects_Boss,
};

static int MapObjectsMaxArray[] = {
	ARRAYSIZE(MapObjects_1),
	ARRAYSIZE(MapObjects_2),
	ARRAYSIZE(MapObjects_3),
#ifdef TEISYUTU_YOU
#else
	ARRAYSIZE(MapObjects_4),
	ARRAYSIZE(MapObjects_5),
	ARRAYSIZE(MapObjects_6),
	ARRAYSIZE(MapObjects_Final),
#endif
	ARRAYSIZE(MapObjects_Boss),
};

// フィールドで使用するBGM
static LPCSTR Stage_BGM[] = {
	"joban_Stage",
	"joban_Stage",
	"tyuban_Stage",
#ifdef TEISYUTU_YOU
#else
	"tyuban_Stage",
	"lastStage",
	"lastStage",
	"Modern",
#endif
};

// ボス戦用のBGM。
static LPCSTR Boss_BGM[] = {
	"BossStage",
	"BossBattle",
};

enum STAGE_ID{
	FIRST = 0,
	SECOUND,
	THARD,
#ifdef TEISYUTU_YOU
#else
	FOURTH,
	FIFTH,
	SIX,
	FINAL,
#endif
	BOSS,
	MAX,
	STAGE_NONE = 999
};


// ステージ1のコース定義
static D3DXVECTOR3 CourceDefine_1[] = {
#include "CourceDefStage1.h"
};
static D3DXVECTOR3 CourceDefine_2[] = {
#include "CourceDefStage2.h"
};
static D3DXVECTOR3 CourceDefine_3[] = {
#include "CourceDefStage3.h"
};
#ifdef TEISYUTU_YOU
#else
static D3DXVECTOR3 CourceDefine_4[] = {
#include "CourceDefStage4.h"
};
static D3DXVECTOR3 CourceDefine_5[] = {
#include "CourceDefStage5.h"
};
static D3DXVECTOR3 CourceDefine_6[] = {
#include "CourceDefStage6.h"
};
static D3DXVECTOR3 CourceDefine_Final[] = {
#include "CourceDefStageFinal.h"
};
#endif
static D3DXVECTOR3 CourceDefine_Boss[] = {
#include "CourceDefStageBoss.h"
};


static D3DXVECTOR3* CourceDefineArray[] = {
	CourceDefine_1,
	CourceDefine_2,
	CourceDefine_3,
#ifdef TEISYUTU_YOU
#else
	CourceDefine_4,
	CourceDefine_5,
	CourceDefine_6,
	CourceDefine_Final,
#endif
	CourceDefine_Boss,
};

static int CourceDefineNumArray[] = {
	ARRAYSIZE(CourceDefine_1),
	ARRAYSIZE(CourceDefine_2),
	ARRAYSIZE(CourceDefine_3),
#ifdef TEISYUTU_YOU
#else
	ARRAYSIZE(CourceDefine_4),
	ARRAYSIZE(CourceDefine_5),
	ARRAYSIZE(CourceDefine_6),
	ARRAYSIZE(CourceDefine_Final),
#endif
	ARRAYSIZE(CourceDefine_Boss),
};

// コース定義(ボス用)。
static BOSS_Cource_Block_ByUnity BossCourceDefine_Boss[] = {
#include "BossCourceDefStageBoss.h"
};
static int BossCourceDefineNumArray[] = {
	ARRAYSIZE(BossCourceDefine_Boss),
};

// ステージごとのプレイヤーのポジション情報を格納
static SCollisionInfo PlayerTransformArray[] = {
#include "PlayerPositionInfoStage1.h"
#include "PlayerPositionInfoStage2.h"
#include "PlayerPositionInfoStage3.h"
#ifdef TEISYUTU_YOU
#else
#include "PlayerPositionInfoStage4.h"
#include "PlayerPositionInfoStage5.h"
#include "PlayerPositionInfoStage6.h"
#include "PlayerPositionInfoStageFinal.h"
#endif
#include "PlayerPositionInfoStageBoss.h"
};

static D3DXVECTOR3 EM_CameraPosArray[] = {
#include "OutputMapCameraStage1.h"
#include "OutputMapCameraStage2.h"
#include "OutputMapCameraStage3.h"
#ifdef TEISYUTU_YOU
#else
#include "OutputMapCameraStage4.h"
#include "OutputMapCameraStage5.h"
#include "OutputMapCameraStage6.h"
#include "OutputMapCameraStageFinal.h"
#endif
#include "OutputMapCameraStageBoss.h"
};

// ステージ1で使用するギミックやエネミーのポジションなどを設定した配列
// ※ステージを追加したら同じような配列を新しく作って、追加したステージの出力ヘッダーを読み込んでください
static SEnemyAndGimmickInfo infoTable_1[] = {
#include "EnemyGimmickInfoStage1.h"
};
static SEnemyAndGimmickInfo infoTable_2[] = {
#include "EnemyGimmickInfoStage2.h"
};
static SEnemyAndGimmickInfo infoTable_3[] = {
#include "EnemyGimmickInfoStage3.h"
};
#ifdef TEISYUTU_YOU
#else
static SEnemyAndGimmickInfo infoTable_4[] = {
#include "EnemyGimmickInfoStage4.h"
};
static SEnemyAndGimmickInfo infoTable_5[] = {
#include "EnemyGimmickInfoStage5.h"
};
static SEnemyAndGimmickInfo infoTable_6[] = {
#include "EnemyGimmickInfoStage6.h"
};
static SEnemyAndGimmickInfo infoTable_Final[] = {
#include "EnemyGimmickInfoStageFinal.h"
};
#endif
static SEnemyAndGimmickInfo infoTable_Boss[] = {
#include "EnemyGimmickInfoStageBoss.h"
};


// ステージ1で使用するギミックやエネミーのポジションなどを設定した配列のポインタを格納する配列
// ※ステージを追加したら下のような書き方で配列に要素を追加してください
static SEnemyAndGimmickInfo* infoTableArray[] = {
	infoTable_1,
	infoTable_2,
	infoTable_3,
#ifdef TEISYUTU_YOU
#else
	infoTable_4,
	infoTable_5,
	infoTable_6,
	infoTable_Final,
#endif
	infoTable_Boss,
};

// ステージ1で使用するギミックやエネミーのポジションなどを設定した配列の要素数を格納する配列
// ※ステージを追加したら下のような書き方で配列に要素を追加してください
static int InfoTableSizeArray[] = {
	ARRAYSIZE(infoTable_1),
	ARRAYSIZE(infoTable_2),
	ARRAYSIZE(infoTable_3),
#ifdef TEISYUTU_YOU
#else
	ARRAYSIZE(infoTable_4),
	ARRAYSIZE(infoTable_5),
	ARRAYSIZE(infoTable_6),
	ARRAYSIZE(infoTable_Final),
#endif
	ARRAYSIZE(infoTable_Boss),
};


// ステージ1で使用するギミック発動用当たり判定
// ※ステージを追加したら同じような配列を新しく作って、追加したステージの出力ヘッダーを読み込んでください
static SCollisionInfo GimmickTriggerInfoTable_1[] = {
#include "GimmickTriggerInfoStage1.h"
};
static SCollisionInfo GimmickTriggerInfoTable_2[] = {
#include "GimmickTriggerInfoStage2.h"
};
static SCollisionInfo GimmickTriggerInfoTable_3[] = {
#include "GimmickTriggerInfoStage3.h"
};
#ifdef TEISYUTU_YOU
#else
static SCollisionInfo GimmickTriggerInfoTable_4[] = {
#include "GimmickTriggerInfoStage4.h"
};
static SCollisionInfo GimmickTriggerInfoTable_5[] = {
#include "GimmickTriggerInfoStage5.h"
};
static SCollisionInfo GimmickTriggerInfoTable_6[] = {
#include "GimmickTriggerInfoStage6.h"
};
static SCollisionInfo GimmickTriggerInfoTable_Final[] = {
#include "GimmickTriggerInfoStageFinal.h"
};
#endif
static SCollisionInfo GimmickTriggerInfoTable_Boss[] = {
#include "GimmickTriggerInfoStageBoss.h"
};


// ステージ1で使用するギミック発動用当たり判定を設定した配列のポインタを格納する配列
// ※ステージを追加したら下のような書き方で配列に要素を追加してください
static SCollisionInfo* GimmickinfoTableArray[] = {
	GimmickTriggerInfoTable_1,
	GimmickTriggerInfoTable_2,
	GimmickTriggerInfoTable_3,
#ifdef TEISYUTU_YOU
#else
	GimmickTriggerInfoTable_4,
	GimmickTriggerInfoTable_5,
	GimmickTriggerInfoTable_6,
	GimmickTriggerInfoTable_Final,
#endif
	GimmickTriggerInfoTable_Boss,
};


// ステージ1で使用するギミック発動用当たり判定を設定した配列の要素数を格納する配列
// ※ステージを追加したら下のような書き方で配列に要素を追加してください
static int GimmickInfoTableSizeArray[] = {
	ARRAYSIZE(GimmickTriggerInfoTable_1),
	ARRAYSIZE(GimmickTriggerInfoTable_2),
	ARRAYSIZE(GimmickTriggerInfoTable_3),
#ifdef TEISYUTU_YOU
#else
	ARRAYSIZE(GimmickTriggerInfoTable_4),
	ARRAYSIZE(GimmickTriggerInfoTable_5),
	ARRAYSIZE(GimmickTriggerInfoTable_6),
	ARRAYSIZE(GimmickTriggerInfoTable_Final),
#endif
	ARRAYSIZE(GimmickTriggerInfoTable_Boss),
};

// ステージ1のフィールドの当たり判定
// ※ステージを追加したら同じような配列を新しく作って、追加したステージの出力ヘッダーを読み込んでください
static SCollisionInfo collisionInfoTable_1[] = {
#include "collisionInfoStage1.h"
};
static SCollisionInfo collisionInfoTable_2[] = {
#include "collisionInfoStage2.h"
};
static SCollisionInfo collisionInfoTable_3[] = {
#include "collisionInfoStage3.h"
};
#ifdef TEISYUTU_YOU
#else
static SCollisionInfo collisionInfoTable_4[] = {
#include "collisionInfoStage4.h"
};
static SCollisionInfo collisionInfoTable_5[] = {
#include "collisionInfoStage5.h"
};
static SCollisionInfo collisionInfoTable_6[] = {
#include "collisionInfoStage6.h"
};
static SCollisionInfo collisionInfoTable_Final[] = {
#include "collisionInfoStageFinal.h"
};
#endif
static SCollisionInfo collisionInfoTable_Boss[] = {
#include "collisionInfoStageBoss.h"
};



// ステージ1のフィールドの当たり判定を設定した配列のポインタを格納する配列
// ※ステージを追加したら下のような書き方で配列に要素を追加してください
static SCollisionInfo* collisionInfoTableArray[] = {
	collisionInfoTable_1,
	collisionInfoTable_2,
	collisionInfoTable_3,
#ifdef TEISYUTU_YOU
#else
	collisionInfoTable_4,
	collisionInfoTable_5,
	collisionInfoTable_6,
	collisionInfoTable_Final,
#endif
	collisionInfoTable_Boss,
};

// ステージ1でのフィールドの当たり判定を設定した配列の要素数を格納する配列
// ※ステージを追加したら下のような書き方で配列に要素を追加してください
static int collisionInfoTableSizeArray[] = {
	ARRAYSIZE(collisionInfoTable_1),
	ARRAYSIZE(collisionInfoTable_2),
	ARRAYSIZE(collisionInfoTable_3),
#ifdef TEISYUTU_YOU
#else
	ARRAYSIZE(collisionInfoTable_4),
	ARRAYSIZE(collisionInfoTable_5),
	ARRAYSIZE(collisionInfoTable_6),
	ARRAYSIZE(collisionInfoTable_Final),
#endif
	ARRAYSIZE(collisionInfoTable_Boss),
};

static D3DXVECTOR4 LightColorTable_1[] = {
	D3DXVECTOR4(0.75f, 0.75f, 0.75f, 1.0f) ,
	D3DXVECTOR4(0.75f, 0.75f, 0.75f, 1.0f),
	D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f),
	D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f)
};

static D3DXVECTOR4 LightColorTable_2[] = {
	D3DXVECTOR4(0.75f, 0.75f, 0.75f, 1.0f) ,
	D3DXVECTOR4(0.75f, 0.75f, 0.75f, 1.0f),
	D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f),
	D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f)
};

static D3DXVECTOR4 LightColorTable_3[] = {
	D3DXVECTOR4(0.75f, 0.75f, 0.75f, 1.0f) ,
	D3DXVECTOR4(0.75f, 0.75f, 0.75f, 1.0f),
	D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f),
	D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f)
};

#ifdef TEISYUTU_YOU
#else
static D3DXVECTOR4 LightColorTable_4[] = {
	D3DXVECTOR4(0.75f, 0.75f, 0.75f, 1.0f) ,
	D3DXVECTOR4(0.75f, 0.75f, 0.75f, 1.0f),
	D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f),
	D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f)
};

static D3DXVECTOR4 LightColorTable_5[] = {
	D3DXVECTOR4(0.75f, 0.75f, 0.75f, 1.0f) ,
	D3DXVECTOR4(0.75f, 0.75f, 0.75f, 1.0f),
	D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f),
	D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f)
};

static D3DXVECTOR4 LightColorTable_6[] = {
	D3DXVECTOR4(0.75f, 0.75f, 0.75f, 1.0f) ,
	D3DXVECTOR4(0.75f, 0.75f, 0.75f, 1.0f),
	D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f),
	D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f)
};

static D3DXVECTOR4 LightColorTable_Final[] = {
	D3DXVECTOR4(0.75f, 0.75f, 0.75f, 1.0f) ,
	D3DXVECTOR4(0.75f, 0.75f, 0.75f, 1.0f),
	D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f),
	D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f)
};
#endif
static D3DXVECTOR4 LightColorTable_Boss[] = {
	D3DXVECTOR4(0.75f, 0.75f, 0.75f, 1.0f) ,
	D3DXVECTOR4(0.75f, 0.75f, 0.75f, 1.0f),
	D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f),
	D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f)
};


static D3DXVECTOR4* LightColorTableArray[] = {
	LightColorTable_1,
	LightColorTable_2,
	LightColorTable_3,
#ifdef TEISYUTU_YOU
#else
	LightColorTable_4,
	LightColorTable_5,
	LightColorTable_6,
	LightColorTable_Final,
#endif
	LightColorTable_Boss,
};


static D3DXVECTOR3 LightDirectionTable_1[] = {
	D3DXVECTOR3(0.5f, -0.5f, 0.0f),
	D3DXVECTOR3(-0.5f, -0.5f, 0.0f),
	D3DXVECTOR3(0.5f, 0.5f, 0.0f),
	D3DXVECTOR3(-0.5f, 0.5f, 0.0f)
};

static D3DXVECTOR3 LightDirectionTable_2[] = {
	D3DXVECTOR3(0.5f, -0.5f, 0.0f),
	D3DXVECTOR3(-0.5f, -0.5f, 0.0f),
	D3DXVECTOR3(0.5f, 0.5f, 0.0f),
	D3DXVECTOR3(-0.5f, 0.5f, 0.0f)
};

static D3DXVECTOR3 LightDirectionTable_3[] = {
	D3DXVECTOR3(0.5f, -0.5f, 0.0f),
	D3DXVECTOR3(-0.5f, -0.5f, 0.0f),
	D3DXVECTOR3(0.5f, 0.5f, 0.0f),
	D3DXVECTOR3(-0.5f, 0.5f, 0.0f)
};

#ifdef TEISYUTU_YOU
#else
static D3DXVECTOR3 LightDirectionTable_4[] = {
	D3DXVECTOR3(0.5f, -0.5f, 0.0f),
	D3DXVECTOR3(-0.5f, -0.5f, 0.0f),
	D3DXVECTOR3(0.5f, 0.5f, 0.0f),
	D3DXVECTOR3(-0.5f, 0.5f, 0.0f)
};

static D3DXVECTOR3 LightDirectionTable_5[] = {
	D3DXVECTOR3(0.5f, -0.5f, 0.0f),
	D3DXVECTOR3(-0.5f, -0.5f, 0.0f),
	D3DXVECTOR3(0.5f, 0.5f, 0.0f),
	D3DXVECTOR3(-0.5f, 0.5f, 0.0f)
};

static D3DXVECTOR3 LightDirectionTable_6[] = {
	D3DXVECTOR3(0.5f, -0.5f, 0.0f),
	D3DXVECTOR3(-0.5f, -0.5f, 0.0f),
	D3DXVECTOR3(0.5f, 0.5f, 0.0f),
	D3DXVECTOR3(-0.5f, 0.5f, 0.0f)
};

static D3DXVECTOR3 LightDirectionTable_Final[] = {
	D3DXVECTOR3(0.5f, -0.5f, 0.0f),
	D3DXVECTOR3(-0.5f, -0.5f, 0.0f),
	D3DXVECTOR3(0.5f, 0.5f, 0.0f),
	D3DXVECTOR3(-0.5f, 0.5f, 0.0f)
};
#endif

static D3DXVECTOR3 LightDirectionTable_Boss[] = {
	D3DXVECTOR3(0.5f, -0.5f, 0.0f),
	D3DXVECTOR3(-0.5f, -0.5f, 0.0f),
	D3DXVECTOR3(0.5f, 0.5f, 0.0f),
	D3DXVECTOR3(-0.5f, 0.5f, 0.0f)
};


static D3DXVECTOR3* LightDirectionTableArray[] = {
	LightDirectionTable_1,
	LightDirectionTable_2,
	LightDirectionTable_3,
#ifdef TEISYUTU_YOU
#else
	LightDirectionTable_4,
	LightDirectionTable_5,
	LightDirectionTable_6,
	LightDirectionTable_Final,
#endif
	LightDirectionTable_Boss,
};