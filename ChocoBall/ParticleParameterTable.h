#pragma once
#include "stdafx.h"


// パーティクル生成パラメータ構造体の定義
typedef struct SParticleEmitParameter{
	void Init(){
		memset(this, 0, sizeof(SParticleEmitParameter));
		initAlpha = 0.0f;
		uvTableSize = 1;
		brightness = 1.0f;
		isBillboard = true;
	};
	const char* texturePath;	// テクスチャのファイルパス
	D3DXVECTOR3 initVelocity;	// 初速度
	float life;					// 寿命
	float intervalTime;			// 発生時間
	int EmitNum;				// 1フレームに発生させる量
	float w;					// パーティクルの幅
	float h;					// パーティクルの高さ
	float size_randMax;				// パーティクルサイズのランダム最大倍率
	float size_randMin;				// パーティクルサイズのランダム最小倍率
	D3DXVECTOR3 initPositionRandomMargin;	// 初期位置のランダム幅
	D3DXVECTOR3 initVelocityVelocityRandomMargin;	// 初速度のランダム幅
	D3DXVECTOR3 addVelocityRandomMargin;			// 速度の積分のときのランダム幅
	D3DXVECTOR4 uvTable[4];		// uvテーブル。最大4まで保存できる。xが左上のu。yが左上のv。zが右下のu。wが右下のvになる。
	int uvTableSize;			// uvテーブルのサイズ
	D3DXVECTOR3 gravity;		// 重力
	bool isFade;				// 死ぬときにフェードアウトするか
	float fadeTime;				// フェードする時間
	float initAlpha;			// 初期アルファ値
	bool isBillboard;			// ビルボード？
	float brightness;			// 輝度。ブルームが有効になっているとこれを強くすると光があふれる
	int alphaBlendMode;			// 半透明合成、1加算合成
};




// 新しいパラメータを設定したらこの列挙体に追加すること
enum PARTICLE_TYPE{ FIRE = 0, PORIGON, FIREGIMMICK, STAR, SMOKE, GUNPARTICLE, SOMKEGIMMICK };


// パーティクルのパラメータ
// 新しくパーティクルを生成したらここに以下のような書き方で追加してください
// ※追加した構造体のポインタを一番下のポインター配列に追加するのを忘れないこと

// 燃え上がる炎
static SParticleEmitParameter Param_Fire = {
	"image/PRT_Fire.png",	//テクスチャのファイル名
	D3DXVECTOR3(0.0f, 1.0f, 0.0f),		// 初速度。
	1.5f,							// 寿命。単位は秒。
	0.001f,							// 発生時間。単位は秒。
	1,								// 1フレームに発生させる量
	0.5f,							// パーティクルの幅。
	0.5f,							// パーティクルの高さ。
	1.0f,							// パーティクルサイズのランダム最大倍率
	1.0f,							// パーティクルサイズのランダム最小倍率
	D3DXVECTOR3(0.3f, 0.0f, 0.3f),		// 初期位置のランダム幅。
	D3DXVECTOR3(0.3f, 0.0f, 0.0f),		// 初速度のランダム幅。
	D3DXVECTOR3(0.006f, 0.0f, 0.006f),	// 速度の積分のときのランダム幅。
	{	// UVテーブル。最大4まで保持できる。xが左上のu、yが左上のv、zが右下のu、wが右下のvになる。
		D3DXVECTOR4(0.0f, 0.0f, 0.25f, 0.5f),
		D3DXVECTOR4(0.25f, 0.0f, 0.5f, 0.5f),
		D3DXVECTOR4(0.5f, 0.0f, 0.75f, 0.5f),
		D3DXVECTOR4(0.75f, 0.0f, 1.0f, 0.5f),
	},
	4,									// UVテーブルのサイズ。
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// 重力。
	true,							// 死ぬときにフェードアウトする？
	1.5f,							// フェードする時間。
	1.0f,							// 初期アルファ値。
	true,							// ビルボード？
	2.0f,							// 輝度。ブルームが有効になっているとこれを強くすると光が溢れます。
	1,								// 半透明合成、1加算合成。
};

