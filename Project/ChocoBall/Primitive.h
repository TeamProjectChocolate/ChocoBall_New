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

	// �|���ƃv���~�e�B�u�����֐��B
	// ����:	�|���T�C�Y���B
	//			�|���T�C�Y�c�B
	//			uv(x�������U�Ax�������V�Ax���E����U�Ax���E����V)�B
	//			�C���f�b�N�X�o�b�t�@�̌`���B
	//			���_���B
	//			1�̒��_�̃T�C�Y�B
	//			���_���C�A�E�g(1�̒��_�̍\���������݌v�}�̂悤�Ȃ��́B��l������nullptr�������ꍇ�͂�����Q�Ƃ��Ē��_��`���쐬����)�B
	//			�C���f�b�N�X�o�b�t�@�̃T�C�Y�B
	//			�C���f�b�N�X�o�b�t�@�̃t�H�[�}�b�g�B
	void Initialize(float PrimitiveSize_W, float PrimitiveSize_H, const D3DXVECTOR4& uv,EType, int, int, const D3DVERTEXELEMENT9*, int, D3DFORMAT);

	// �v���~�e�B�u�����֐��B
	// ����:	�C���f�b�N�X�o�b�t�@�̌`���B
	//			���_���B
	//			1�̒��_�̃T�C�Y�B
	//			���_���C�A�E�g(1�̒��_�̍\���������݌v�}�̂悤�Ȃ��́B��l������nullptr�������ꍇ�͂�����Q�Ƃ��Ē��_��`���쐬����)�B
	//			�C���f�b�N�X�o�b�t�@�̃T�C�Y�B
	//			�C���f�b�N�X�o�b�t�@�̃t�H�[�}�b�g�B
	void Create(EType, int, int, const D3DVERTEXELEMENT9*, int, D3DFORMAT);

	// �|�������֐��B
	// ����:	�|���T�C�Y���B
	//			�|���T�C�Y�c�B
	//			uv(x�������U�Ay�������V�Az���E����U�Aw���E����V)�B
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
	// ���_��`�̎擾
	IDirect3DVertexDeclaration9* GetVertexDecl() {
		return m_pVertexDecl;
	}

private:
	// ���_�o�b�t�@�����B
	void VertexBufferCreate(int, int);
	// �C���f�b�N�X�o�b�t�@�����B
	void IndexBufferCreate(int, D3DFORMAT);
	// ���_��`�쐬�B
	void VertexDeclCreate(LPDIRECT3DVERTEXDECLARATION9 decl, const D3DVERTEXELEMENT9* element);

	// ���_�o�b�t�@�Ƀf�[�^��]���B
	void CopyVertexData(const void* data);
	// �C���f�b�N�X�o�b�t�@�Ƀf�[�^��]���B
	void CopyIndexData(const void* data);

private:
	int m_numVertex;	// ���_�o�b�t�@�̗v�f���B
	int m_vertexStride;	// ���_�o�b�t�@�̈�v�f�̃T�C�Y�B
	int m_numIndex;	// �C���f�b�N�X�o�b�t�@�̗v�f���B
	int m_indexStride;	// �C���f�b�N�X�o�b�t�@�̈�v�f�̃T�C�Y�B
	int m_numPolygon;
	LPDIRECT3DVERTEXBUFFER9 m_vertexBuffer = nullptr;
	LPDIRECT3DINDEXBUFFER9 m_indexBuffer = nullptr;
	EType m_type;
	D3DPRIMITIVETYPE m_d3dPrimitiveType;
	IDirect3DVertexDeclaration9* m_pVertexDecl = nullptr;
};

