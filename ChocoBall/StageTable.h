#pragma once
#include "stdafx.h"



enum GimmickType{
	GimmickType_Chocoball,
	GimmickType_Wall,
	GimmickType_FallFloor,
	GimmickType_MoveFloor,
	GimmickType_UpFloor,
	GimmickType_FireJet,
	GimmickType_SmokeJet
};

enum EnemyType{
	EnemyType_LR,
	EnemyType_FB,
	EnemyType_JUMP,
	EnemyType_BULLET
};
struct SEnemyAndGimmickInfo{
	D3DXVECTOR3 pos;
	D3DXQUATERNION rot;
	D3DXVECTOR3 scale;
	int enemyType;
	int gimmickType;
	float MaxMove;
};
struct SCollisionInfo {
	D3DXVECTOR3 pos;
	D3DXQUATERNION rotation;
	D3DXVECTOR3 scale;
};

// フィールドのXファイル名
// ※上から順にステージ1,2,3……
static LPCSTR FieldNameArray[] = {
	"image/SUTE2_01.x",
	"image/StageModel_Iriguchi1.x",
	"image/StageModel_Huzita3.x",
	"image/StageModel_Huzita4.x",
	"image/StageModel_Huzita5.x",
	"image/StageModel_Iriguchi2.x",
	"image/StageModel_Ookawa1.x",
};

// フィールドで使用するBGM
static LPCSTR Stage_BGM[] = {
	"joban_Stage",
	"joban_Stage",
	"tyuban_Stage",
	"tyuban_Stage",
	"lastStage",
	"lastStage",
	"Modern",
};



enum STAGE_ID{ FIRST = 0, SECOUND,THARD,FOURTH,FIFTH,SIX, FINAL, MAX, STAGE_NONE = 999 };



// ステージ1で使用するオブジェクトの定義配列
// ※新しいステージを作ったら同じ書き方で新しい配列を追加してください
// ※オブジェクトの名前は、同じステージの中でなければ被っても構いません
// ※下の配列に新しい名前のオブジェクトを追加したらStage.cppのActivateObjects関数にif文を追加してください
static LPCSTR StageGameObject_1[] = {
	{ "BulletPhysics" },// すべてのステージにこれが必要です
	{ "TESTStage3D" },// すべてのステージにこれが必要です
	{ "Camera" },// すべてのステージにこれが必要です
	{ "EnemyManager" },// すべてのステージにこれが必要です
	{ "TEST3D" },// すべてのステージにこれが必要です
	{ "NUMBER" },// すべてのステージにこれが必要です
	{ "StageNUMBER" },// すべてのステージにこれが必要です
};
static LPCSTR StageGameObject_2[] = {
	{ "BulletPhysics" },	// すべてのステージにこれが必要です
	{ "TESTStage3D" },	// すべてのステージにこれが必要です
	{ "Camera" },	// すべてのステージにこれが必要です
	{ "EnemyManager" },	// すべてのステージにこれが必要です
	{ "TEST3D" },	// すべてのステージにこれが必要です
	{ "NUMBER" },// すべてのステージにこれが必要です
	{ "StageNUMBER" },// すべてのステージにこれが必要です
};
static LPCSTR StageGameObject_3[] = {
	{ "BulletPhysics" },	// すべてのステージにこれが必要です
	{ "TESTStage3D" },	// すべてのステージにこれが必要です
	{ "Camera" },	// すべてのステージにこれが必要です
	{ "EnemyManager" },	// すべてのステージにこれが必要です
	{ "TEST3D" },	// すべてのステージにこれが必要です
	{ "NUMBER" },// すべてのステージにこれが必要です
	{ "StageNUMBER" },// すべてのステージにこれが必要です
};
static LPCSTR StageGameObject_4[] = {
	{ "BulletPhysics" },	// すべてのステージにこれが必要です
	{ "TESTStage3D" },	// すべてのステージにこれが必要です
	{ "Camera" },	// すべてのステージにこれが必要です
	{ "EnemyManager" },	// すべてのステージにこれが必要です
	{ "TEST3D" },	// すべてのステージにこれが必要です
	{ "NUMBER" },// すべてのステージにこれが必要です
	{ "StageNUMBER" },// すべてのステージにこれが必要です
};
static LPCSTR StageGameObject_5[] = {
	{ "BulletPhysics" },	// すべてのステージにこれが必要です
	{ "TESTStage3D" },	// すべてのステージにこれが必要です
	{ "Camera" },	// すべてのステージにこれが必要です
	{ "EnemyManager" },	// すべてのステージにこれが必要です
	{ "TEST3D" },	// すべてのステージにこれが必要です
	{ "NUMBER" },// すべてのステージにこれが必要です
	{ "StageNUMBER" },// すべてのステージにこれが必要です
};
static LPCSTR StageGameObject_6[] = {
	{ "BulletPhysics" },	// すべてのステージにこれが必要です
	{ "TESTStage3D" },	// すべてのステージにこれが必要です
	{ "Camera" },	// すべてのステージにこれが必要です
	{ "EnemyManager" },	// すべてのステージにこれが必要です
	{ "TEST3D" },	// すべてのステージにこれが必要です
	{ "NUMBER" },// すべてのステージにこれが必要です
	{ "StageNUMBER" },// すべてのステージにこれが必要です
};
static LPCSTR StageGameObject_Final[] = {
	{ "BulletPhysics" },	// すべてのステージにこれが必要です
	{ "TESTStage3D" },	// すべてのステージにこれが必要です
	{ "Camera" },	// すべてのステージにこれが必要です
	{ "EnemyManager" },	// すべてのステージにこれが必要です
	{ "TEST3D" },	// すべてのステージにこれが必要です
	{ "NUMBER" },// すべてのステージにこれが必要です
	{ "StageNUMBER" },// すべてのステージにこれが必要です
};

