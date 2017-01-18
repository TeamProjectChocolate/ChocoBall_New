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
	GimmickType_None = 999
};

enum EnemyType{
	EnemyType_LR,
	EnemyType_FB,
	EnemyType_JUMP,
	EnemyType_BULLET,
	EnemyType_None = 999
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

struct MapObject
{
	const char* FileName;
	D3DXVECTOR3 Position;
	D3DXQUATERNION Rotation;
	D3DXVECTOR3 Scale;
	int RType;	// ���ܗ��B
	float Alpha;
};

// �t�B�[���h�Ŏg�p����X�t�@�C�����B
// ���ォ�珇�ɃX�e�[�W1,2,3�c�c
static MapObject MapObjects_1[] = {
#include "MapChipOutputByUnityStage1.h"
};
static MapObject MapObjects_2[] = {
#include "MapChipOutputByUnityStage2.h"
};
static MapObject MapObjects_3[] = {
#include "MapChipOutputByUnityStage3.h"
};
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
static MapObject MapObjects_Boss[] = {
#include "MapChipOutputByUnityStageBoss.h"
};

static MapObject* MapObjectsPointerArray[] = {
	MapObjects_1,
	MapObjects_2,
	MapObjects_3,
	MapObjects_4,
	MapObjects_5,
	MapObjects_6,
	MapObjects_Final,
	MapObjects_Boss,
};

static int MapObjectsMaxArray[] = {
	ARRAYSIZE(MapObjects_1),
	ARRAYSIZE(MapObjects_2),
	ARRAYSIZE(MapObjects_3),
	ARRAYSIZE(MapObjects_4),
	ARRAYSIZE(MapObjects_5),
	ARRAYSIZE(MapObjects_6),
	ARRAYSIZE(MapObjects_Final),
	ARRAYSIZE(MapObjects_Boss),
};

// �t�B�[���h�Ŏg�p����BGM
static LPCSTR Stage_BGM[] = {
	"joban_Stage",
	"joban_Stage",
	"tyuban_Stage",
	"tyuban_Stage",
	"lastStage",
	"lastStage",
	"Modern",
	"Modern",
};

enum STAGE_ID{ FIRST = 0, SECOUND,THARD,FOURTH,FIFTH,SIX, FINAL,BOSS, MAX, STAGE_NONE = 999 };


// �X�e�[�W1�̃R�[�X��`
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
static D3DXVECTOR3 CourceDefine_Boss[] = {
#include "CourceDefStageBoss.h"
};


static D3DXVECTOR3* CourceDefineArray[] = {
	CourceDefine_1,
	CourceDefine_2,
	CourceDefine_3,
	CourceDefine_4,
	CourceDefine_5,
	CourceDefine_6,
	CourceDefine_Final,
	CourceDefine_Boss,
};

static int CourceDefineNumArray[] = {
	ARRAYSIZE(CourceDefine_1),
	ARRAYSIZE(CourceDefine_2),
	ARRAYSIZE(CourceDefine_3),
	ARRAYSIZE(CourceDefine_4),
	ARRAYSIZE(CourceDefine_5),
	ARRAYSIZE(CourceDefine_6),
	ARRAYSIZE(CourceDefine_Final),
	ARRAYSIZE(CourceDefine_Boss),
};

// �X�e�[�W���Ƃ̃v���C���[�̃|�W�V���������i�[
static SCollisionInfo PlayerTransformArray[] = {
#include "PlayerPositionInfoStage1.h"
#include "PlayerPositionInfoStage2.h"
#include "PlayerPositionInfoStage3.h"
#include "PlayerPositionInfoStage4.h"
#include "PlayerPositionInfoStage5.h"
#include "PlayerPositionInfoStage6.h"
#include "PlayerPositionInfoStageFinal.h"
#include "PlayerPositionInfoStageBoss.h"
};

static D3DXVECTOR3 EM_CameraPosArray[] = {
#include "OutputMapCameraStage1.h"
#include "OutputMapCameraStage2.h"
#include "OutputMapCameraStage3.h"
#include "OutputMapCameraStage4.h"
#include "OutputMapCameraStage5.h"
#include "OutputMapCameraStage6.h"
#include "OutputMapCameraStageFinal.h"
#include "OutputMapCameraStageBoss.h"
};

// �X�e�[�W1�Ŏg�p����M�~�b�N��G�l�~�[�̃|�W�V�����Ȃǂ�ݒ肵���z��
// ���X�e�[�W��ǉ������瓯���悤�Ȕz���V��������āA�ǉ������X�e�[�W�̏o�̓w�b�_�[��ǂݍ���ł�������
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
static SEnemyAndGimmickInfo infoTable_Boss[] = {
#include "EnemyGimmickInfoStageBoss.h"
};


// �X�e�[�W1�Ŏg�p����M�~�b�N��G�l�~�[�̃|�W�V�����Ȃǂ�ݒ肵���z��̃|�C���^���i�[����z��
// ���X�e�[�W��ǉ������牺�̂悤�ȏ������Ŕz��ɗv�f��ǉ����Ă�������
static SEnemyAndGimmickInfo* infoTableArray[] = {
	infoTable_1,
	infoTable_2,
	infoTable_3,
	infoTable_4,
	infoTable_5,
	infoTable_6,
	infoTable_Final,
	infoTable_Boss,
};

// �X�e�[�W1�Ŏg�p����M�~�b�N��G�l�~�[�̃|�W�V�����Ȃǂ�ݒ肵���z��̗v�f�����i�[����z��
// ���X�e�[�W��ǉ������牺�̂悤�ȏ������Ŕz��ɗv�f��ǉ����Ă�������
static int InfoTableSizeArray[] = {
	ARRAYSIZE(infoTable_1),
	ARRAYSIZE(infoTable_2),
	ARRAYSIZE(infoTable_3),
	ARRAYSIZE(infoTable_4),
	ARRAYSIZE(infoTable_5),
	ARRAYSIZE(infoTable_6),
	ARRAYSIZE(infoTable_Final),
	ARRAYSIZE(infoTable_Boss),
};


// �X�e�[�W1�Ŏg�p����M�~�b�N�����p�����蔻��
// ���X�e�[�W��ǉ������瓯���悤�Ȕz���V��������āA�ǉ������X�e�[�W�̏o�̓w�b�_�[��ǂݍ���ł�������
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
static SCollisionInfo GimmickTriggerInfoTable_Boss[] = {
#include "GimmickTriggerInfoStageBoss.h"
};


// �X�e�[�W1�Ŏg�p����M�~�b�N�����p�����蔻���ݒ肵���z��̃|�C���^���i�[����z��
// ���X�e�[�W��ǉ������牺�̂悤�ȏ������Ŕz��ɗv�f��ǉ����Ă�������
static SCollisionInfo* GimmickinfoTableArray[] = {
	GimmickTriggerInfoTable_1,
	GimmickTriggerInfoTable_2,
	GimmickTriggerInfoTable_3,
	GimmickTriggerInfoTable_4,
	GimmickTriggerInfoTable_5,
	GimmickTriggerInfoTable_6,
	GimmickTriggerInfoTable_Final,
	GimmickTriggerInfoTable_Boss,
};


// �X�e�[�W1�Ŏg�p����M�~�b�N�����p�����蔻���ݒ肵���z��̗v�f�����i�[����z��
// ���X�e�[�W��ǉ������牺�̂悤�ȏ������Ŕz��ɗv�f��ǉ����Ă�������
static int GimmickInfoTableSizeArray[] = {
	ARRAYSIZE(GimmickTriggerInfoTable_1),
	ARRAYSIZE(GimmickTriggerInfoTable_2),
	ARRAYSIZE(GimmickTriggerInfoTable_3),
	ARRAYSIZE(GimmickTriggerInfoTable_4),
	ARRAYSIZE(GimmickTriggerInfoTable_5),
	ARRAYSIZE(GimmickTriggerInfoTable_6),
	ARRAYSIZE(GimmickTriggerInfoTable_Final),
	ARRAYSIZE(GimmickTriggerInfoTable_Boss),
};

// �X�e�[�W1�̃t�B�[���h�̓����蔻��
// ���X�e�[�W��ǉ������瓯���悤�Ȕz���V��������āA�ǉ������X�e�[�W�̏o�̓w�b�_�[��ǂݍ���ł�������
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
static SCollisionInfo collisionInfoTable_Boss[] = {
#include "collisionInfoStageBoss.h"
};



// �X�e�[�W1�̃t�B�[���h�̓����蔻���ݒ肵���z��̃|�C���^���i�[����z��
// ���X�e�[�W��ǉ������牺�̂悤�ȏ������Ŕz��ɗv�f��ǉ����Ă�������
static SCollisionInfo* collisionInfoTableArray[] = {
	collisionInfoTable_1,
	collisionInfoTable_2,
	collisionInfoTable_3,
	collisionInfoTable_4,
	collisionInfoTable_5,
	collisionInfoTable_6,
	collisionInfoTable_Final,
	collisionInfoTable_Boss,
};

// �X�e�[�W1�ł̃t�B�[���h�̓����蔻���ݒ肵���z��̗v�f�����i�[����z��
// ���X�e�[�W��ǉ������牺�̂悤�ȏ������Ŕz��ɗv�f��ǉ����Ă�������
static int collisionInfoTableSizeArray[] = {
	ARRAYSIZE(collisionInfoTable_1),
	ARRAYSIZE(collisionInfoTable_2),
	ARRAYSIZE(collisionInfoTable_3),
	ARRAYSIZE(collisionInfoTable_4),
	ARRAYSIZE(collisionInfoTable_5),
	ARRAYSIZE(collisionInfoTable_6),
	ARRAYSIZE(collisionInfoTable_Final),
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
	LightColorTable_4,
	LightColorTable_5,
	LightColorTable_6,
	LightColorTable_Final,
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
	LightDirectionTable_4,
	LightDirectionTable_5,
	LightDirectionTable_6,
	LightDirectionTable_Final,
	LightDirectionTable_Boss,
};