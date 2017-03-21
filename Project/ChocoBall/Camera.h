#pragma once

enum EUpdateType{
	enUpdateTypeTarget = 0,		//カメラ行列を注視点を使って計算する。
	enUpdateTypeDirection,	//カメラ行列を向きを使って計算する。
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
	inline void SetPos(const D3DXVECTOR3& pos){
		m_position = pos;
	}
	inline D3DXVECTOR3 GetPos(){
		return m_position;
	}
	inline D3DXVECTOR3 GetDir(){
		return m_direction;
	}
	inline void SetTarget(const D3DXVECTOR3& target){
		m_target = target;
	}
	inline void SetUp(const D3DXVECTOR3& up){
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
	inline void SetDirection(const D3DXVECTOR3& dir)
	{
		m_direction = dir;
	}
	inline void SetUpdateType(EUpdateType type)
	{
		m_updateType = type;
	}
	inline void SetAxis(const D3DXVECTOR3& axis){
		m_Axis = axis;
	}
	const D3DXMATRIX& GetView()
	{
		return m_View;
	}
	D3DXMATRIX* GetViewPointer(){
		return &m_View;
	}
	const D3DXMATRIX& GetProj()
	{
		return m_Proj;
	}
	const D3DXMATRIX& GetRotation(){
		return m_Rota;
	}
	const D3DXVECTOR3& GetTarget()
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
	D3DXVECTOR3 m_direction;	//カメラの向き。
	EUpdateType m_updateType;
	float m_ViewAngle;	// 画角
	float m_aspect;
	float m_Near;
	float m_Far;
	D3DXMATRIX m_View;	// ビュー変換行列
	D3DXMATRIX m_Proj;	// 射影変換行列
	D3DXVECTOR3 m_Axis;	// 回転軸
	D3DXMATRIX m_Rota;	// カメラの回転行列
	D3DXMATRIX m_RotaInv;	// カメラの回転行列の逆行列
	bool m_IsOrtho;
	D3DXVECTOR2 m_viewVolume;
	bool m_NotWorkOut_Of_Up_and_Target;	// 上方向と注視点を自動計算しない場合はtrue。
};

