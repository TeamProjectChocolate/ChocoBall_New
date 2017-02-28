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
		m_Direction = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	};
	virtual ~CGameObject();
	virtual void OnDestroy(){};		// ObjectManagerクラスのDeleteGameObject関数が呼ばれたときに呼び出される関数
	virtual void Initialize();
	void InitInstancing(int,bool);
	virtual void ActivateShadowRender(){
		SetShadowRenderState();
		m_pShadowRender = SINSTANCE(CRenderContext)->SelectRender(m_ShadowRenderingState, m_pRenderName, false, m_pModel);
	}

	virtual void Update();
	virtual void Draw();
	virtual void DrawShadow(CCamera*);
	virtual void Draw_EM(CCamera*);

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
	inline virtual void EM_SetRenderState(){
		if (m_pModel){
			switch (m_pModel->m_Type){
			case MODEL_TYPE::T_2D:
				return;
			case MODEL_TYPE::T_3D:
				m_EMRenderingState = RENDER_STATE::EM_Sampling;
				break;
			}
		}
	}

	virtual void Is_DrawShadow_Use_Horizon();

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

	// 与えられたベクトルをこのGameObjectの座標系に変換して返却する関数。
	__inline D3DXVECTOR4 Vec3Transform(const D3DXVECTOR3& vec3) {
		return *D3DXVec3Transform(&D3DXVECTOR4(), &vec3, &(GetModel()->m_World));
	}

	// モデルクラスのテクニックの名前を設定
	// (デフォルトは設定済み、独自のテクニックを使用する場合は継承先でオーバーライド！)
	virtual inline void SetUpTechnique(){
		if (m_RenderingState == RENDER_STATE::_3D){
			m_pRender->SetUpTechnique("Boneless");
		}
		else if (m_RenderingState == RENDER_STATE::_2D){
			m_pRender->SetUpTechnique("BasicTec");
		}
	}
	virtual inline void EM_SetUpTechnique(){
		m_pEMSamplingRender->SetUpTechnique("Boneless");
	}

	virtual inline void SetUpShadowTechnique() {
		m_pShadowRender->SetUpTechnique("BonelessShadowMapping");
	}


	virtual void ConfigLight() {
		// 専用ライトを使用することを明示する。
		m_HasMyLight = true;
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
	virtual inline const D3DXVECTOR3& GetDirection() {
		return m_Direction;
	}
	virtual inline void SetQuaternion(const D3DXQUATERNION& quat) {
		m_transform.angle = quat;
	}
	virtual inline const D3DXQUATERNION& GetQuaternion() {
		return m_transform.angle;
	}
	// 2D回転
	inline void SetRotation(float rota){
		SetRotation(D3DXVECTOR3(0.0f, 0.0f, 1.0f), rota);
	}
	inline void SetRotation(const D3DXVECTOR3& axis, float rota){
		D3DXQuaternionRotationAxis(&(m_transform.angle), &axis, rota);
	}

	float GetmoveSpeed()
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

	inline void SetLight(CLight* pLight) {
		m_pLight = pLight;
	}

	// 屈折率をモデルに設定。
	// 引数は反射タイプの列挙型。
	inline void SetRefractive(REFRACTIVES ref) {
		m_pModel->m_Refractive = g_RefractivesTable[ref];
	}
	inline void SetAlpha(float f) {
		m_pModel->SetAlpha(f);
	}
	inline float GetAlpha() {
		return m_pModel->GetAlpha();
	}
protected:
	CModel* m_pModel;
	CRender* m_pRender;
	CLight* m_pLight = nullptr;
	CRender* m_pShadowRender;
	CRender* m_pEMSamplingRender;
	RENDER_STATE m_RenderingState;	// どのレンダーを使うか
	RENDER_STATE m_ShadowRenderingState;
	RENDER_STATE m_EMRenderingState;
	float m_moveSpeed;
	TRANSFORM m_transform;
	CHAR m_pFileName[MAX_FILENAME + 1];	// モデルファイルの名前(継承先で指定)
	CHAR m_pRenderName[MAX_FILENAME + 1];	// レンダーの識別名(デフォルトでは共通レンダーを使いまわすようになっている)
// オーバーロード初期化フラグ
// (継承先のクラスでInitialize関数のオーバーロードを使用した場合は、このフラグをそのクラス内で必ずtrueにしてください)
	bool m_OriginalInit;
	vector<CGameObject*> m_ChildObjects;	// 子ども。
private:
	D3DXVECTOR3 m_Direction;

	bool m_ManagerNewFlg;	// ObjectManagerクラスでnewされたものか判定する変数
	bool m_alive;			// 生存フラグ(trueなら生存、falseなら死亡)
	bool m_common;			// 常駐フラグ(trueならシーン切り替えで削除されない)
	bool m_UseModel;		// 変数m_pModelをnewしているか
	bool m_HasMyLight;		// 自分専用のライトを使用するか。
};

