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

	// 板ポリとプリミティブ生成関数。
	// 引数:	板ポリサイズ横。
	//			板ポリサイズ縦。
	//			uv(xが左上のU、xが左上のV、xが右下のU、xが右下のV)。
	//			インデックスバッファの形式。
	//			頂点数。
	//			1つの頂点のサイズ。
	//			頂点レイアウト(1つの頂点の構成を示す設計図のようなもの。第四引数がnullptrだった場合はこれを参照して頂点定義を作成する)。
	//			インデックスバッファのサイズ。
	//			インデックスバッファのフォーマット。
	void Initialize(float PrimitiveSize_W, float PrimitiveSize_H, const D3DXVECTOR4& uv,EType, int, int, const D3DVERTEXELEMENT9*, int, D3DFORMAT);

	// プリミティブ生成関数。
	// 引数:	インデックスバッファの形式。
	//			頂点数。
	//			1つの頂点のサイズ。
	//			頂点レイアウト(1つの頂点の構成を示す設計図のようなもの。第四引数がnullptrだった場合はこれを参照して頂点定義を作成する)。
	//			インデックスバッファのサイズ。
	//			インデックスバッファのフォーマット。
	void Create(EType, int, int, const D3DVERTEXELEMENT9*, int, D3DFORMAT);

	// 板ポリ生成関数。
	// 引数:	板ポリサイズ横。
	//			板ポリサイズ縦。
	//			uv(xが左上のU、yが左上のV、zが右下のU、wが右下のV)。
	void CreatePorygon(float PrimitiveSize_W, float PrimitiveSize_H, const D3DXVECTOR4& uv);

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
	// 頂点定義の取得
	IDirect3DVertexDeclaration9* GetVertexDecl() {
		return m_pVertexDecl;
	}

private:
	// 頂点バッファ生成。
	void VertexBufferCreate(int, int);
	// インデックスバッファ生成。
	void IndexBufferCreate(int, D3DFORMAT);
	// 頂点定義作成。
	void VertexDeclCreate(LPDIRECT3DVERTEXDECLARATION9 decl, const D3DVERTEXELEMENT9* element);

	// 頂点バッファにデータを転送。
	void CopyVertexData(const void* data);
	// インデックスバッファにデータを転送。
	void CopyIndexData(const void* data);

private:
	int m_numVertex;	// 頂点バッファの要素数。
	int m_vertexStride;	// 頂点バッファの一要素のサイズ。
	int m_numIndex;	// インデックスバッファの要素数。
	int m_indexStride;	// インデックスバッファの一要素のサイズ。
	int m_numPolygon;
	LPDIRECT3DVERTEXBUFFER9 m_vertexBuffer = nullptr;
	LPDIRECT3DINDEXBUFFER9 m_indexBuffer = nullptr;
	EType m_type;
	D3DPRIMITIVETYPE m_d3dPrimitiveType;
	IDirect3DVertexDeclaration9* m_pVertexDecl = nullptr;
};

