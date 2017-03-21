#pragma once
#include "Render.h"
class CEM_SamplingRender_I :
	public CRender
{
public:
	CEM_SamplingRender_I();
	~CEM_SamplingRender_I();
	void Initialize()override;
	void Draw()override;
	void CreateMatrixBuffer(unsigned int);	// 考え得る最大数分のワールド行列格納用バッファを定義する関数
	void AddWorldMatrix(const D3DXMATRIX&);
	void AddRotationMatrix(const D3DXMATRIX&);

	void SetCamera(CCamera* camera){
		m_pCamera = camera;
	}
private:
	/*!
	*@brief	ワールド行列を頂点バッファにコピー。
	*/
	void CopyMatrixToVertexBuffer();
	void DrawFrame(LPD3DXFRAME);
	void DrawMeshContainer(LPD3DXMESHCONTAINER, LPD3DXFRAME);
	void AnimationDraw(ANIMATION::D3DXMESHCONTAINER_DERIVED*, ANIMATION::D3DXFRAME_DERIVED*);
	void NonAnimationDraw(ANIMATION::D3DXFRAME_DERIVED*);
private:
	bool m_UseBorn;
	CCamera* m_pCamera;
	bool m_IsFirst;
	// インスタンシング描画用変数
	IDirect3DVertexBuffer9* m_WorldMatrixBuffer = nullptr;			//ワールド行列のバッファ。
	IDirect3DVertexDeclaration9* m_VertexDecl = nullptr;			//頂点定義。
	vector<D3DXMATRIX> m_worldMatrix;	// ワールド行列の配列
	IDirect3DVertexBuffer9* m_RotationMatrixBuffer = nullptr;
	vector<D3DXMATRIX> m_RotationMatrix;
	unsigned int m_MaxNum;	// バッファーのサイズ(この数を超えるバッファーサイズが必要になった場合、バッファーを作り直す);
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
};

