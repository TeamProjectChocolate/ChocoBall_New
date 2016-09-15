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

void CPrimitive::Create(EType primitiveType,
	int numVertex,
	int vertexStride,
	const D3DVERTEXELEMENT9* vertexLayout,
	void* pSrcVertexBuffer,
	int numIndex,
	D3DFORMAT indexFormat,
	void* pSrcIndexBuffer){

	CH_ASSERT(primitiveType < eTypeNum);
	CH_ASSERT(numVertex != 0);
	CH_ASSERT(vertexStride != 0);
	CH_ASSERT(numIndex != 0);
	m_type = primitiveType;
	m_numVertex = numVertex;
	m_vertexStride = vertexStride;
	m_numIndex = numIndex;
	Release();
	VertexBufferCreate(numVertex, vertexStride, vertexLayout, pSrcVertexBuffer);
	IndexBufferCreate(numIndex, indexFormat, pSrcIndexBuffer);
	if (primitiveType == eTriangleList){
		m_numPolygon = numIndex / 3;
		m_d3dPrimitiveType = D3DPT_TRIANGLELIST;
	}
	else if (primitiveType == eTriangleStrip){
		m_numPolygon = numIndex - 2;
		m_d3dPrimitiveType = D3DPT_TRIANGLESTRIP;
	}
}

void CPrimitive::Release(){
	SAFE_RELEASE(m_vertexBuffer);
	SAFE_RELEASE(m_indexBuffer);
}

void CPrimitive::VertexBufferCreate(int numVertex, int stride, const D3DVERTEXELEMENT9* vertexLayout, const void* pSrcVertexBuffer){
	LPDIRECT3DVERTEXBUFFER9 pVB;
	int l_numVertex = numVertex;
	int l_stride = stride;
	int l_size = l_numVertex * l_stride;
	LPDIRECT3DDEVICE9 d3dDevice = graphicsDevice();
	HRESULT hr = d3dDevice->CreateVertexBuffer(numVertex * stride, 0, 0, D3DPOOL_DEFAULT, &pVB, nullptr);
	CH_ASSERT(SUCCEEDED(hr));
	if (pSrcVertexBuffer != nullptr){
		// ソースが指定されている
		// 頂点バッファをロックしてコピー
		void* pDstVertexBuffer;
		hr = pVB->Lock(0, 0, &pDstVertexBuffer, D3DLOCK_DISCARD);
		CH_ASSERT(SUCCEEDED(hr));
		// まるっとコピー
		memcpy(pDstVertexBuffer, pSrcVertexBuffer, l_size);
		pVB->Unlock();
	}
	m_vertexBuffer = pVB;
	//頂点定義を作成。
	d3dDevice->CreateVertexDeclaration(vertexLayout, &m_pVertexDecl);
}

void CPrimitive::IndexBufferCreate(int numIndex, D3DFORMAT format, const void* pSrcIndexBuffer){
	LPDIRECT3DINDEXBUFFER9 pIB;
	int size = 0;
	D3DFORMAT d3dFormat;
	if (format == D3DFMT_INDEX16) {
		d3dFormat = D3DFMT_INDEX16;
		size = numIndex * 2;
	}
	else if (format == D3DFMT_INDEX32) {
		d3dFormat = D3DFMT_INDEX32;
		size = numIndex * 4;
	}
	LPDIRECT3DDEVICE9 d3dDevice = graphicsDevice();
	HRESULT hr = d3dDevice->CreateIndexBuffer(
		size,
		0,
		(D3DFORMAT)d3dFormat,
		D3DPOOL_DEFAULT,
		&pIB,
		nullptr
		);
	CH_ASSERT(SUCCEEDED(hr));
	if (pSrcIndexBuffer != nullptr) {
		//ソースが指定されている。
		//インデックスバッファをロックしてコピー。
		void* pDstIndexBuffer;
		hr = pIB->Lock(0, 0, &pDstIndexBuffer, D3DLOCK_DISCARD);
		CH_ASSERT(SUCCEEDED(hr));
		//まるっとコピー。
		memcpy(pDstIndexBuffer, pSrcIndexBuffer, size);
		pIB->Unlock();
	}
	m_indexBuffer = pIB;
}