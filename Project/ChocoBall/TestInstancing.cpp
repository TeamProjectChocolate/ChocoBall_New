#include "stdafx.h"
#include "TestInstancing.h"
#include "RenderContext.h"
#include "Camera.h"
#include "ShadowRender.h"
#include "Player.h"
#include "ObjectManager.h"

#define RANDOM_MARGIN 32767.0
#define MARGIN_BAIRITU 2.0
#define RANDOM_MARGIN_BAIRITU 1000.0

CTestInstancing::CTestInstancing()
{
}


CTestInstancing::~CTestInstancing()
{
	for (D3DXMATRIX* idx : pWorlds){
		SAFE_DELETE(idx);
	}
}

void CTestInstancing::CreateMatrixBuffer(int MaxNum){
	//ワールド行列用のバッファの作成。
	if (m_WorldMatrixBuffer == nullptr){
		(*graphicsDevice()).CreateVertexBuffer(sizeof(D3DXMATRIX)* MaxNum, 0, 0, D3DPOOL_MANAGED, &m_WorldMatrixBuffer, 0);
	}
}

void CTestInstancing::CopyMatrixToVertexBuffer()
{
	D3DVERTEXBUFFER_DESC desc;
	m_WorldMatrixBuffer->GetDesc(&desc);
	D3DXMATRIX* pData;
	m_WorldMatrixBuffer->Lock(0, desc.Size, (void**)&pData, D3DLOCK_DISCARD);

	for (int i = 0; i < m_worldMatrix.size(); i++) {
		*pData = m_worldMatrix[i];
		pData++;
	}
	m_WorldMatrixBuffer->Unlock();
}

void CTestInstancing::Initialize(){
	
	model.SetFileName(_T("image/ball.x"));
	model.Initialize();
	model.m_alpha = 1.0f;
	model.m_luminance = 0.0f;

	m_pEffect = SINSTANCE(CEffect)->SetEffect(_T("Shader/ModelShader.hlsl"));	// 使用するshaderファイルを指定(デフォルト)
	m_hEyePosition = m_pEffect->GetParameterByName(nullptr, "EyePosition");
	m_hWorldMatrixArray = m_pEffect->GetParameterByName(nullptr, "g_WorldMatrixArray");
	m_hluminance = m_pEffect->GetParameterByName(nullptr, "g_luminance");
	m_hnumBone = m_pEffect->GetParameterByName(nullptr, "g_numBone");
	m_hAlpha = m_pEffect->GetParameterByName(nullptr, "Alpha");
	m_hRota = m_pEffect->GetParameterByName(nullptr, "Rota");
	m_hWorld = m_pEffect->GetParameterByName(nullptr, "World");
	m_hTexture = m_pEffect->GetParameterByName(nullptr, "g_Texture");
	m_hShadowMap = m_pEffect->GetParameterByName(nullptr, "g_ShadowMap");

	//インスタンシング描画用の初期化。
	D3DVERTEXELEMENT9 declElement[MAX_FVF_DECL_SIZE];
	ANIMATION::D3DXMESHCONTAINER_DERIVED* container = model.GetImage_3D()->GetContainer();
	// 現在の頂点宣言オブジェクトを取得(※頂点宣言オブジェクトとは、頂点データがどのような構成になっているかを宣言したものである)
	container->MeshData.pMesh->GetDeclaration(declElement);
	// 頂点の宣言の終端を探索
	int elementIndex = 0;
	while (true) {
		if (declElement[elementIndex].Type == D3DDECLTYPE_UNUSED) {
			//終端を発見。
			//ここからインスタンシング用の頂点レイアウトを埋め込む。
			WORD offset = 0;
			
			for (BYTE num = 0; num < 4; num++){
				// WORLD行列はfloat4x4なので、1行ずつfloat4で定義していく
				declElement[elementIndex] = {
					1,	// ストリーム番号
					offset,	/*offset要素(各ストリームの先頭から何番目に定義されているかを表す数字[sizeof(型名) * 数字]の書き方)*/
					D3DDECLTYPE_FLOAT4,	// 頂点情報をどの肩を使用して宣言するか(行列はfloat4の要素を四つ使用して構成する)
					D3DDECLMETHOD_DEFAULT, // テッセレーション(ポリゴン分割)の方法を指定。普通テッセレーションはあまり使わないため、デフォルト設(D3DDECLMETHOD_DEFAULT)で十分
					D3DDECLUSAGE_TEXCOORD,	// usage要素
					static_cast<BYTE>(num + 1) /*UsageIndex要素(Usageが重複しているものについて、固有番号を振って識別するもの)*/
				};
				offset += sizeof(float)* 4;
				elementIndex++;
			}
			declElement[elementIndex] = D3DDECL_END();	// 頂点宣言の終端(番兵)を設定(正確には、頂点宣言の最後の要素を初期化している)
			break;
		}
		elementIndex++;
	}
	//上で宣言した頂点情報で頂点定義を作成。
	(*graphicsDevice()).CreateVertexDeclaration(declElement, &m_VertexDecl);
	this->CreateMatrixBuffer(MAX_INSTANCE);

	for (int idx = 0; idx < MAX_INSTANCE; idx++){
		D3DXMATRIX* pWorldMat = new D3DXMATRIX;
		D3DXMatrixIdentity(pWorldMat);
		m_worldMatrix.push_back(*pWorldMat);
		pWorlds.push_back(pWorldMat);
		float rndom = (static_cast<float>(rand()) / RANDOM_MARGIN) * MARGIN_BAIRITU;
		margin[idx].x = rndom - (1.0f * MARGIN_BAIRITU / 2.0f);
		rndom = (static_cast<float>(rand()) / RANDOM_MARGIN) * MARGIN_BAIRITU;
		margin[idx].y = rndom - (1.0f * MARGIN_BAIRITU / 2.0f);
		rndom = (static_cast<float>(rand()) / RANDOM_MARGIN) * MARGIN_BAIRITU;
		margin[idx].z = rndom - (1.0f * MARGIN_BAIRITU/ 2.0f);

		rnd[idx] = D3DXVECTOR3(0.0f,0.0f,0.0f);
		TargetRnd[idx] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		flg[idx] = false;
	}

	m_TimeCounter = 0.0f;
	m_IntervalTime = 1.0f;
}

