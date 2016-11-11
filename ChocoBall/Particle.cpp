#include "stdafx.h"
#include "Particle.h"
#include "ObjectManager.h"
#include "ParticleEmitter.h"
#include "RenderContext.h"
#include "C2DRender.h"


CParticle::CParticle()
{
	m_applyFource = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	strcpy(m_pFileName,"image/test.png");
}

CParticle::~CParticle()
{
	m_Primitive.Release();
}

void CParticle::Initialize(){
}

void CParticle::Update(){
	m_timer += m_deltaTime;
	D3DXVECTOR3 addGravity = m_ParticleData.gravity;
	addGravity *= m_deltaTime;
	m_ParticleData.velocity += addGravity;
	D3DXVECTOR3 force = m_applyFource;
	force.x += (static_cast<float>(m_random->GetRandDouble() - 0.5f) * 2.0f) * m_addVelocityRandomMargin.x;
	force.y += (static_cast<float>(m_random->GetRandDouble() - 0.5f) * 2.0f) * m_addVelocityRandomMargin.y;
	force.z += (static_cast<float>(m_random->GetRandDouble() - 0.5f) * 2.0f) * m_addVelocityRandomMargin.z;
	force *= m_deltaTime;
	D3DXVECTOR3 addPos = m_ParticleData.velocity;
	addPos *= m_deltaTime;
	m_applyFource = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_ParticleData.position += addPos;
}

