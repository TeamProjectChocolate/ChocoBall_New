#pragma once
#include "stdafx.h"
#include "Assert.h"

enum EType{
	eTriangleList = 0,
	eTriangleStrip,
	eTypeNum
};

static const D3DVERTEXELEMENT9 scShapeVertex_PT_Element[] = {
	{ 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	D3DDECL_END()
};

class CPrimitive
{
public:
	CPrimitive();
	~CPrimitive();
	// プリミティブ生成関数。
	// 引数:	インデックスバッファの形式。
	//			頂点数。
	//			1つの頂点のサイズ。
	//			頂点定義(外部から頂点定義を指定する場合はここに入れる。nullptrを入れるとクラス内で生成される)。
	//			頂点レイアウト(1つの頂点の構成を示す設計図のようなもの。第四引数がnullptrだった場合はこれを参照して頂点定義を作成する)。
	//			頂点バッファに渡す頂点情報。
	//			インデックスバッファのサイズ。
	//			インデックスバッファのフォーマット。
	//			インデックスバッファに設定する値。
	void Create(EType, int, int, LPDIRECT3DVERTEXDECLARATION9 ,const D3DVERTEXELEMENT9*, void*, int, D3DFORMAT, void*);
	void Release();
	LPDIRECT3DVERTEXBUFFER9 GetVertexBuffer(){
		return m_vertexBuffer;
	}
	LPDIRECT3DINDEXBUFFER9 GetIndexBuffer(){
		return m_indexBuffer;
	}
	D3DPRIMITIVETYPE GetD3DPrimitiveType() const{
		return m_d3dPrimitiveType;
	}
	int GetNumVertex()const{
		return m_numVertex;
	}
	int GetNumpolygon()const{
		return m_numPolygon;
	}
	void VertexBufferCreate(int, int, const void*);
	void IndexBufferCreate(int, D3DFORMAT, const void*);
	void VertexDeclCreate(LPDIRECT3DVERTEXDECLARATION9 decl, const D3DVERTEXELEMENT9* element);
		// 頂点定義の取得
	IDirect3DVertexDeclaration9* GetVertexDecl(){
		return m_pVertexDecl;
	}
private:
	int m_numVertex;
	int m_numIndex;
	int m_vertexStride;
	int m_numPolygon;
	LPDIRECT3DVERTEXBUFFER9 m_vertexBuffer = nullptr;
	LPDIRECT3DINDEXBUFFER9 m_indexBuffer = nullptr;
	EType m_type;
	D3DPRIMITIVETYPE m_d3dPrimitiveType;
	IDirect3DVertexDeclaration9* m_pVertexDecl = nullptr;
};

