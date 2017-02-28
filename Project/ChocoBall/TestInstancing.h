#pragma once
#include "stdafx.h"
#include "C3DImage.h"
#include "Effect.h"

#define MAX_INSTANCE 5000

class CTestInstancing
{
public:
	CTestInstancing();
	~CTestInstancing();
	void CreateMatrixBuffer(int);	// 考え得る最大数分のワールド行列格納用バッファを定義する関数
	void Initialize();
	void Update();
	void Draw();
	void CopyMatrixToVertexBuffer();
private:
	C3DImage model;
	LPD3DXEFFECT m_pEffect;
	// インスタンシング描画用変数
	IDirect3DVertexBuffer9* m_WorldMatrixBuffer = nullptr;			//ワールド行列のバッファ。
	IDirect3DVertexDeclaration9* m_VertexDecl = nullptr;			//頂点定義。
	vector<D3DXMATRIX> m_worldMatrix;	// ワールド行列の配列
	vector<D3DXMATRIX*> pWorlds;
	// シェーダーに値をセットするためのハンドル群
	D3DXHANDLE m_hEyePosition;
	D3DXHANDLE m_hWorldMatrixArray;
	D3DXHANDLE m_hluminance;
	D3DXHANDLE m_hnumBone;
	D3DXHANDLE m_hAlpha;
	D3DXHANDLE m_hRota;
	D3DXHANDLE m_hWorld;
	D3DXHANDLE m_hTexture;
	D3DXHANDLE m_hShadowMap;
	D3DXVECTOR3 margin[MAX_INSTANCE];
	float  m_IntervalTime;
	float m_TimeCounter;
	D3DXVECTOR3 rnd[MAX_INSTANCE];
	D3DXVECTOR3 TargetRnd[MAX_INSTANCE];
	bool flg[MAX_INSTANCE];
};

