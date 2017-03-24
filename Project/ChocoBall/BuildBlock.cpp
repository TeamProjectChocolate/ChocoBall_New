#include "stdafx.h"
#include "BuildBlock.h"
#include "ShadowRender.h"
#include "EM_SamplingRender_I.h"

int CBuildBlock::m_BBManagerNum = 0;

CBuildBlock::CBuildBlock(){
	strcpy(m_pRenderName, "Wall");
	m_BBManagerNum++;
}

CBuildBlock::~CBuildBlock(){
	m_BBManagerNum--;
}

void CBuildBlock::Initialize(D3DVECTOR pos, D3DXQUATERNION rot)
{
	// プレイヤーのインスタンス取得。
	CPlayer* Player = SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("Player"));

	for (int i = 0; i < BUILD_H; i++){
		for (int j = 0; j < BUILD_W; j++){
			D3DXVECTOR4 offset(j*BLOCK_W, 0.0f, 0.0f, 1.0f);
			D3DXMATRIX mRot;
			D3DXMatrixRotationQuaternion(&mRot, &rot);
			D3DXVec4Transform(&offset, &offset, &mRot);
			D3DXVECTOR3 _pos;
			_pos.x = pos.x + offset.x;
			_pos.y = pos.y + offset.y + i*BLOCK_H;
			_pos.z = pos.z + offset.z;

			m_blocks[i][j].Initialize(_pos, rot);
			m_blocks[i][j].SetPlayer(Player);
		}
	}
	//親子関係の構築。
	for (int i = 0; i < BUILD_W; i++){
		for (int j = 0; j < BUILD_H - 1; j++){
			m_blocks[j + 1][i].SetParent(&m_blocks[j][i]);
		}
	}

#ifdef NOT_INSTANCING
#else
	UseModel<C3DImage>();
	m_pModel->SetFileName("image/BR.x");
	CGameObject::Initialize();
	InitInstancing(m_BBManagerNum * BUILD_H * BUILD_W,false);
	SINSTANCE(CShadowRender)->Entry(this);
	m_pModel->m_alpha = 1.0f;
	m_pModel->m_luminance = 0.0f;
	m_pModel->m_Refractive = FRESNEL::g_RefractivesTable[FRESNEL::REFRACTIVES::CHOCOLATE];
#endif

	SetAlive(true);
}

void CBuildBlock::ActivateShadowRender(){
#ifdef NOT_INSTANCING
#else
	CGameObject::ActivateShadowRender();
	static_cast<CShadowSamplingRender_I*>(m_pShadowRender)->CreateMatrixBuffer(m_BBManagerNum * BUILD_H * BUILD_W);
#endif
}

void CBuildBlock::Update()
{
	for (int i = 0; i < BUILD_H; i++){
		for (int j = 0; j < BUILD_W; j++){
			if (m_blocks[i][j].IsDead()){
				continue;
			}
			if (m_blocks[i][j].GetAlive()){
				m_blocks[i][j].Update();
			}
			else
			{
				m_blocks[i][j].OnDestroy();
			}
		}
	}
}


void CBuildBlock::Draw()
{
#ifdef NOT_INSTANCING
	bool isBeginDraw = false;
	int beginDrawBlockNo_w = 0;
	int beginDrawBlockNo_h = 0;

	for (int i = 0; i < BUILD_H; i++){
		for (int j = 0; j < BUILD_W; j++){
			if (m_blocks[i][j].GetAlive()){
				if (!isBeginDraw){
					m_blocks[i][j].BeginDraw();
					isBeginDraw = true;
					beginDrawBlockNo_w = j;
					beginDrawBlockNo_h = i;

				}
				m_blocks[i][j].Draw();
			}
		}
	}
	if (isBeginDraw){
		m_blocks[beginDrawBlockNo_h][beginDrawBlockNo_w].EndDraw();
	}
#else
		m_pRender->SetModelData(m_pModel);
		SetUpTechnique();
		for (int row = 0; row < BUILD_H; row++){
			for (int col = 0; col < BUILD_W; col++){
				if (m_blocks[row][col].GetAlive()){
					static_cast<CInstancingRender*>(m_pRender)->AddWorldMatrix(m_blocks[row][col].GetModel()->GetWorldMatrix());
					static_cast<CInstancingRender*>(m_pRender)->AddRotationMatrix(m_blocks[row][col].GetModel()->GetRotation());
				}
			}
		}
#endif
}

void CBuildBlock::DrawShadow(CCamera* camera){
#ifdef NOT_INSTANCING
#else
	m_pShadowRender->SetModelData(m_pModel);
	m_pShadowRender->SetShadowCamera(camera);
	for (int row = 0; row < BUILD_H; row++){
		for (int col = 0; col < BUILD_W; col++){
			if (m_blocks[row][col].GetAlive()){
				static_cast<CShadowSamplingRender_I*>(m_pShadowRender)->AddWorldMatrix(m_blocks[row][col].GetModel()->GetWorldMatrix());
			}
		}
	}
#endif
}

void CBuildBlock::Draw_EM(CCamera* camera){
#ifdef NOT_INSTANCING
	for (int i = 0; i < BUILD_H; i++){
		for (int j = 0; j < BUILD_W; j++){
			if (m_blocks[i][j].GetAlive()){
				m_blocks[i][j].Draw_EM(camera);
			}
		}
	}
#else
		m_pEMSamplingRender->SetModelData(m_pModel);
		static_cast<CEM_SamplingRender_I*>(m_pEMSamplingRender)->SetCamera(camera);
		EM_SetUpTechnique();
		for (int row = 0; row < BUILD_H; row++){
			for (int col = 0; col < BUILD_W; col++){
				if (m_blocks[row][col].GetAlive()){
					static_cast<CEM_SamplingRender_I*>(m_pEMSamplingRender)->AddWorldMatrix(m_blocks[row][col].GetModel()->GetWorldMatrix());
					static_cast<CEM_SamplingRender_I*>(m_pEMSamplingRender)->AddRotationMatrix(m_blocks[row][col].GetModel()->GetRotation());
				}
			}
		}
#endif
}