// 黄色の豆腐
static SParticleEmitParameter Param_Porigon = {
	"image/test.png",	//テクスチャのファイル名
	D3DXVECTOR3(0.0f, 1.0f, 0.0f),		// 初速度。
	1.5f,							// 寿命。単位は秒。
	0.001f,							// 発生時間。単位は秒。
	1,								// 1フレームに発生させる量
	0.5f,							// パーティクルの幅。
	0.5f,							// パーティクルの高さ。
	1.0f,							// パーティクルサイズのランダム最大倍率
	1.0f,							// パーティクルサイズのランダム最小倍率
	D3DXVECTOR3(0.3f, 0.0f, 0.3f),		// 初期位置のランダム幅。
	D3DXVECTOR3(0.3f, 0.0f, 0.3f),		// 初速度のランダム幅。
	D3DXVECTOR3(0.006f, 0.0f, 0.006f),	// 速度の積分のときのランダム幅。
	{	// UVテーブル。最大4まで保持できる。xが左上のu、yが左上のv、zが右下のu、wが右下のvになる。
		D3DXVECTOR4(0.0f, 0.0f, 0.25f, 0.5f),
		D3DXVECTOR4(0.25f, 0.0f, 0.5f, 0.5f),
		D3DXVECTOR4(0.5f, 0.0f, 0.75f, 0.5f),
		D3DXVECTOR4(0.75f, 0.0f, 1.0f, 0.5f),
	},
	4,									// UVテーブルのサイズ。
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// 重力。
	true,							// 死ぬときにフェードアウトする？
	1.5f,							// フェードする時間。
	1.0f,							// 初期アルファ値。
	true,							// ビルボード？
	2.0f,							// 輝度。ブルームが有効になっているとこれを強くすると光が溢れます。
	1,								// 半透明合成、1加算合成。
};

// 炎噴出
static SParticleEmitParameter Param_FireGimmick = {
	"image/PRT_Fire.png",	//テクスチャのファイル名
	D3DXVECTOR3(0.0f, 0.0f, 1.0f),		// 初速度。
	4.0f,							// 寿命。単位は秒。
	0.0f,							// 発生時間。単位は秒。
	1,								// 1フレームに発生させる量
	0.8f,							// パーティクルの幅。
	0.8f,							// パーティクルの高さ。
	1.0f,							// パーティクルサイズのランダム最大倍率
	1.0f,							// パーティクルサイズのランダム最小倍率
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// 初期位置のランダム幅。
	D3DXVECTOR3(0.25f, 0.25f, 0.0f),		// 初速度のランダム幅。
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// 速度の積分のときのランダム幅。
	{	// UVテーブル。最大4まで保持できる。xが左上のu、yが左上のv、zが右下のu、wが右下のvになる。
		D3DXVECTOR4(0.0f, 0.0f, 0.25f, 0.5f),
		D3DXVECTOR4(0.25f, 0.0f, 0.5f, 0.5f),
		D3DXVECTOR4(0.5f, 0.0f, 0.75f, 0.5f),
		D3DXVECTOR4(0.75f, 0.0f, 1.0f, 0.5f),
	},
	4,									// UVテーブルのサイズ。
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// 重力。
	true,							// 死ぬときにフェードアウトする？
	0.5f,							// フェードする時間。
	1.0f,							// 初期アルファ値。
	true,							// ビルボード？
	2.0f,							// 輝度。ブルームが有効になっているとこれを強くすると光が溢れます。
	1,								// 半透明合成、1加算合成。
};

// 星
static SParticleEmitParameter Param_Star = {
	"image/Star.png",	//テクスチャのファイル名
	D3DXVECTOR3(0.0f, 6.5f, 0.0f),		// 初速度。
	1.0f,							// 寿命。単位は秒。
	0.05f,							// 発生時間。単位は秒。
	1,								// 1フレームに発生させる量
	0.7f,							// パーティクルの幅。
	0.7f,							// パーティクルの高さ。
	0.8f,							// パーティクルサイズのランダム最大倍率
	0.3f,							// パーティクルサイズのランダム最小倍率
	D3DXVECTOR3(0.1f, 0.0f, 0.1f),		// 初期位置のランダム幅。
	D3DXVECTOR3(0.5f, 0.5f, 0.5f),		// 初速度のランダム幅。
	D3DXVECTOR3(0.4f, 0.4f, 0.4f),	// 速度の積分のときのランダム幅。
	{	// UVテーブル。最大4まで保持できる。xが左上のu、yが左上のv、zが右下のu、wが右下のvになる。
		D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f),
		D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f),
		D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f),
		D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f),
	},
	1,									// UVテーブルのサイズ。
	D3DXVECTOR3(0.0f, -9.8f, 0.0f),		// 重力。
	true,							// 死ぬときにフェードアウトする？
	1.5f,							// フェードする時間。
	1.0f,							// 初期アルファ値。
	true,							// ビルボード？
	1.7f,							// 輝度。ブルームが有効になっているとこれを強くすると光が溢れます。
	1,								// 半透明合成、1加算合成。
};

