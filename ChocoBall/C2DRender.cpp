#include "stdafx.h"
#include "C2DRender.h"
#include "Effect.h"
#include "GraphicsDevice.h"
#include "Model.h"

C2DRender::C2DRender()
{
	m_pVertexBuffer = nullptr;
}


C2DRender::~C2DRender()
{
	SAFE_RELEASE(m_pVertexBuffer);
}

void C2DRender::Initialize(){
	m_pEffect = SINSTANCE(CEffect)->SetEffect(_T("Shader/2DShader.hlsl"));	// �g�p����shader�t�@�C�����w��(�f�t�H���g)
	m_hWorld = m_pEffect->GetParameterByName(nullptr, "World");
	m_hSplit_X = m_pEffect->GetParameterByName(nullptr, "Split_X");
	m_hSplit_Y = m_pEffect->GetParameterByName(nullptr, "Split_Y");
	m_hNowCol = m_pEffect->GetParameterByName(nullptr, "NowCol");
	m_hNowRow = m_pEffect->GetParameterByName(nullptr, "NowRow");
	m_hRatio_X = m_pEffect->GetParameterByName(nullptr, "Ratio_X");
	m_hRatio_Y = m_pEffect->GetParameterByName(nullptr, "Ratio_Y");
	m_hTexture = m_pEffect->GetParameterByName(nullptr, "g_Texture");
	m_hAlpha = m_pEffect->GetParameterByName(nullptr, "Alpha");

	SVertex vertices[] = {
		{ -1.0f, -1.0f, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f, },
		{ -1.0f, 1.0f, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f, },
		{ 1.0f, -1.0f, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f, },
		{ 1.0f, -1.0f, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f, },
		{ -1.0f, 1.0f, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f, },
		{ 1.0f, 1.0f, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f, },
	};

	(*graphicsDevice()).CreateVertexBuffer(6 * sizeof(SVertex), 0, FVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL);
	VOID* pVertices;
	m_pVertexBuffer->Lock(0, sizeof(vertices), (void**)&pVertices, 0);
	memcpy(pVertices, vertices, sizeof(vertices));
	m_pVertexBuffer->Unlock();
}

void C2DRender::Draw(){

	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pEffect->SetTechnique(m_pTechniqueName);
	m_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	m_pEffect->BeginPass(0);	//�p�X�̔ԍ����w�肵�Ăǂ̃p�X���g�p���邩�w��


	//SINSTANCE(CRenderContext)->GetCurrentCamera()->SetCamera(m_pEffect);
	//�����ŌŒ�`��Ɠ����悤�ɁA���[�J�����W�ɐݒ肳�ꂽ���_�Q���f�o�C�X�ɓn���B�ʏ�Ɠ������@�B
	//	���b�V�����������A�}�e���A����e�N�X�`����ݒ�
	//DrawSubset()���Ăяo���ĕ`��

	(*graphicsDevice()).SetStreamSource(0, m_pVertexBuffer, 0, sizeof(SVertex));
	(*graphicsDevice()).SetFVF(FVF_CUSTOMVERTEX);
	D3DXVECTOR2 split = m_pModel->GetSplit();
	m_pEffect->SetMatrix(m_hWorld, &(m_pModel->m_World));
	m_pEffect->SetInt(m_hSplit_X, static_cast<int>(split.x));
	m_pEffect->SetInt(m_hSplit_Y, static_cast<int>(split.y));
	D3DXVECTOR2 Now = m_pModel->GetNow();
	m_pEffect->SetInt(m_hNowCol, static_cast<int>(Now.x));
	m_pEffect->SetInt(m_hNowRow, static_cast<int>(Now.y));

	float ratio_X = m_pModel->GetImage_2D()->RealSize.x / m_pModel->GetImage_2D()->UnRealSize.x;
	float ratio_Y = m_pModel->GetImage_2D()->RealSize.y / m_pModel->GetImage_2D()->UnRealSize.y;

	m_pEffect->SetFloat(m_hRatio_X, ratio_X);
	m_pEffect->SetFloat(m_hRatio_Y, ratio_Y);

	m_pEffect->SetTexture(m_hTexture, m_pModel->GetImage_2D()->pTex/*m_pData->m_pImage_2D->pTex*/ /*�e�N�X�`�����*/);


	m_pEffect->SetFloat(m_hAlpha, m_pModel->m_alpha);
	m_pEffect->CommitChanges();				//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B�`����s���O�Ɉ�񂾂��Ăяo���B

	(*graphicsDevice()).DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	m_pEffect->EndPass();
	m_pEffect->End();

	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}