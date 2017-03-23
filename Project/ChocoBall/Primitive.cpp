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
	// ���f���f�[�^�����B
	this->Create(primitiveType,numVertex,vertexStride,vertexLayout,numIndex,indexFormat);
	// �|�������B
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
	// �ʒu��񂪔|���̒��S�������悤�ɂ��������߁A�����̃T�C�Y�ɂ���B
	PrimitiveSize_W *= 0.5f;
	PrimitiveSize_H *= 0.5f;

	// ���_���쐬�B
	PRIMITIVE::SShapeVertex_PT vp[] = {
		{ D3DXVECTOR4(-PrimitiveSize_W, PrimitiveSize_H, 0.0f, 1.0f), D3DXVECTOR2(uv.x, uv.y) },	// ����B
		{ D3DXVECTOR4(PrimitiveSize_W, PrimitiveSize_H, 0.0f, 1.0f), D3DXVECTOR2(uv.z, uv.y) },		// �E��B
		{ D3DXVECTOR4(-PrimitiveSize_W, -PrimitiveSize_H, 0.0f, 1.0f), D3DXVECTOR2(uv.x, uv.w) },	// �����B
		{ D3DXVECTOR4(PrimitiveSize_W, -PrimitiveSize_H, 0.0f, 1.0f), D3DXVECTOR2(uv.z, uv.w) }		// �E���B
	};
	short index[]{
		0, 1, 2, 3
	};

	// �o�b�t�@�ɍ쐬�����f�[�^���i�[�B
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

	// ���_�o�b�t�@�쐬�B
	HRESULT hr = d3dDevice->CreateVertexBuffer(l_size, 0, 0, D3DPOOL_DEFAULT, &pVB, nullptr);
	CH_ASSERT(SUCCEEDED(hr));

	// �쐬�������_�o�b�t�@��ۑ��B
	m_vertexBuffer = pVB;

	// ���_�o�b�t�@�Ɏ󂯎�����f�[�^��]���B
	this->CopyVertexData(pSrcVertexBuffer);
}

void CPrimitive::IndexBufferCreate(int numIndex, D3DFORMAT format, const void* pSrcIndexBuffer){
	LPDIRECT3DINDEXBUFFER9 pIB;

	// �n���ꂽ�C���f�b�N�X�o�b�t�@�̃t�H�[�}�b�g��K���B
	D3DFORMAT d3dFormat;
	if (format == D3DFMT_INDEX16) {
		d3dFormat = D3DFMT_INDEX16;
		m_indexStride = numIndex * 2;
	}
	else if (format == D3DFMT_INDEX32) {
		d3dFormat = D3DFMT_INDEX32;
		m_indexStride = numIndex * 4;
	}

	// �C���f�b�N�X�o�b�t�@�����B
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

	// ���������C���f�b�N�X�o�b�t�@��ۑ��B
	m_indexBuffer = pIB;

	// �C���f�b�N�X�o�b�t�@�Ɏ󂯎�����f�[�^��]���B
	this->CopyIndexData(pSrcIndexBuffer);
}

void CPrimitive::VertexDeclCreate(LPDIRECT3DVERTEXDECLARATION9 decl, const D3DVERTEXELEMENT9* element) {
	// ���_��`���n����Ă��Ȃ���Ύ󂯎�����l�Œ��_��`�쐬�B
	if (decl == nullptr) {
		(*graphicsDevice()).CreateVertexDeclaration(element, &m_pVertexDecl);
	}
	else {
		m_pVertexDecl = decl;
	}
}

// ���_�o�b�t�@�Ƀf�[�^��]���B
void CPrimitive::CopyVertexData(const void* data) {
	if (data != nullptr) {
		// �\�[�X���w�肳��Ă���
		// ���_�o�b�t�@�����b�N���ăR�s�[
		void* pDstVertexBuffer;
		if (m_vertexBuffer) {
			HRESULT hr = m_vertexBuffer->Lock(0, 0, &pDstVertexBuffer, D3DLOCK_DISCARD);
			CH_ASSERT(SUCCEEDED(hr));
			// �܂���ƃR�s�[
			int size = m_numVertex * m_vertexStride;
			memcpy(pDstVertexBuffer, data, size);
			m_vertexBuffer->Unlock();
		}
		else {
			// �����_�o�b�t�@�������̂ɒl���i�[���悤�Ƃ��Ă��܂��B
			abort();
		}
	}
}

// �C���f�b�N�X�o�b�t�@�Ƀf�[�^��]���B
void CPrimitive::CopyIndexData(const void* data) {
	if (data != nullptr) {
		//�\�[�X���w�肳��Ă���B
		//�C���f�b�N�X�o�b�t�@�����b�N���ăR�s�[�B
		void* pDstIndexBuffer;
		if (m_indexBuffer) {
			HRESULT hr = m_indexBuffer->Lock(0, 0, &pDstIndexBuffer, D3DLOCK_DISCARD);
			CH_ASSERT(SUCCEEDED(hr));
			//�܂���ƃR�s�[�B
			int size = m_numIndex * m_indexStride;
			memcpy(pDstIndexBuffer, data, size);
			m_indexBuffer->Unlock();
		}
		else {
			// ���C���f�b�N�X�o�b�t�@�������̂ɒl���i�[���悤�Ƃ��Ă��܂��B
			abort();
		}
	}
}
