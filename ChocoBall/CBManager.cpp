#include "stdafx.h"
#include "Chocoball.h"
#include "CBManager.h"
#include "ShadowRender.h"
#include "InstancingRender.h"
#include "InstancingRender_D.h"
#include "ShadowSamplingRender_I.h"

int CCBManager::m_CBManagerNum = 0;

void CCBManager::Initialize()
{
	m_interval = 0.2f;
	m_timer = 0.0f;
	m_numCreate = 0;
	/*for (int i = 0; i < CHOCO_NUM; i++)
	{
	int rate = 100.0f / (rand() % 100+1);
	if (rand() % 2){
	rate *= -1.0f;
	}
	D3DXVECTOR3 pos(GetStartPosition());
	pos.z += fabsf(rate);
	pos.y += rate;
	D3DXVECTOR3 Epos(GetEndPosition());
	Epos.z += fabsf(rate);
	Epos.y += rate;
	m_Choco[i].Initialize(pos, Epos);
	}*/
	SetAlive(true);

	m_IsFirst = true;

	//SInstancingVertex vertices[] = {
	//	{ -1.0f, -1.0f, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f, },
	//	{ -1.0f, 1.0f, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f, },
	//	{ 1.0f, -1.0f, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f, },
	//	{ 1.0f, -1.0f, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f, },
	//	{ -1.0f, 1.0f, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f, },
	//	{ 1.0f, 1.0f, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f, },
	//};
	//
	//(*graphicsDevice()).CreateVertexBuffer(6 * sizeof(SInstancingVertex), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL);

	m_IsInstancing = true;
	// モデルデータは一つでいいため、個々のチョコ粒で読み込まず、ここで読み込む
	UseModel<C3DImage>();
	m_pModel->SetFileName("image/ball.x");
	CGameObject::Initialize();
	if (m_IsInstancing){
		static_cast<CInstancingRender*>(m_pRender)->CreateMatrixBuffer(m_CBManagerNum * CHOCO_NUM);
		static_cast<CInstancingRender_D*>(m_pDepthRender)->CreateMatrixBuffer(m_CBManagerNum * CHOCO_NUM);
		SINSTANCE(CShadowRender)->Entry(this);	// チョコボールはインスタンシング描画のため自身を登録する
	}

	m_pModel->m_alpha = 1.0f;
	m_pModel->m_luminance = 0.0f;
}

void CCBManager::ActivateShadowRender(){
	CGameObject::ActivateShadowRender();
	static_cast<CShadowSamplingRender_I*>(m_pShadowRender)->CreateMatrixBuffer(m_CBManagerNum * CHOCO_NUM);
}

void CCBManager::Update()
{
	//チョコボールを生成していく。
	const float deltaTime = 1.0f / 60.0f;
	m_timer += deltaTime;
	if (m_interval < m_timer){
		int createCount = 0;
		while ( m_numCreate < CHOCO_NUM ){
			if (createCount == 10){
				break;
			}
			float rate = 100.0f / (rand() % 100 + 1);
			rate /= 50.0f;
			if (rand() % 2){
				rate *= -1.0f;
			}
			D3DXVECTOR3 pos(GetStartPosition());
			pos.x += rate;
			pos.z += fabsf(rate);
			pos.y += rate;
			D3DXVECTOR3 Epos(GetEndPosition());
			Epos.x += rate;
			Epos.z += fabsf(rate);
			Epos.y += rate;
			m_Choco[m_numCreate].Initialize(pos, Epos);
			if (!m_IsInstancing){
				SINSTANCE(CShadowRender)->Entry(&m_Choco[m_numCreate]);
			}
			//if (m_IsFirst){
			//	D3DXMESHCONTAINER_DERIVED* pMeshContainer = static_cast<D3DXMESHCONTAINER_DERIVED*>(m_Choco[m_numCreate].GetImage()->pModel->GetFrameRoot()->pMeshContainer);
			//	LPDIRECT3DVERTEXBUFFER9 buffer;
			//	pMeshContainer->MeshData.pMesh->GetVertexBuffer(&buffer)/*->GetBufferPointer()*/;
			//	VOID* pVertices;
			//	buffer->Lock(0, sizeof(buffer), (void**)&pVertices, 0);
			//	D3DVERTEXBUFFER_DESC desc;
			//	buffer->GetDesc(&desc);
			//	int stride = D3DXGetFVFVertexSize(desc.FVF);
			//	char* p = (char*)pVertices;
			//	int offset = -1;
			//	D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
			//	memset(pDecl, -1, sizeof(pDecl));
			//	pMeshContainer->MeshData.pMesh->GetDeclaration(pDecl);
			//	for (int idx = 0; idx < MAX_FVF_DECL_SIZE; idx++){
			//		if (pDecl[idx].Stream == 255){
			//			//終わり
			//			break;
			//		}
			//		if (pDecl[idx].Usage == D3DDECLUSAGE_BINORMAL){
			//			offset = pDecl[idx].Offset;
			//		}
			//	}
			//	for (int vertNo = 0; vertNo < pMeshContainer->MeshData.pMesh->GetNumVertices(); vertNo++){
			//		D3DXVECTOR3* pBinomal = (D3DXVECTOR3*)&p[offset];
			//		pBinomal->x = 
			//		p += stride;
			//	}
			//	
			//	
			//	//buffer
			//	//m_pVertexBuffer->
			//	//memcpy(pVertices, buffer, sizeof(buffer));
			//	//pVertices
			//	buffer->Unlock();

			//	(*graphicsDevice()).CreateVertexBuffer(sizeof(buffer), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pVertexBuffer, nullptr);
			//	m_IsFirst = false;
			//}
			createCount++;
			m_numCreate++;
		}
	}
	for (int i = 0; i < m_numCreate; i++)
	{
		m_Choco[i].Update();
	}
}

