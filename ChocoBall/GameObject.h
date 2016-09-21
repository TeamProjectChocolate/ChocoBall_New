#pragma once
#include "Infomation.h"
#include "Assert.h"
#include "Model.h"
#include "C2DImage.h"
#include "C3DImage.h"

class CGameObject
{
public:
	CGameObject(){
		m_OriginalInit = false;
		m_ManagerNewFlg = false;
		m_alive = false;
		m_pRender = nullptr;
		m_RenderingState = RENDER_STATE::None;
		strcpy(m_pRenderName, "");
	};
	virtual ~CGameObject();
	virtual void OnDestroy(){};		// ObjectManagerクラスのDeleteGameObject関数が呼ばれたときに呼び出される関数
	virtual void Initialize();
	virtual void ActivateShadowRender(){
		SetShadowRenderState();
		m_pShadowRender = SINSTANCE(CRenderContext)->SelectRender(m_ShadowRenderingState, m_pRenderName, false, m_pModel);
	}

	virtual void Update();
	virtual void Draw();
	virtual void DrawShadow(CCamera*);

	virtual inline void SetFileName(LPCSTR name){
		m_pModel->SetFileName(name);
	}

	inline virtual void SetRenderState(){
		if (m_pModel){
			switch (m_pModel->m_Type){
			case MODEL_TYPE::T_2D:
				m_RenderingState = RENDER_STATE::_2D;
				break;
			case MODEL_TYPE::T_3D:
				m_RenderingState = RENDER_STATE::_3D;
				break;
			}
		}
	}
	inline virtual void SetShadowRenderState(){
		if (m_pModel){
			switch (m_pModel->m_Type){
			case MODEL_TYPE::T_2D:
				return;
			case MODEL_TYPE::T_3D:
				m_ShadowRenderingState = RENDER_STATE::_3D_ShadowSample;
				break;
			}
		}
	}



	template<class T>
	void UseModel();

	template<>
	void UseModel<C2DImage>(){
		m_pModel = new C2DImage;
		m_pModel->m_Type = MODEL_TYPE::T_2D;
		m_UseModel = true;
	}
	template<>
	void UseModel<C3DImage>(){
		m_pModel = new C3DImage;
		m_pModel->m_Type = MODEL_TYPE::T_3D;
		m_UseModel = true;
	}


	// モデルクラスのテクニックの名前を設定
	// (デフォルトは設定済み、独自のテクニックを使用する場合は継承先でオーバーライド！)
	virtual inline void SetUpTechnique(){
		m_pRender->SetUpTechnique("BasicTec");
	}

	CModel* GetModel(){
		return m_pModel;
	}
	void SetAlive(bool alive){
		m_alive = alive;
	}
	bool GetAlive(){
		return m_alive;
	}
	bool GetOriginal(){
		return m_OriginalInit;
	}
	void ActiveManagerNewFlg(){
		m_ManagerNewFlg = true;
	}
	bool GetManagerNewFlg(){
		return m_ManagerNewFlg;
	}
	virtual void SetPos(const D3DXVECTOR3& pos){
		m_transform.position = pos;
	}
	virtual const D3DXVECTOR3& GetPos()
	{
		return m_transform.position;
	}
	virtual void SetScale(const D3DXVECTOR3 scale){
		m_transform.scale = scale;
	}
	virtual const D3DXVECTOR3& GetScale(){
		return m_transform.scale;
	}
	// 2D回転
	inline void SetRotation(float rota){
		SetRotation(D3DXVECTOR3(0.0f, 0.0f, 1.0f), rota);
	}
	inline void SetRotation(const D3DXVECTOR3& axis, float rota){
		D3DXQuaternionRotationAxis(&(m_transform.angle), &axis, rota);
	}

	D3DXVECTOR3 GetmoveSpeed()
	{
		return m_moveSpeed;
	}
	void SetCommon(bool common){
		m_common = common;
	}
	bool GetCommon(){
		return m_common;
	}
	virtual void SetPintoWorld(const D3DXMATRIX& mat){
		if (m_UseModel){
			m_pModel->SetPintoWorld(mat);
		}
	}
	virtual void SetPintoPos(const D3DXVECTOR3& pos){
		if (m_UseModel){
			m_pModel->SetPintoPos(pos);
		}
	}
protected:
	CModel* m_pModel;
	CRender* m_pRender;
	CRender* m_pShadowRender;
	RENDER_STATE m_RenderingState;	// どのレンダーを使うか
	RENDER_STATE m_ShadowRenderingState;
	D3DXVECTOR3 m_moveSpeed;
	TRANSFORM m_transform;
	CHAR m_pFileName[MAX_FILENAME + 1];	// モデルファイルの名前(継承先で指定)
	CHAR m_pRenderName[MAX_FILENAME + 1];	// レンダーの識別名(デフォルトでは共通レンダーを使いまわすようになっている)
	bool m_IsInstancing;	// インスタンシング描画をするかどうかのフラグ
// オーバーロード初期化フラグ
// (継承先のクラスでInitialize関数のオーバーロードを使用した場合は、このフラグをそのクラス内で必ずtrueにしてください)
	bool m_OriginalInit;
private:
	bool m_ManagerNewFlg;	// ObjectManagerクラスでnewされたものか判定する変数
	bool m_alive;			// 生存フラグ(trueなら生存、falseなら死亡)
	bool m_common;			// 常駐フラグ(trueならシーン切り替えで削除されない)
	bool m_UseModel;		// 変数m_pModelをnewしているか
};