void CParticle::Draw(){
	SetupMatrices();

	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	// アルファテストを有効化
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	// アルファの値が一定値より大きければ合格とし、描画する。小さければピクセルが破棄される
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	// アルファテストの境界値
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHAREF, 0x1);

	(*graphicsDevice()).SetRenderState(D3DRS_ZENABLE, true);
	(*graphicsDevice()).SetRenderState(D3DRS_ZWRITEENABLE, false);


	switch (m_alphaBlendMode){
	case 0:
		(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
	case 1:
		(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		(*graphicsDevice()).SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, true);
		(*graphicsDevice()).SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
		(*graphicsDevice()).SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ZERO);
		break;
	}


	//SINSTANCE(CRenderContext)->GetCurrentCamera()->SetCamera(m_pEffect);
	//ここで固定描画と同じように、ローカル座標に設定された頂点群をデバイスに渡す。通常と同じ方法。
	//	メッシュも同じく、マテリアルやテクスチャを設定
	//DrawSubset()を呼び出して描画

	(*graphicsDevice()).SetStreamSource(0, m_Primitive.GetVertexBuffer(), 0, sizeof(SShapeVertex_PT));
	(*graphicsDevice()).SetIndices(m_Primitive.GetIndexBuffer());
	(*graphicsDevice()).SetVertexDeclaration(m_Primitive.GetVertexDecl());


	SetUpTechnique();
	m_pRender->GetEffect()->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	LPD3DXEFFECT work = m_pRender->GetEffect();
	work->BeginPass(0);	//パスの番号を指定してどのパスを使用するか指定

	work->SetMatrix("World", &mWorldViewProj);
	D3DXVECTOR2 split = m_pModel->GetSplit();
	work->SetInt("Split_X", split.x);
	work->SetInt("Split_Y", split.y);
	D3DXVECTOR2 now = m_pModel->GetNow();
	work->SetInt("NowCol", now.x);
	work->SetInt("NowRow", now.y);
	float ratio_X = m_pModel->GetImage_2D()->RealSize.x / m_pModel->GetImage_2D()->UnRealSize.x;
	float ratio_Y = m_pModel->GetImage_2D()->RealSize.y / m_pModel->GetImage_2D()->UnRealSize.y;

	work->SetFloat("Ratio_X", ratio_X);
	work->SetFloat("Ratio_Y", ratio_Y);

	work->SetTexture("g_Texture", m_pModel->GetImage_2D()->pTex);
	work->SetFloat("g_brightness", m_brightness);
	work->SetFloat("Alpha", m_pModel->m_alpha);

	// 深度を書き込むのに必要
	work->SetVector("g_PintoPoint", &(static_cast<D3DXVECTOR4>(m_pModel->GetPintoPos())));
	work->SetVector("g_DepthFarNear", &(static_cast<D3DXVECTOR4>(SINSTANCE(CRenderContext)->GetDofRender()->GetDepthFarNear())));
	work->SetMatrix("g_PintoWorld", &m_pModel->GetPintoWorld());// ピントを合わせるポイントを行列変換するためのワールド行列

	work->CommitChanges();				//この関数を呼び出すことで、データの転送が確定する。描画を行う前に一回だけ呼び出す。

	(*graphicsDevice()).DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	work->EndPass();
	work->End();

	// アルファテスト、オフ
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	(*graphicsDevice()).SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, false);

	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	(*graphicsDevice()).SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void CParticle::Draw_EM(CCamera* camera){
	SetupMatrices();

	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	// アルファテストを有効化
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	// アルファの値が一定値より大きければ合格とし、描画する。小さければピクセルが破棄される
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	// アルファテストの境界値
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHAREF, 0x1);

	(*graphicsDevice()).SetRenderState(D3DRS_ZENABLE, true);
	(*graphicsDevice()).SetRenderState(D3DRS_ZWRITEENABLE, false);


	switch (m_alphaBlendMode){
	case 0:
		(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
	case 1:
		(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		(*graphicsDevice()).SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, true);
		(*graphicsDevice()).SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
		(*graphicsDevice()).SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ZERO);
		break;
	}


	//SINSTANCE(CRenderContext)->GetCurrentCamera()->SetCamera(m_pEffect);
	//ここで固定描画と同じように、ローカル座標に設定された頂点群をデバイスに渡す。通常と同じ方法。
	//	メッシュも同じく、マテリアルやテクスチャを設定
	//DrawSubset()を呼び出して描画

	(*graphicsDevice()).SetStreamSource(0, m_Primitive.GetVertexBuffer(), 0, sizeof(SShapeVertex_PT));
	(*graphicsDevice()).SetIndices(m_Primitive.GetIndexBuffer());
	(*graphicsDevice()).SetVertexDeclaration(m_Primitive.GetVertexDecl());

	static_cast<CEM_SamplingRender*>(m_pEMSamplingRender)->SetCamera(camera);
	EM_SetUpTechnique();
	m_pRender->GetEffect()->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	LPD3DXEFFECT work = m_pEMSamplingRender->GetEffect();
	work->BeginPass(0);	//パスの番号を指定してどのパスを使用するか指定

	work->SetMatrix("World", &mWorldViewProj);
	D3DXVECTOR2 split = m_pModel->GetSplit();
	work->SetInt("Split_X", split.x);
	work->SetInt("Split_Y", split.y);
	D3DXVECTOR2 now = m_pModel->GetNow();
	work->SetInt("NowCol", now.x);
	work->SetInt("NowRow", now.y);
	float ratio_X = m_pModel->GetImage_2D()->RealSize.x / m_pModel->GetImage_2D()->UnRealSize.x;
	float ratio_Y = m_pModel->GetImage_2D()->RealSize.y / m_pModel->GetImage_2D()->UnRealSize.y;

	work->SetFloat("Ratio_X", ratio_X);
	work->SetFloat("Ratio_Y", ratio_Y);

	work->SetTexture("g_Texture", m_pModel->GetImage_2D()->pTex);
	work->SetFloat("g_brightness", m_brightness);
	work->SetFloat("Alpha", m_pModel->m_alpha);

	work->CommitChanges();				//この関数を呼び出すことで、データの転送が確定する。描画を行う前に一回だけ呼び出す。

	(*graphicsDevice()).DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	work->EndPass();
	work->End();

	// アルファテスト、オフ
	(*graphicsDevice()).SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	(*graphicsDevice()).SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, false);

	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	(*graphicsDevice()).SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void CParticle::SetupMatrices(){
	D3DXMATRIX Trans;
	D3DXMatrixIdentity(&(m_pModel->m_World));
	D3DXMATRIX Scale;
	D3DXMatrixIdentity(&Scale);
	D3DXMatrixScaling(&Scale, m_transform.scale.x, m_transform.scale.y, m_transform.scale.z);
	D3DXMatrixMultiply(&(m_pModel->m_World), &(m_pModel->m_World), &Scale);

	D3DXMatrixIdentity(&Trans);
	D3DXMatrixTranslation(&Trans, m_ParticleData.position.x, m_ParticleData.position.y, m_ParticleData.position.z);
	if (m_isBillboard){
		// ビルボード処理
		const D3DXMATRIX CameraRota = m_camera->GetCameraRotation();
		D3DXMatrixMultiply(&(m_pModel->m_World), &(m_pModel->m_World), &CameraRota);
	}
	else{
		D3DXMatrixMultiply(&(m_pModel->m_World), &(m_pModel->m_World), &Trans);
	}

	D3DXMatrixMultiply(&(m_pModel->m_World), &(m_pModel->m_World), &Trans);

	switch (m_state){
	case EMIT_STATE::RUN:
		if (m_timer >= m_life){
			if (m_isFade){
				m_state = EMIT_STATE::FADEOUT;
				m_timer = 0.0f;
			}
			else{
				m_state = EMIT_STATE::DEAD;
			}
		}
		break;
	case EMIT_STATE::FADEOUT:{
		float t = m_timer / m_fadeTime;
		m_timer += m_deltaTime;
		m_pModel->m_alpha = m_initAlpha + (-m_initAlpha)*t;
		if (m_pModel->m_alpha <= 0.0f){
			m_pModel->m_alpha = 0.0f;
			m_state = EMIT_STATE::DEAD;
		}
	}break;
	case EMIT_STATE::DEAD:
		SINSTANCE(CObjectManager)->DeleteGameObject(this);
		break;
	}
	D3DXMatrixMultiply(&mWorldViewProj, &(m_pModel->m_World), &(m_camera->GetView()));
	D3DXMatrixMultiply(&mWorldViewProj, &mWorldViewProj, &(m_camera->GetProj()));
}

