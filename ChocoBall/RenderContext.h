#pragma once

#include "stdafx.h"
#include "Light.h"
#include "Primitive.h"
#include "BloomRender.h"
#include "DofRender.h"
#include "Render.h"
#include "Infomation.h"
#include "C3DObjectRender.h"
#include "C3DObjectRender_S.h"
#include "InstancingRender.h"
#include "C2DRender.h"
#include "C3DRender_D.h"
#include "InstancingRender_D.h"
#include "ShadowSamplingRender.h"
#include "ShadowSamplingRender_I.h"
#include "RenderTarget.h"

class CCamera;

typedef struct RENDER_DATA{
	CRender* render;
	CHAR name[MAX_FILENAME + 1];	// 識別名
	bool IsCommon = false;	// シーン切り替え時に削除しない場合trueにする(一度しか初期化されないBloomやDofはこれをtrueにする)
};

class CRenderContext{
	SINGLETON_DECL(CRenderContext)
public:

	CRender* SelectRender(RENDER_STATE state,LPCSTR name,bool IsCommon,CModel* pModel){
		switch (state){
		case RENDER_STATE::None:
			return nullptr;
		case RENDER_STATE::_2D:
			return CreateRender<C2DRender>(state, name, IsCommon);
		case RENDER_STATE::_3D:
			return CreateRender<C3DObjectRender>(state, name, IsCommon);
		case RENDER_STATE::_3D_Simple:
			return CreateRender<C3DObjectRender_S>(state, name, IsCommon);
		case RENDER_STATE::_3D_Depth:
			return CreateRender<C3DRender_D>(state, name, IsCommon);
		case RENDER_STATE::_3D_ShadowSample:
			return CreateRender<CShadowSamplingRender>(state, name, IsCommon);
		case RENDER_STATE::_3D_ShadowSample_I:
			return CreateRender<CShadowSamplingRender_I>(state, name, IsCommon,pModel);
		case RENDER_STATE::Bloom:
			return CreateRender<CBloomRender>(state, name, IsCommon);
		case RENDER_STATE::Dof:
			return CreateRender<CDofRender>(state, name, IsCommon);
		case RENDER_STATE::Instancing:
			return CreateRender<CInstancingRender>(state, name, IsCommon,pModel);
		case RENDER_STATE::Instancing_Depth:
			return CreateRender<CInstancingRender_D>(state, name, IsCommon, pModel);
		case RENDER_STATE::Max:
			MessageBox(nullptr, _T("そのレンダーステートはあかんでぇ～"), _T("Message"), MB_OK);
			abort();
		}
	}
	// レンダーの生成、初期化
	// 第二引数は生成するレンダーの名前(同じRENDER_STATEかつ同じレンダーで事足りる場合は指定しなくてもよい)
	// 第三引数はシーン切り替え時に削除するかのフラグ(削除しない場合true)
	template<class Type>
	Type* CreateRender(RENDER_STATE state, LPCSTR name,bool IsCommon){
		for (auto render : m_Renders[state]){
			if (render->render != nullptr){
				if (!strcmp(render->name, name)){
					return static_cast<Type*>(render->render);
				}
			}
		}
		RENDER_DATA* data = new RENDER_DATA;
		strcpy(data->name, name);
		data->IsCommon = IsCommon;
		Type* Render = new Type;
		static_cast<Type*>(Render)->Initialize();
		data->render = Render;
		m_Renders[state].push_back(data);
		return static_cast<Type*>(Render);
	}

	// レンダーの生成、初期化
	// 第二引数は生成するレンダーの名前(同じRENDER_STATEかつ同じレンダーで事足りる場合は指定しなくてもよい)
	// 第三引数はシーン切り替え時に削除するかのフラグ(削除しない場合true)
	// 第四引数は初期化で使用するモデルの情報
	template<class Type>
	Type* CreateRender(RENDER_STATE state, LPCSTR name, bool IsCommon,CModel* pModel){
		for (auto render : m_Renders[state]){
			if (render->render != nullptr){
				if (!strcmp(render->name, name)){
					return static_cast<Type*>(render->render);
				}
			}
		}
		RENDER_DATA* data = new RENDER_DATA;
		strcpy(data->name, name);
		data->IsCommon = IsCommon;
		Type* Render = new Type;
		Render->SetModelData(pModel);
		static_cast<Type*>(Render)->Initialize();
		data->render = Render;
		m_Renders[state].push_back(data);
		return static_cast<Type*>(Render);
	}

	void Draw(RENDER_STATE);
	void Draw(CRender*);
	void CreateRenderingTerget();
	void RenderingStart();
	void RenderingEnd();
	void SetRenderingBuffer();

	void DeleteRenders();

	template<class Type>
	Type* FindRender(RENDER_STATE state, LPCSTR name){
		int size = m_Renders[state].size();
		for (int idx = 0; idx < size; idx++){
			if (m_Renders[state][idx]){
				if (!strcmp(m_Renders[state][idx]->name, name)){
					return static_cast<Type*>(m_Renders[state][idx]->render);
				}
			}
			else{
				// nullアクセス
				abort();
			}
		}
	}
	const vector<RENDER_DATA*>& GetRenderArray(RENDER_STATE state){
		return m_Renders[state];
	}

	void SetCurrentCamera(CCamera* cam)
	{
		m_pCamera = cam;
	}
	CCamera* GetCurrentCamera()
	{
		return m_pCamera;
	}
	void SetCurrentLight(CLight* light){
		m_pLight = light;
	}
	CLight* GetCurrentLight(){
		return m_pLight;
	}
	LPDIRECT3DSURFACE9 GetRenderingTarget(){
		return m_RenderTarget.GetSurface();
	}
	LPDIRECT3DSURFACE9 GetMapZ(){
		return m_RenderTarget.GetZMap();
	}
	LPDIRECT3DTEXTURE9 GetTexture(){
		return m_RenderTarget.GetTexture();
	}
	unsigned int GetWindowWidth(){
		return  m_bufferSize_Width;
	}
	unsigned int GetWindowHeight(){
		return  m_bufferSize_Height;
	}
	CPrimitive* GetPrimitive(){
		return &m_copyBackBufferPrim;
	}
	CDofRender* GetDofRender(){
		return m_DofRender;
	}
private:
	CRenderTarget m_RenderTarget;
	LPDIRECT3DSURFACE9 m_SavedMapZ;			// 深度バッファを保存しておくための変数
	LPDIRECT3DSURFACE9 m_SavedBuffer;			// バッファを保存しておくための変数
	CPrimitive m_copyBackBufferPrim;		//!<メインレンダーターゲットをバックバッファにコピーするときに使うプリミティブ。
	LPD3DXEFFECT m_pEffect;
	unsigned int m_bufferSize_Width;
	unsigned int m_bufferSize_Height;
	CCamera* m_pCamera = nullptr;
	CLight* m_pLight = nullptr;
	CBloomRender* m_BloomRender;
	CDofRender* m_DofRender;
	void DeleteAll();
private:
	vector<vector<RENDER_DATA*>> m_Renders;	// 使用するレンダーの名前が同じならば新しく作らず、既存のものを使いまわす(新しく生成する場合は配列に積んでいく)
};
