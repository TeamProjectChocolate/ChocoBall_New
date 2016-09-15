#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "C3DImage.h"
#include "Camera.h"

class CShadowRender
{
	SINGLETON_DECL(CShadowRender)
	enum RenderState{None = 0,Boneless,Bone};
public:
	// 影を生成するオブジェクトを登録する関数
	// 引き数： C3DImage* C3DImageを継承したクラスのポインタ
	// 返り値： なし
	void Entry(CGameObject*);
	void Initialize();
	void Update();
	void Draw();
	void BeginDraw();
	void EndDraw();
	void SetObjectPos(D3DXVECTOR3 pos){
		m_target = pos;
	}
	void SetShadowCameraPos(D3DXVECTOR3 pos){
		m_ShadowLightPos = pos;
	}
	void SetShadowCameraDir(D3DXVECTOR3 dir){
		m_ShadowLightDirection = dir;
	}
	LPDIRECT3DTEXTURE9 GetTexture(){
		return m_pShadow;
	}
	void SetShadowCamera(LPD3DXEFFECT effect){
		effect->SetMatrix("LightViewProj", &(m_camera.GetView() * m_camera.GetProj()));
		m_camera.SetFarNear(effect);
	}
	void DeleteObject(CGameObject*);
	void DeleteObjectImidieit(CGameObject*);
	void CleanManager();
	void ExcuteDeleteObjects();
private:
	LPDIRECT3DSURFACE9 m_pMapZ = nullptr;			// 深度バッファ
	LPDIRECT3DTEXTURE9 m_pShadow = nullptr;		// 影を落とすためのテクスチャ
	LPDIRECT3DSURFACE9 m_pOriginalSurf = nullptr;	// サーフェス
	vector<CGameObject*> m_ShadowObjects;	// 影を生成したいオブジェクトの配列
	vector<CGameObject*> m_DeleteObjects;
	D3DXVECTOR3 m_target;		// 影を生成したいオブジェクトのポジション
	D3DXVECTOR3 m_ShadowLightPos;		// 影を描画するためのカメラのポジション
	D3DXVECTOR3 m_ShadowLightDirection;		// 影を描画するためのカメラの向き
	CCamera m_camera;
	D3DXMATRIX m_View;					// ライトのビュー行列
	D3DXMATRIX m_Proj;					// ライトのプロジェクション行列
	void DeleteAll();
	RenderState m_RenderState;
};

