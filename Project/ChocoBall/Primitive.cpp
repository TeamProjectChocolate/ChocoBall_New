#include "stdafx.h"
#include "Primitive.h"
#include "ParticleEmitter.h"


CPrimitive::CPrimitive()
{
	m_numVertex = 0;
	m_numIndex = 0;
	m_vertexStride = 0;
	m_type = eTriangleList;
	m_d3dPrimitiveType = D3DPT_TRIANGLELIST;
}


CPrimitive::~CPrimitive()
{
	Release();
}

void CPrimitive::Initialize(
	float PrimitiveSize_W,
	float PrimitiveSize_H, 
	const D3DXVECTOR4& uv, 
	EType primitiveType,
	int numVertex,
	int vertexStride,
	const D3DVERTEXELEMENT9* vertexLayout,
	int numIndex,
	D3DFORMAT indexFormat) 
{
	// モデルデータ生成。
	this->Create(primitiveType,numVertex,vertexStride,vertexLayout,numIndex,indexFormat);
	// 板ポリ生成。
	this->CreatePorygon(PrimitiveSize_W,PrimitiveSize_H,uv);
}


void CPrimitive::Create(
	EType primitiveType,
	int numVertex,
	int vertexStride,
	const D3DVERTEXELEMENT9* vertexLayout,
	int numIndex,
	D3DFORMAT indexFormat) {

	CH_ASSERT(primitiveType < eTypeNum);
	CH_ASSERT(numVertex != 0);
	CH_ASSERT(vertexStride != 0);
	CH_ASSERT(numIndex != 0);
	m_type = primitiveType;
	m_numVertex = numVertex;
	m_vertexStride = vertexStride;
	m_numIndex = numIndex;

	//Release();
	VertexBufferCreate(m_numVertex, m_vertexStride, nullptr);
	IndexBufferCreate(m_numIndex, indexFormat, nullptr);
	VertexDeclCreate(nullptr, vertexLayout);
	if (m_type == eTriangleList) {
		m_numPolygon = m_numIndex / 3;
		m_d3dPrimitiveType = D3DPT_TRIANGLELIST;
	}
	else if (m_type == eTriangleStrip) {
		m_numPolygon = m_numIndex - 2;
		m_d3dPrimitiveType = D3DPT_TRIANGLESTRIP;
	}
}

void CPrimitive::CreatePorygon(float PrimitiveSize_W, float PrimitiveSize_H, const D3DXVECTOR4& uv) {
	// 位置情報が板ポリの中心をさすようにしたいため、半分のサイズにする。
	PrimitiveSize_W *= 0.5f;
	PrimitiveSize_H *= 0.5f;

	// 頂点情報作成。
	PRIMITIVE::SShapeVertex_PT vp[] = {
		{ D3DXVECTOR4(-PrimitiveSize_W, PrimitiveSize_H, 0.0f, 1.0f), D3DXVECTOR2(uv.x, uv.y) },	// 左上。
		{ D3DXVECTOR4(PrimitiveSize_W, PrimitiveSize_H, 0.0f, 1.0f), D3DXVECTOR2(uv.z, uv.y) },		// 右上。
		{ D3DXVECTOR4(-PrimitiveSize_W, -PrimitiveSize_H, 0.0f, 1.0f), D3DXVECTOR2(uv.x, uv.w) },	// 左下。
		{ D3DXVECTOR4(PrimitiveSize_W, -PrimitiveSize_H, 0.0f, 1.0f), D3DXVECTOR2(uv.z, uv.w) }		// 右下。
	};
	short index[]{
		0, 1, 2, 3
	};

	// バッファに作成したデータを格納。
	int numVertex = 4;
	int numIndex = 4;
	this->CopyVertexData(vp);
	this->CopyIndexData(index);
}

void CPrimitive::Release() {
	if (m_vertexBuffer) {
		m_vertexBuffer->Release();
	}
	if (m_indexBuffer) {
		m_indexBuffer->Release();
	}
}

