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
	// �v���~�e�B�u�����֐��B
	// ����:	�C���f�b�N�X�o�b�t�@�̌`���B
	//			���_���B
	//			1�̒��_�̃T�C�Y�B
	//			���_��`(�O�����璸�_��`���w�肷��ꍇ�͂����ɓ����Bnullptr������ƃN���X���Ő��������)�B
	//			���_���C�A�E�g(1�̒��_�̍\���������݌v�}�̂悤�Ȃ��́B��l������nullptr�������ꍇ�͂�����Q�Ƃ��Ē��_��`���쐬����)�B
	//			���_�o�b�t�@�ɓn�����_���B
	//			�C���f�b�N�X�o�b�t�@�̃T�C�Y�B
	//			�C���f�b�N�X�o�b�t�@�̃t�H�[�}�b�g�B
	//			�C���f�b�N�X�o�b�t�@�ɐݒ肷��l�B
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
		// ���_��`�̎擾
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

