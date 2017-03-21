#pragma once
#include "Render.h"
class CShadowSamplingRender_I :
	public CRender
{
public:
	CShadowSamplingRender_I();
	~CShadowSamplingRender_I();
	void CreateMatrixBuffer(unsigned int);	// �l������ő吔���̃��[���h�s��i�[�p�o�b�t�@���`����֐�
	void AddWorldMatrix(const D3DXMATRIX&);
	void Initialize()override;
	void Draw()override;
private:
	/*!
	*@brief	���[���h�s��𒸓_�o�b�t�@�ɃR�s�[�B
	*/
	void CopyMatrixToVertexBuffer();
	void DrawFrame(LPD3DXFRAME);
	void DrawMeshContainer(LPD3DXMESHCONTAINER, LPD3DXFRAME);
	void AnimationDraw(ANIMATION::D3DXMESHCONTAINER_DERIVED*, ANIMATION::D3DXFRAME_DERIVED*);
	void NonAnimationDraw(ANIMATION::D3DXFRAME_DERIVED*);
	inline void SetShadowCamera(CCamera* camera)override{
		//m_pShadowCamera = camera;
	}
private:
	bool m_UseBorn;
	bool m_IsFirst;
	// �C���X�^���V���O�`��p�ϐ�
	unique_ptr<IDirect3DVertexBuffer9> m_WorldMatrixBuffer;			//���[���h�s��̃o�b�t�@�B
	unique_ptr<IDirect3DVertexDeclaration9> m_VertexDecl;			//���_��`�B
	vector<D3DXMATRIX> m_worldMatrix;	// ���[���h�s��̔z��
	CCamera* m_pShadowCamera = nullptr;
	unsigned int m_MaxNum = 0;
};