//砂煙（ジャンプ）
static SParticleEmitParameter Param_Smoke = {
	"image/pipo-btleffect059.png",	//テクスチャのファイル名
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// 初速度。
	0.2f,							// 寿命。単位は秒。
	0.001f,							// 発生時間。単位は秒。
	2,								// 1フレームに発生させる量
	0.7f,							// パーティクルの幅。
	0.7f,							// パーティクルの高さ。
	1.0f,							// パーティクルサイズのランダム最大倍率
	0.5f,							// パーティクルサイズのランダム最小倍率
	D3DXVECTOR3(0.1f, -0.1f, 0.1f),		// 初期位置のランダム幅。
	D3DXVECTOR3(1.0f, 0.001f, 1.0f),		// 初速度のランダム幅。
	D3DXVECTOR3(0.006f, 0.0f, 0.006f),	// 速度の積分のときのランダム幅。
	{	// UVテーブル。最大4まで保持できる。xが左上のu、yが左上のv、zが右下のu、wが右下のvになる。
		D3DXVECTOR4(0.3f, 0.0f, 0.4f, 1.0f),
		D3DXVECTOR4(0.4f, 0.0f, 0.5f, 1.0f),
		D3DXVECTOR4(0.5f, 0.0f, 0.6f, 1.0f),
		D3DXVECTOR4(0.6f, 0.0f, 0.7f, 1.0f),
	},
	4,									// UVテーブルのサイズ。
	D3DXVECTOR3(0.0f, 0.7f, 0.0f),		// 重力。
	true,							// 死ぬときにフェードアウトする？
	0.2f,							// フェードする時間。
	1.0f,							// 初期アルファ値。
	true,							// ビルボード？
	1.0f,							// 輝度。ブルームが有効になっているとこれを強くすると光が溢れます。
	1,								// 半透明合成、1加算合成。
};

//銃発射時のエフェクト
static SParticleEmitParameter Param_GunParticle = {
	"image/Star.png",	//テクスチャのファイル名
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// 初速度。
	0.01f,							// 寿命。単位は秒。
	0.001f,							// 発生時間。単位は秒。
	4,								// 1フレームに発生させる量
	0.7f,							// パーティクルの幅。
	0.7f,							// パーティクルの高さ。
	0.8f,							// パーティクルサイズのランダム最大倍率
	0.3f,							// パーティクルサイズのランダム最小倍率
	D3DXVECTOR3(0.1f, -0.1f, 0.1f),		// 初期位置のランダム幅。
	D3DXVECTOR3(1.0f, 0.001f, 1.0f),		// 初速度のランダム幅。
	D3DXVECTOR3(0.006f, 0.0f, 0.006f),	// 速度の積分のときのランダム幅。
	{	// UVテーブル。最大4まで保持できる。xが左上のu、yが左上のv、zが右下のu、wが右下のvになる。
		D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f),
		D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f),
		D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f),
		D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f),
	},
	4,									// UVテーブルのサイズ。
	D3DXVECTOR3(0.0f, 0.7f, 0.0f),		// 重力。
	true,							// 死ぬときにフェードアウトする？
	0.01f,							// フェードする時間。
	1.0f,							// 初期アルファ値。
	true,							// ビルボード？
	1.0f,							// 輝度。ブルームが有効になっているとこれを強くすると光が溢れます。
	0,								// 半透明合成、1加算合成。
};

// 煙噴出
static SParticleEmitParameter Param_SmokeGimmick = {
	"image/pipo-btleffect059.png",	//テクスチャのファイル名
	D3DXVECTOR3(0.0f, 0.0f, 1.0f),		// 初速度。
	4.0f,							// 寿命。単位は秒。
	0.0f,							// 発生時間。単位は秒。
	1,								// 1フレームに発生させる量
	0.6f,							// パーティクルの幅。
	0.6f,							// パーティクルの高さ。
	1.0f,							// パーティクルサイズのランダム最大倍率
	1.0f,							// パーティクルサイズのランダム最小倍率
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// 初期位置のランダム幅。
	D3DXVECTOR3(0.1f, 0.1f, 0.0f),		// 初速度のランダム幅。
	D3DXVECTOR3(0.006f, 0.0f, 0.0006f),	// 速度の積分のときのランダム幅。
	{	// UVテーブル。最大4まで保持できる。xが左上のu、yが左上のv、zが右下のu、wが右下のvになる。
		D3DXVECTOR4(0.3f, 0.0f, 0.4f, 1.0f),
		D3DXVECTOR4(0.4f, 0.0f, 0.5f, 1.0f),
		D3DXVECTOR4(0.5f, 0.0f, 0.6f, 1.0f),
		D3DXVECTOR4(0.6f, 0.0f, 0.7f, 1.0f),
	},
	4,									// UVテーブルのサイズ。
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// 重力。
	true,							// 死ぬときにフェードアウトする？
	0.5f,							// フェードする時間。
	1.0f,							// 初期アルファ値。
	true,							// ビルボード？
	2.0f,							// 輝度。ブルームが有効になっているとこれを強くすると光が溢れます。
	1,								// 半透明合成、1加算合成。
};


// パラメーター構造体のポインタ配列
// ※パラメーターを追加したらここに追加した構造体のポインタを格納すること
static SParticleEmitParameter* ParticleParamPT_Array[] = { 
	&Param_Fire,
	&Param_Porigon,
	&Param_FireGimmick,
	&Param_Star,
	&Param_Smoke,
	&Param_GunParticle,
	&Param_SmokeGimmick,
};