void CTestInstancing::Update(){
	CPlayer* player = SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("Player"));
	if (player == nullptr){
		return;
	}
	D3DXVECTOR3 position = player->GetPos();

	for (int idx = 0; idx < MAX_INSTANCE; idx++){
		m_worldMatrix[idx].m[3][0] = position.x;
		m_worldMatrix[idx].m[3][1] = position.y;
		m_worldMatrix[idx].m[3][2] = position.z;
		m_worldMatrix[idx].m[3][3] = 1.0f;

		if (flg[idx]){
			if (D3DXVec3Length(&rnd[idx]) <= 0.0001f){
				rnd[idx] = D3DXVECTOR3(0.0f,0.0f,0.0f);
				flg[idx] = false;
				m_TimeCounter = 0.0f;
			}
			else{
				rnd[idx] -= rnd[idx] / 10.0f;
			}
		}
		else if (m_IntervalTime <= m_TimeCounter){
			if (rnd[idx].x == 0.0f && rnd[idx].y == 0.0f && rnd[idx].z == 0.0f){
				TargetRnd[idx].x = (static_cast<float>(rand()) / RANDOM_MARGIN) * RANDOM_MARGIN_BAIRITU;
				TargetRnd[idx].x = TargetRnd[idx].x - (1.0f * RANDOM_MARGIN_BAIRITU / 2.0f);
				rnd[idx].x = TargetRnd[idx].x / 10.0f;
				TargetRnd[idx].y = (static_cast<float>(rand()) / RANDOM_MARGIN) * RANDOM_MARGIN_BAIRITU;
				TargetRnd[idx].y = TargetRnd[idx].y - (1.0f * RANDOM_MARGIN_BAIRITU / 2.0f);
				rnd[idx].y = TargetRnd[idx].y / 10.0f;
				TargetRnd[idx].z = (static_cast<float>(rand()) / RANDOM_MARGIN) * RANDOM_MARGIN_BAIRITU;
				TargetRnd[idx].z = TargetRnd[idx].z - (1.0f * RANDOM_MARGIN_BAIRITU / 2.0f);
				rnd[idx].z = TargetRnd[idx].z / 10.0f;
			}
			else{
				if (fabsf(D3DXVec3Length(&TargetRnd[idx])) < fabsf(D3DXVec3Length(&rnd[idx]))){
					rnd[idx] = TargetRnd[idx];
					TargetRnd[idx] = D3DXVECTOR3(0.0f,0.0f,0.0f);
					flg[idx] = true;
				}
				else{
					rnd[idx] += rnd[idx] / 10.0f;
				}
			}
		}

		m_worldMatrix[idx].m[3][0] += margin[idx].x + (rnd[idx].x * 0.5f);
		m_worldMatrix[idx].m[3][1] += margin[idx].y + (rnd[idx].y * 0.5f);
		m_worldMatrix[idx].m[3][2] += margin[idx].z + (rnd[idx].z * 0.5f);
	}
	m_TimeCounter += 1.0f / 60.0f;
}


