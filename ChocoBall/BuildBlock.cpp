#include "stdafx.h"
#include "BuildBlock.h"
#include "ShadowRender.h"

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
		}
	}
	//eŽqŠÖŒW‚Ì\’zB
	for (int i = 0; i < BUILD_W; i++){
		for (int j = 0; j < BUILD_H - 1; j++){
			m_blocks[j + 1][i].SetParent(&m_blocks[j][i]);
		}
	}

	m_IsInstancing = true;

	UseModel<C3DImage>();
	m_pModel->SetFileName("image/BR.x");
	CGameObject::Initialize();

	if (m_IsInstancing){
		static_cast<CInstancingRender*>(m_pRender)->CreateMatrixBuffer(m_BBManagerNum * BUILD_H * BUILD_W);
		SINSTANCE(CShadowRender)->Entry(this);
	}

	m_pModel->m_alpha = 1.0f;
	m_pModel->m_luminance = 0.0f;
	m_pModel->m_Refractive = 0.34f;

	SetAlive(true);
}

void CBuildBlock::ActivateShadowRender(){
	CGameObject::ActivateShadowRender();
	static_cast<CShadowSamplingRender_I*>(m_pShadowRender)->CreateMatrixBuffer(m_BBManagerNum * BUILD_H * BUILD_W);
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
	if (m_IsInstancing){
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
	}
	else{
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
	}
}

void CBuildBlock::DrawShadow(CCamera* camera){
	m_pShadowRender->SetModelData(m_pModel);
	m_pShadowRender->SetShadowCamera(camera);
	for (int row = 0; row < BUILD_H; row++){
		for (int col = 0; col < BUILD_W; col++){
			if (m_blocks[row][col].GetAlive()){
				static_cast<CShadowSamplingRender_I*>(m_pShadowRender)->AddWorldMatrix(m_blocks[row][col].GetModel()->GetWorldMatrix());
			}
		}
	}
}