void CPrimitive::VertexBufferCreate(int numVertex, int stride,const void* pSrcVertexBuffer){
	LPDIRECT3DVERTEXBUFFER9 pVB;

	int l_size = numVertex * stride;

	LPDIRECT3DDEVICE9 d3dDevice = graphicsDevice();

	// 頂点バッファ作成。
	HRESULT hr = d3dDevice->CreateVertexBuffer(l_size, 0, 0, D3DPOOL_DEFAULT, &pVB, nullptr);
	CH_ASSERT(SUCCEEDED(hr));

	// 作成した頂点バッファを保存。
	m_vertexBuffer = pVB;

	// 頂点バッファに受け取ったデータを転送。
	this->CopyVertexData(pSrcVertexBuffer);
}

void CPrimitive::IndexBufferCreate(int numIndex, D3DFORMAT format, const void* pSrcIndexBuffer){
	LPDIRECT3DINDEXBUFFER9 pIB;

	// 渡されたインデックスバッファのフォーマットを適応。
	D3DFORMAT d3dFormat;
	if (format == D3DFMT_INDEX16) {
		d3dFormat = D3DFMT_INDEX16;
		m_indexStride = numIndex * 2;
	}
	else if (format == D3DFMT_INDEX32) {
		d3dFormat = D3DFMT_INDEX32;
		m_indexStride = numIndex * 4;
	}

	// インデックスバッファ生成。
	LPDIRECT3DDEVICE9 d3dDevice = graphicsDevice();
	HRESULT hr = d3dDevice->CreateIndexBuffer(
		m_indexStride,
		0,
		(D3DFORMAT)d3dFormat,
		D3DPOOL_DEFAULT,
		&pIB,
		nullptr
		);
	CH_ASSERT(SUCCEEDED(hr));

	// 生成したインデックスバッファを保存。
	m_indexBuffer = pIB;

	// インデックスバッファに受け取ったデータを転送。
	this->CopyIndexData(pSrcIndexBuffer);
}

void CPrimitive::VertexDeclCreate(LPDIRECT3DVERTEXDECLARATION9 decl, const D3DVERTEXELEMENT9* element) {
	// 頂点定義が渡されていなければ受け取った値で頂点定義作成。
	if (decl == nullptr) {
		(*graphicsDevice()).CreateVertexDeclaration(element, &m_pVertexDecl);
	}
	else {
		m_pVertexDecl = decl;
	}
}

// 頂点バッファにデータを転送。
void CPrimitive::CopyVertexData(const void* data) {
	if (data != nullptr) {
		// ソースが指定されている
		// 頂点バッファをロックしてコピー
		void* pDstVertexBuffer;
		if (m_vertexBuffer) {
			HRESULT hr = m_vertexBuffer->Lock(0, 0, &pDstVertexBuffer, D3DLOCK_DISCARD);
			CH_ASSERT(SUCCEEDED(hr));
			// まるっとコピー
			int size = m_numVertex * m_vertexStride;
			memcpy(pDstVertexBuffer, data, size);
			m_vertexBuffer->Unlock();
		}
		else {
			// ※頂点バッファが無いのに値を格納しようとしています。
			abort();
		}
	}
}

// インデックスバッファにデータを転送。
void CPrimitive::CopyIndexData(const void* data) {
	if (data != nullptr) {
		//ソースが指定されている。
		//インデックスバッファをロックしてコピー。
		void* pDstIndexBuffer;
		if (m_indexBuffer) {
			HRESULT hr = m_indexBuffer->Lock(0, 0, &pDstIndexBuffer, D3DLOCK_DISCARD);
			CH_ASSERT(SUCCEEDED(hr));
			//まるっとコピー。
			int size = m_numIndex * m_indexStride;
			memcpy(pDstIndexBuffer, data, size);
			m_indexBuffer->Unlock();
		}
		else {
			// ※インデックスバッファが無いのに値を格納しようとしています。
			abort();
		}
	}
}
