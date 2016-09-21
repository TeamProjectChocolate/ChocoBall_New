#pragma once
#include "Render.h"
class CShadowSamplingRender_I :
	public CRender
{
public:
	CShadowSamplingRender_I();
	~CShadowSamplingRender_I();
	void CreateMatrixBuffer(unsigned int);	// 考え得る最大数分のワールド行列格納用バッファを定義する関数
	void AddWorldMatrix(const D3DXMATRIX&);
	void Initialize()override;
	void Draw()override;
private:
	/*!
	*@brief	ワールド行列を頂点バッファにコピー。
	*/
	void CopyMatrixToVertexBuffer();
	void DrawFrame(LPD3DXFRAME);
	void DrawMeshContainer(LPD3DXMESHCONTAINER, LPD3DXFRAME);
	void AnimationDraw(D3DXMESHCONTAINER_DERIVED*, D3DXFRAME_DERIVED*);
	void NonAnimationDraw(D3DXFRAME_DERIVED*);
	inline void SetShadowCamera(CCamera* camera)override{
		m_pShadowCamera = camera;
	}
private:
	bool m_UseBorn;
	bool m_IsFirst;
	// インスタンシング描画用変数
	IDirect3DVertexBuffer9* m_WorldMatrixBuffer = nullptr;			//ワールド行列のバッファ。
	IDirect3DVertexDeclaration9* m_VertexDecl = nullptr;			//頂点定義。
	vector<D3DXMATRIX> m_worldMatrix;	// ワールド行列の配列
	CCamera* m_pShadowCamera;
	unsigned int m_MaxNum;
};