void CTestInstancing::Draw(){
	ANIMATION::D3DXMESHCONTAINER_DERIVED* container = model.GetImage_3D()->GetContainer();

	(*graphicsDevice()).SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pEffect->SetTechnique("NonAnimationInstancing_SFresnel");
	m_pEffect->Begin(nullptr, D3DXFX_DONOTSAVESHADERSTATE);
	m_pEffect->BeginPass(0);	//パスの番号を指定してどのパスを使用するか指定

	//// 透明度有効化
	//(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	//(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 現在のプロジェクション行列とビュー行列をシェーダーに転送
	CCamera* camera = SINSTANCE(CRenderContext)->GetCurrentCamera();
	if (camera){
		camera->SetCamera(m_pEffect);
	}
	CLight* light = SINSTANCE(CRenderContext)->GetCurrentLight();
	if (light){
		light->SetLight(m_pEffect);
	}
	D3DXVECTOR3 pos;
	if (camera){
		pos = camera->GetPos();
	}
	else{
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	// 視点をシェーダーに転送
	m_pEffect->SetVector(m_hEyePosition, reinterpret_cast<D3DXVECTOR4*>(&pos));


	SINSTANCE(CShadowRender)->SetShadowCamera(m_pEffect);

	m_pEffect->SetFloat(m_hAlpha, model.m_alpha);
	m_pEffect->SetFloat(m_hluminance, model.m_luminance);
	m_pEffect->SetTexture(m_hShadowMap, SINSTANCE(CShadowRender)->GetTexture());	// テクスチャ情報をセット

	m_pEffect->SetTexture(m_hTexture, container->ppTextures[0]);	// テクスチャ情報をセット

	// ワールドトランスフォーム(絶対座標変換)
	// ワールド行列生成


	// インスタンシング描画
	{
		//DrawSubsetを使用するとインスタンシング描画が行えないので
		//g_pMeshから頂点バッファ、インデックスバッファを引っ張ってきて、直接描画する。
		LPDIRECT3DVERTEXBUFFER9 vb;
		LPDIRECT3DINDEXBUFFER9 ib;
		container->MeshData.pMesh->GetVertexBuffer(&vb);
		container->MeshData.pMesh->GetIndexBuffer(&ib);

		DWORD fvf = container->MeshData.pMesh->GetFVF();
		DWORD stride = D3DXGetFVFVertexSize(fvf);

		(*graphicsDevice()).SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | MAX_INSTANCE);
		(*graphicsDevice()).SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);

		(*graphicsDevice()).SetVertexDeclaration(m_VertexDecl);

		(*graphicsDevice()).SetStreamSource(0, vb, 0, stride);							//頂点バッファをストリーム0番目に設定
		(*graphicsDevice()).SetStreamSource(1, m_WorldMatrixBuffer, 0, sizeof(D3DXMATRIX));	//ワールド行列用のバッファをストリーム1番目に設定。
		//ワールド行列を頂点バッファにコピー。
		CopyMatrixToVertexBuffer();
		(*graphicsDevice()).SetIndices(ib);
		m_pEffect->CommitChanges();						//この関数を呼び出すことで、データの転送が確定する。描画を行う前に一回だけ呼び出す。
		(*graphicsDevice()).DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, container->MeshData.pMesh->GetNumVertices(), 0, container->MeshData.pMesh->GetNumFaces());
		vb->Release();
		ib->Release();
	}
	m_pEffect->EndPass();
	m_pEffect->End();
}