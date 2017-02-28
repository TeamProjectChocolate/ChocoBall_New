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
	virtual void OnDestroy(){};		// ObjectManager�N���X��DeleteGameObject�֐����Ă΂ꂽ�Ƃ��ɌĂяo�����֐�
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

	// �^����ꂽ�x�N�g��������GameObject�̍��W�n�ɕϊ����ĕԋp����֐��B
	__inline D3DXVECTOR4 Vec3Transform(const D3DXVECTOR3& vec3) {
		return *D3DXVec3Transform(&D3DXVECTOR4(), &vec3, &(GetModel()->m_World));
	}

	// ���f���N���X�̃e�N�j�b�N�̖��O��ݒ�
	// (�f�t�H���g�͐ݒ�ς݁A�Ǝ��̃e�N�j�b�N���g�p����ꍇ�͌p����ŃI�[�o�[���C�h�I)
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
		// ��p���C�g���g�p���邱�Ƃ𖾎�����B
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
	// 2D��]
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

	// ���ܗ������f���ɐݒ�B
	// �����͔��˃^�C�v�̗񋓌^�B
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
	RENDER_STATE m_RenderingState;	// �ǂ̃����_�[���g����
	RENDER_STATE m_ShadowRenderingState;
	RENDER_STATE m_EMRenderingState;
	float m_moveSpeed;
	TRANSFORM m_transform;
	CHAR m_pFileName[MAX_FILENAME + 1];	// ���f���t�@�C���̖��O(�p����Ŏw��)
	CHAR m_pRenderName[MAX_FILENAME + 1];	// �����_�[�̎��ʖ�(�f�t�H���g�ł͋��ʃ����_�[���g���܂킷�悤�ɂȂ��Ă���)
// �I�[�o�[���[�h�������t���O
// (�p����̃N���X��Initialize�֐��̃I�[�o�[���[�h���g�p�����ꍇ�́A���̃t���O�����̃N���X���ŕK��true�ɂ��Ă�������)
	bool m_OriginalInit;
	vector<CGameObject*> m_ChildObjects;	// �q�ǂ��B
private:
	D3DXVECTOR3 m_Direction;

	bool m_ManagerNewFlg;	// ObjectManager�N���X��new���ꂽ���̂����肷��ϐ�
	bool m_alive;			// �����t���O(true�Ȃ琶���Afalse�Ȃ玀�S)
	bool m_common;			// �풓�t���O(true�Ȃ�V�[���؂�ւ��ō폜����Ȃ�)
	bool m_UseModel;		// �ϐ�m_pModel��new���Ă��邩
	bool m_HasMyLight;		// ������p�̃��C�g���g�p���邩�B
};

