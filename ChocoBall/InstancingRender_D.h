#pragma once
#include "Render.h"
class CInstancingRender_D :
	public CRender
{
public:
	CInstancingRender_D();
	~CInstancingRender_D();
	void CreateMatrixBuffer(int);	// 考え得る最大数分のワールド行列格納用バッファを定義する関数
	void AddWorldMatrix(const D3DXMATRIX&);
	void Initialize()override;
	void Draw()override{};
	void Draw(const D3DXVECTOR2&, const D3DXVECTOR3&, const D3DXMATRIX&)override;
private:
	/*!
	*@brief	ワールド行列を頂点バッファにコピー。
	*/
	void CopyMatrixToVertexBuffer();
	void DrawFrameDepth(LPD3DXFRAME, LPD3DXEFFECT, const D3DXVECTOR2&, const D3DXVECTOR3&, const D3DXMATRIX&);
	void DrawMeshContainerDepth(LPD3DXMESHCONTAINER, LPD3DXFRAME, LPD3DXEFFECT, const D3DXVECTOR2&, const D3DXVECTOR3&, const D3DXMATRIX&);
	void AnimationDrawDepth(D3DXMESHCONTAINER_DERIVED*, D3DXFRAME_DERIVED*, LPD3DXEFFECT, const D3DXVECTOR2&, const D3DXVECTOR3&, const D3DXMATRIX&);
	void NonAnimationDrawDepth(D3DXFRAME_DERIVED*, LPD3DXEFFECT, const D3DXVECTOR2&, const D3DXVECTOR3&, const D3DXMATRIX&);
private:
	bool m_IsFirst;
	// インスタンシング描画用変数
	IDirect3DVertexBuffer9* m_WorldMatrixBuffer = nullptr;			//ワールド行列のバッファ。
	IDirect3DVertexDeclaration9* m_VertexDecl = nullptr;			//頂点定義。
	vector<D3DXMATRIX> m_worldMatrix;	// ワールド行列の配列
};

