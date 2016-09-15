#include "stdafx.h"
#include "ZBufferSphere.h"
#include "RenderContext.h"
#include "ObjectManager.h"
#include "CourceCamera.h"
#include "Player.h"

CZBufferSphere::CZBufferSphere(){}

CZBufferSphere::~CZBufferSphere(){}

void CZBufferSphere::Initialize()
{
	m_pMapZ = nullptr;
	m_pZBufferSphereShadow = nullptr;
	m_pOriginalSurf = nullptr;

	// レンダリングターゲット生成
	(*graphicsDevice()).CreateDepthStencilSurface(WINDOW_WIDTH, WINDOW_HEIGHT, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pMapZ, NULL);
	(*graphicsDevice()).CreateTexture(WINDOW_WIDTH, WINDOW_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pZBufferSphereShadow, NULL);
	m_pZBufferSphereShadow->GetSurfaceLevel(0, &m_pOriginalSurf);

	m_transform.position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_transform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	GameCamera = SINSTANCE(CObjectManager)->FindGameObject<CCourceCamera>(_T("Camera"));
	Player = SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("TEST3D"));
	UseModel<C3DImage>();
	m_pModel->SetFileName("image/ZBufferSphere2.x");
	CGameObject::Initialize();
	SetAlive(true);
}

void CZBufferSphere::Update()
{
	float rate = 0.2f;
	if (!Player->IsVibration()) {
		m_transform.position = (Player->GetPos() * (1.0f - rate) + GameCamera->GetCamera()->GetPos() * rate);
	}
#if 0
	D3DXMATRIX mRot = GameCamera->GetCamera()->GetView();
	D3DXMatrixInverse(&mRot, NULL, &mRot); //カメラ行列の逆行列はカメラのワールド行列
	mRot.m[3][0] = 0.0f;
	mRot.m[3][1] = 0.0f;
	mRot.m[3][2] = 0.0f;
	mRot.m[3][3] = 1.0f;
	D3DXQuaternionRotationMatrix(&m_transform.angle, &mRot);
	D3DXVECTOR3 ToCamera = GameCamera->GetCamera()->GetPos() - Player->GetPos();
	D3DXVec3Normalize(&ToCamera, &ToCamera);
	ToCamera = ToCamera*5.0f;
	m_transform.position += ToCamera;
#endif
	CGameObject::Update();
}

void CZBufferSphere::Draw()
{
	// もとのレンダリングターゲットを保存
	IDirect3DSurface9* pOldBackBuffer;
	IDirect3DSurface9* pOldZBuffer;

	(*graphicsDevice()).GetRenderTarget(0, &pOldBackBuffer);
	(*graphicsDevice()).GetDepthStencilSurface(&pOldZBuffer);

	// レンダリングターゲットの変更
	(*graphicsDevice()).SetRenderTarget(0, m_pOriginalSurf);
	(*graphicsDevice()).SetDepthStencilSurface(m_pMapZ);

	(*graphicsDevice()).Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 0.0f, 0);
	SetUpTechnique();
	(*graphicsDevice()).SetRenderState(D3DRS_ZENABLE, FALSE);
	(*graphicsDevice()).SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	CGameObject::Draw();
	(*graphicsDevice()).SetRenderState(D3DRS_ZENABLE, TRUE);
	(*graphicsDevice()).SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	// レンダリングターゲットを元に戻す
	(*graphicsDevice()).SetRenderTarget(0, pOldBackBuffer);
	(*graphicsDevice()).SetDepthStencilSurface(pOldZBuffer);

}

void CZBufferSphere::DrawDepth(const D3DXVECTOR2& FarNear, const D3DXVECTOR3& PintoPos, const D3DXMATRIX& PintoWorld){

}