// 上に新しい配列を追加したらここにそのオブジェクト配列のポインタを順番どおりに格納してください
// ※その後、一番下にある、配列の要素数を格納する配列も更新してください

static LPCSTR* StageArray[] = {
	StageGameObject_1,
	StageGameObject_2,
	StageGameObject_3,
	StageGameObject_4,
	StageGameObject_5,
	StageGameObject_6,
	StageGameObject_Final
};

// ステージオブジェクト配列の要素数を格納する配列
static int StageObjectNumArray[] = {
	ARRAYSIZE(StageGameObject_1),
	ARRAYSIZE(StageGameObject_2),
	ARRAYSIZE(StageGameObject_3),
	ARRAYSIZE(StageGameObject_4),
	ARRAYSIZE(StageGameObject_5),
	ARRAYSIZE(StageGameObject_6),
	ARRAYSIZE(StageGameObject_Final)
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

static D3DXVECTOR3* CourceDefineArray[] = {
	CourceDefine_1,
	CourceDefine_2,
	CourceDefine_3,
	CourceDefine_4,
	CourceDefine_5,
	CourceDefine_6,
	CourceDefine_Final
};

static int CourceDefineNumArray[] = {
	ARRAYSIZE(CourceDefine_1),
	ARRAYSIZE(CourceDefine_2),
	ARRAYSIZE(CourceDefine_3),
	ARRAYSIZE(CourceDefine_4),
	ARRAYSIZE(CourceDefine_5),
	ARRAYSIZE(CourceDefine_6),
	ARRAYSIZE(CourceDefine_Final)
};

// ステージごとのプレイヤーのポジション情報を格納
static SCollisionInfo PlayerTransformArray[] = {
#include "PlayerPositionInfoStage1.h"
#include "PlayerPositionInfoStage2.h"
#include "PlayerPositionInfoStage3.h"
#include "PlayerPositionInfoStage4.h"
#include "PlayerPositionInfoStage5.h"
#include "PlayerPositionInfoStage6.h"
#include "PlayerPositionInfoStageFinal.h"
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


// ステージ1で使用するギミックやエネミーのポジションなどを設定した配列のポインタを格納する配列
// ※ステージを追加したら下のような書き方で配列に要素を追加してください
static SEnemyAndGimmickInfo* infoTableArray[] = {
	infoTable_1,
	infoTable_2,
	infoTable_3,
	infoTable_4,
	infoTable_5,
	infoTable_6,
	infoTable_Final
};

// ステージ1で使用するギミックやエネミーのポジションなどを設定した配列の要素数を格納する配列
// ※ステージを追加したら下のような書き方で配列に要素を追加してください
static int InfoTableSizeArray[] = {
	ARRAYSIZE(infoTable_1),
	ARRAYSIZE(infoTable_2),
	ARRAYSIZE(infoTable_3),
	ARRAYSIZE(infoTable_4),
	ARRAYSIZE(infoTable_5),
	ARRAYSIZE(infoTable_6),
	ARRAYSIZE(infoTable_Final)
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
// ステージ1で使用するギミック発動用当たり判定を設定した配列のポインタを格納する配列
// ※ステージを追加したら下のような書き方で配列に要素を追加してください
static SCollisionInfo* GimmickinfoTableArray[] = {
	GimmickTriggerInfoTable_1,
	GimmickTriggerInfoTable_2,
	GimmickTriggerInfoTable_3,
	GimmickTriggerInfoTable_4,
	GimmickTriggerInfoTable_5,
	GimmickTriggerInfoTable_6,
	GimmickTriggerInfoTable_Final
};


// ステージ1で使用するギミック発動用当たり判定を設定した配列の要素数を格納する配列
// ※ステージを追加したら下のような書き方で配列に要素を追加してください
static int GimmickInfoTableSizeArray[] = {
	ARRAYSIZE(GimmickTriggerInfoTable_1),
	ARRAYSIZE(GimmickTriggerInfoTable_2),
	ARRAYSIZE(GimmickTriggerInfoTable_3),
	ARRAYSIZE(GimmickTriggerInfoTable_4),
	ARRAYSIZE(GimmickTriggerInfoTable_5),
	ARRAYSIZE(GimmickTriggerInfoTable_6),
	ARRAYSIZE(GimmickTriggerInfoTable_Final)
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


// ステージ1のフィールドの当たり判定を設定した配列のポインタを格納する配列
// ※ステージを追加したら下のような書き方で配列に要素を追加してください
static SCollisionInfo* collisionInfoTableArray[] = {
	collisionInfoTable_1,
	collisionInfoTable_2,
	collisionInfoTable_3,
	collisionInfoTable_4,
	collisionInfoTable_5,
	collisionInfoTable_6,
	collisionInfoTable_Final
};

// ステージ1でのフィールドの当たり判定を設定した配列の要素数を格納する配列
// ※ステージを追加したら下のような書き方で配列に要素を追加してください
static int collisionInfoTableSizeArray[] = {
	ARRAYSIZE(collisionInfoTable_1),
	ARRAYSIZE(collisionInfoTable_2),
	ARRAYSIZE(collisionInfoTable_3),
	ARRAYSIZE(collisionInfoTable_4),
	ARRAYSIZE(collisionInfoTable_5),
	ARRAYSIZE(collisionInfoTable_6),
	ARRAYSIZE(collisionInfoTable_Final)
};