void CParticle::InitParticle(CRandom& random, CCamera& camera, const SParticleEmitParameter* param, const D3DXVECTOR3& emitPosition,D3DXVECTOR3 dir){

	UseModel<C2DImage>();
	m_pModel->SetFileName(param->texturePath);
	m_pRender = SINSTANCE(CRenderContext)->SelectRender(RENDER_STATE::_2D,_T(""),false,m_pModel);
	m_pEMSamplingRender = SINSTANCE(CRenderContext)->SelectRender(RENDER_STATE::EM_Sampling, _T(""), false, m_pModel);


	if (param->size_randMax < param->size_randMin){
		MessageBox(nullptr, _T("MaxとMinの数字が逆転しています"), _T("警告"), MB_OK);
		abort();
	}
	float rand = (static_cast<float>(random.GetRandDouble()) * (param->size_randMax - param->size_randMin)) + param->size_randMin;

	float width = param->w * rand;
	float hight = param->h * rand;

	m_transform.scale = D3DXVECTOR3(width, hight, 1.0f);

	float halfW = param->w * 0.5f;
	float halfH = param->h * 0.5f;
	CH_ASSERT(param->uvTableSize <= ARRAYSIZE(param->uvTable));
	D3DXVECTOR4 uv;
	if (param->uvTableSize > 0){
		uv = param->uvTable[random.GetRandInt() % param->uvTableSize];
	}
	else{
		uv = param->uvTable[0];
	}

	SShapeVertex_PT vp[] = {
		{ -halfW, halfH, 0.0f, 1.0f, uv.x, uv.y },
		{ halfW, halfH, 0.0f, 1.0f, uv.z, uv.y },
		{ -halfW, -halfH, 0.0f, 1.0f, uv.x, uv.w },
		{ halfW, -halfH, 0.0f, 1.0f, uv.z, uv.w }
	};
	short index[]{
		0, 1, 2, 3
	};
	m_Primitive.Create(
		EType::eTriangleStrip,
		4,
		sizeof(SShapeVertex_PT),
		scShapeVertex_PT_Element,
		vp,
		4,
		D3DFMT_INDEX16,
		index);

	CH_ASSERT(strlen(param->texturePath) <= MAX_FILENAME);
	strcpy(m_pFileName, param->texturePath);
	m_pModel->SetImage();
	m_camera = &camera;
	m_random = &random;
	m_life = param->life;

	m_ParticleData.velocity = dir * D3DXVec3Length(&(param->initVelocity));

	m_ParticleData.velocity.x += ((static_cast<float>(random.GetRandDouble()) - 0.5f) * 2.0f) * param->initVelocityVelocityRandomMargin.x;
	m_ParticleData.velocity.y += ((static_cast<float>(random.GetRandDouble()) - 0.5f) * 2.0f) * param->initVelocityVelocityRandomMargin.y;
	m_ParticleData.velocity.z += ((static_cast<float>(random.GetRandDouble()) - 0.5f) * 2.0f) * param->initVelocityVelocityRandomMargin.z;
	m_ParticleData.position = emitPosition;
	m_ParticleData.position.x += ((static_cast<float>(random.GetRandDouble()) - 0.5f) * 2.0f) * param->initPositionRandomMargin.x;
	m_ParticleData.position.y += ((static_cast<float>(random.GetRandDouble()) - 0.5f) * 2.0f) * param->initPositionRandomMargin.y;
	m_ParticleData.position.z += ((static_cast<float>(random.GetRandDouble()) - 0.5f) * 2.0f) * param->initPositionRandomMargin.z;
	m_addVelocityRandomMargin = param->addVelocityRandomMargin;
	m_ParticleData.gravity = param->gravity;
	m_isFade = param->isFade;
	m_state = EMIT_STATE::RUN;
	m_initAlpha = param->initAlpha;
	m_pModel->m_alpha = m_initAlpha;
	m_fadeTime = param->fadeTime;
	m_isBillboard = param->isBillboard;
	m_brightness = param->brightness;
	m_alphaBlendMode = param->alphaBlendMode;
	m_rotateZ = fPI * 2.0f * static_cast<float>(random.GetRandDouble());
	m_deltaTime = 1.0f / 60.0f;
	
	SetAlive(true);
}