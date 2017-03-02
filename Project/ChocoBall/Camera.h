#pragma once

enum EUpdateType{
	enUpdateTypeTarget = 0,		//�J�����s��𒍎��_���g���Čv�Z����B
	enUpdateTypeDirection,	//�J�����s����������g���Čv�Z����B
};

class CCamera
{
public:
	CCamera();
	~CCamera();
	void Initialize();
	void Update();
	void SetCamera(LPD3DXEFFECT);
	//void SetFarNear(LPD3DXEFFECT);
	inline void SetPos(D3DXVECTOR3 pos){
		m_position = pos;
	}
	inline D3DXVECTOR3 GetPos(){
		return m_position;
	}
	inline D3DXVECTOR3 GetDir(){
		return m_direction;
	}
	inline void SetTarget(D3DXVECTOR3 target){
		m_target = target;
	}
	inline void SetUp(D3DXVECTOR3 up){
		m_up = up;
	}
	inline void SetViewAngle(float angle){
		m_ViewAngle = angle;
	}
	inline void SetAspect(float aspect){
		m_aspect = aspect;
	}
	inline void SetNear(float Near){
		m_Near = Near;
	}
	inline float GetNear(){
		return m_Near;
	}
	inline void SetFar(float Far){
		m_Far = Far;
	}
	inline float GetFar(){
		return m_Far;
	}
	inline void SetDirection(D3DXVECTOR3 dir)
	{
		m_direction = dir;
	}
	inline void SetUpdateType(EUpdateType type)
	{
		m_updateType = type;
	}
	inline void SetAxis(D3DXVECTOR3 axis){
		m_Axis = axis;
	}
	D3DXMATRIX GetView()
	{
		return m_View;
	}
	D3DXMATRIX* GetViewPointer(){
		return &m_View;
	}
	D3DXMATRIX GetProj()
	{
		return m_Proj;
	}
	D3DXMATRIX GetCameraRotation(){
		return m_Rota;
	}
	D3DXVECTOR3 GetTarget()
	{
		return m_target;
	}
	void SetIsOrtho(bool flg){
		m_IsOrtho = flg;
	}
	void SetViewVolume(const D3DXVECTOR2 volume){
		m_viewVolume = volume;
	}
	void SetNotWorkOutFlg(bool flg){
		m_NotWorkOut_Of_Up_and_Target = flg;
	}
private:
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_target;
	D3DXVECTOR3 m_up;
	D3DXVECTOR3 m_direction;	//�J�����̌����B
	EUpdateType m_updateType;
	float m_ViewAngle;	// ��p
	float m_aspect;
	float m_Near;
	float m_Far;
	D3DXMATRIX m_View;	// �r���[�ϊ��s��
	D3DXMATRIX m_Proj;	// �ˉe�ϊ��s��
	D3DXVECTOR3 m_Axis;	// ��]��
	D3DXMATRIX m_Rota;	// �J�����̉�]�s��
	D3DXMATRIX m_RotaInv;	// �J�����̉�]�s��̋t�s��
	bool m_IsOrtho;
	D3DXVECTOR2 m_viewVolume;
	bool m_NotWorkOut_Of_Up_and_Target;	// ������ƒ����_�������v�Z���Ȃ��ꍇ��true�B
};
