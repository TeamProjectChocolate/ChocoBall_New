#include "stdafx.h"
#include "RenderContext.h"
#include "GraphicsDevice.h"
#include "BloomRender.h"
#include "Effect.h"
#include "ObjectManager.h"
#include "GameManager.h"


CRenderContext* CRenderContext::m_instance = nullptr;

void CRenderContext::Draw(CRender* pRender){
	pRender->Draw();
}

void CRenderContext::CreateRenderingTerget(){
	for (auto render : m_Renders){
		render.clear();
	}
	m_Renders.clear();

	for (int i = 0; i < RENDER::TYPE::Max; i++){
		m_Renders.push_back(vector<RENDER_DATA*>());
	}


	static PRIMITIVE::SShapeVertex_PT vertex[]{
		{
			-1.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f
		},
		{
			1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f
		},
		{
			-1.0f, -1.0f, 0.0f, 1.0f,
			0.0f, 1.0f
		},
		{
			1.0f, -1.0f, 0.0f, 1.0f,
			1.0f, 1.0f
		},
	};
	static unsigned short index[] = {
		0, 1, 2, 3
	};
	m_copyBackBufferPrim.Create(
		EType::eTriangleStrip,
		4,
		sizeof(PRIMITIVE::SShapeVertex_PT),
		nullptr,
		scShapeVertex_PT_Element,
		vertex,
		4,
		D3DFMT_INDEX16,
		index
		);

	m_bufferSize_Width = WINDOW_WIDTH * 2;
	m_bufferSize_Height = WINDOW_HEIGHT * 2;

	// ��ʊE�[�x�`��p�N���X�B
	m_DofRender = CreateRender<CDofRender>(RENDER::TYPE::Dof, _T(""),true);

	// �u���[���`��p�N���X�B
	m_BloomRender = CreateRender<CBloomRender>(RENDER::TYPE::Bloom,_T(""),true);
	
	// ���}�b�v�`��p�N���X�B
	m_EMRender = CreateRender<CEM_Render>(RENDER::TYPE::EM, _T(""), true);
	m_EMRender->SetCameraPos(D3DXVECTOR3(0.0f,0.0f,0.0f));

	// �����_�����O�^�[�Q�b�g�����B
	STargetParam param = {
		m_bufferSize_Width,
		m_bufferSize_Height,
		D3DFMT_D16,
		D3DMULTISAMPLE_NONE,
		0,
		true,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT
	};
	m_RenderTarget.CreateRenderingTarget(param);

	// �e�N�X�`�����u���[���p�����_�[�ɃZ�b�g
	m_BloomRender->SetSampleTexture(m_RenderTarget.GetTexture());
	// �e�N�X�`�����ʊE�[�x�p�����_�[�ɃZ�b�g
	m_DofRender->SetSampleTexture(m_RenderTarget.GetTexture());

	m_pEffect = SINSTANCE(CEffect)->SetEffect(_T("Shader/TransformedShader.hlsl"));
}

void CRenderContext::RenderingStart(){
#ifdef TEST_EM
	if (m_pCamera) {
		// ���݂̃J��������ݒ�B
		m_EMRender->SetCameraPos(m_pCamera->GetPos() + m_pCamera->GetDir() * 11.0f);
		// ���}�b�v�`��
		m_EMRender->Draw();
	}
#else
	// ���}�b�v�`��
	m_EMRender->Draw();
#endif

	// ���Ƃ̃����_�����O�^�[�Q�b�g��ۑ�
	(*graphicsDevice()).GetRenderTarget(0, &m_SavedBuffer);
	(*graphicsDevice()).GetDepthStencilSurface(&m_SavedMapZ);

	// �����_�����O�^�[�Q�b�g�̕ύX
	(*graphicsDevice()).SetRenderTarget(0, m_RenderTarget.GetSurface());
	(*graphicsDevice()).SetDepthStencilSurface(m_RenderTarget.GetZMap());
	(*graphicsDevice()).SetRenderTarget(1, m_DofRender->GetSurface());
	(*graphicsDevice()).Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	// �[�x���������ނƂ��ɎQ�Ƃ���s���g���v�Z
	m_DofRender->MathPinto();

	for (auto renders : m_Renders){
		for (auto render : renders){
			render->render->SetDepthFarNear(m_DofRender->GetDepthFarNear());
		}
	}
}

void CRenderContext::RenderingEnd(){
	(*graphicsDevice()).SetRenderTarget(1, nullptr);
	m_BloomRender->Draw();
	m_DofRender->Draw();
}

void CRenderContext::SetRenderingBuffer(){
	// �����_�����O�^�[�Q�b�g�����ɖ߂�
	(*graphicsDevice()).SetRenderTarget(0, m_SavedBuffer);
	(*graphicsDevice()).SetDepthStencilSurface(m_SavedMapZ);

	m_pEffect->SetTechnique("TransformedPrim");
	m_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	m_pEffect->BeginPass(0);

	float offset[] = {
		0.5f / m_bufferSize_Width,
		0.5f / m_bufferSize_Height
	};

	m_pEffect->SetValue("g_offset", offset, sizeof(offset));
	m_pEffect->SetTexture("g_tex",m_RenderTarget.GetTexture());
	m_pEffect->CommitChanges();				//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B�`����s���O�Ɉ�񂾂��Ăяo���B
	
	(*graphicsDevice()).SetVertexDeclaration(m_copyBackBufferPrim.GetVertexDecl());
	(*graphicsDevice()).SetStreamSource(0, m_copyBackBufferPrim.GetVertexBuffer(), 0, sizeof(PRIMITIVE::SShapeVertex_PT));
	(*graphicsDevice()).SetIndices(m_copyBackBufferPrim.GetIndexBuffer());

	(*graphicsDevice()).DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	m_pEffect->EndPass();
	m_pEffect->End();
}

void CRenderContext::ChangeScenedProcess(){
	this->DeleteRenders();	// �O�̃V�[���Ŏg�p���������_�[���폜���A���߂ĕK�v�ȃ����_�[��������Initialize�Ő�������
	m_EMRender->SetEnable(true);
}

void CRenderContext::DeleteRenders(){
	for (int idx = 0; idx < m_Renders.size(); idx++){
		if (m_Renders[idx].size() > 0) {
			for (auto itr = m_Renders[idx].begin(); itr != m_Renders[idx].end();) {
				if (!((*itr)->IsCommon)) {
					char text[256];
					sprintf(text, "%x\n", (*itr)->render);
					OutputDebugString(text);
					SAFE_DELETE((*itr)->render);
					SAFE_DELETE(*itr);
					itr = m_Renders[idx].erase(itr);
				}
				else {
					itr++;
				}
			}
		}
	}
	char text[256];
	sprintf(text, "�f���[�g�����B\n");
	OutputDebugString(text);
}

void CRenderContext::DeleteAll(){
	for (int idx = 0; idx < m_Renders.size(); idx++) {
		if (m_Renders[idx].size() > 0) {
			for (auto itr = m_Renders[idx].begin(); itr != m_Renders[idx].end(); itr++) {
				SAFE_DELETE((*itr)->render);
				SAFE_DELETE(*itr);
			}
			m_Renders[idx].clear();
		}
	}
	m_Renders.clear();
}