void CCBManager::Draw()
{
	if (m_IsInstancing){
		// ObjectManagerからDraw関数が呼ばれたら、インスタンシング用のレンダーに行列データを蓄積していく
		m_pRender->SetModelData(m_pModel);
		SetUpTechnique();
		for (int i = 0; i < m_numCreate; i++)
		{
			if (m_Choco[i].GetAlive()){
				static_cast<CInstancingRender*>(m_pRender)->AddWorldMatrix(m_Choco[i].GetModel()->GetWorldMatrix());
				static_cast<CInstancingRender*>(m_pRender)->AddRotationMatrix(m_Choco[i].GetModel()->GetRotation());
			}
		}
	}
	else{
	}
}

void CCBManager::DrawDepth(const D3DXVECTOR2& FarNear, const D3DXVECTOR3& PintoPos, const D3DXMATRIX& PintoWorld){
	if (m_IsInstancing){
		m_pDepthRender->SetModelData(m_pModel);
		for (int i = 0; i < m_numCreate; i++)
		{
			if (m_Choco[i].GetAlive()){
				static_cast<CInstancingRender_D*>(m_pDepthRender)->AddWorldMatrix(m_Choco[i].GetModel()->GetWorldMatrix());
			}
		}
	}
	else{

	}
}

void CCBManager::DrawShadow(CCamera* camera){
	if (m_IsInstancing){
		m_pShadowRender->SetModelData(m_pModel);
		m_pShadowRender->SetShadowCamera(camera);
		for (int i = 0; i < m_numCreate; i++)
		{
			if (m_Choco[i].GetAlive()){
				static_cast<CShadowSamplingRender_I*>(m_pShadowRender)->AddWorldMatrix(m_Choco[i].GetModel()->GetWorldMatrix());
			}
		}
	}
	else{

	}
}

bool CCBManager::IsHit(D3DXVECTOR3 pos,D3DXVECTOR3 size)
{
	for (int i = 0; i < m_numCreate; i++){
		//プレイヤーを内包する箱の最大値と最小値を求める。
		D3DXVECTOR3 MaxSize;//最大値
		D3DXVECTOR3 MinSize;//最小値
		const static float Sphereradius = 0.65f;//チョコボールの半径
		size *= 0.5f;
		MaxSize.x = pos.x + size.x + Sphereradius;
		MaxSize.y = pos.y + size.y + Sphereradius;
		MaxSize.z = pos.z + size.z + Sphereradius;

		MinSize.x = pos.x - size.x - Sphereradius;
		MinSize.y = pos.y - size.y - Sphereradius;
		MinSize.z = pos.z - size.z - Sphereradius;
		D3DXVECTOR3 chocoPos = m_Choco[i].GetPos();
		//プレイヤーにチョコボールの半径分を足した箱内に入っている弾との当たり判定を調べる。
		if (MinSize.x < chocoPos.x&&
			MinSize.y < chocoPos.y&&
			MinSize.z < chocoPos.z&&
			chocoPos.x < MaxSize.x&&
			chocoPos.y < MaxSize.y&&
			chocoPos.z < MaxSize.z)
		{
			return TRUE;
		}
	
	}
	return FALSE;
}

void CCBManager::FindCource(){
	m_CourceDef.SetStageID(m_StageID);
	m_CourceDef.Initialize();
	m_InitPosOfCourceNo = m_CourceDef.FindCource(m_pos).blockNo;
}

void CCBManager::NonActivate(){
	if (m_numCreate <= 0){
		SetAlive(false);
		SINSTANCE(CShadowRender)->DeleteObject(this);
	}
}