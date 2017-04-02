#pragma once
#include "Bullet.h"

// 曲線軌道する弾のクラス。
class CCurveBullet :
	public Bullet
{
public:
	CCurveBullet();
	~CCurveBullet();

	// 弾の初期化。
	// 引数：	発射位置。
	//			目標地点(この方向に徐々に軌道修正する)。
	//			最初に設定したい弾の飛ぶ方向。
	//			1フレームで回転できる量。
	void Initialize(const D3DXVECTOR3& ShotPos,D3DXVECTOR3* TargetPos, const D3DXVECTOR3& InitDir, float MaxAngle);
	void Update()override;

	inline void SetTargetPoint(D3DXVECTOR3* pos) {
		m_TargetPoint = pos;
	}
	// 引数は度。
	inline void SetMaxAngle(float angle) {
		m_MaxAngle = angle;
	}
	// 現在の向きを設定。
	// 引数はこの関数内で正規化される。
	inline void SetDir(const D3DXVECTOR3& dir)override{
		D3DXVec3Normalize(&m_NowDir,&dir);
	}
private:
	//enum class BulletState{Curve = 0};
private:
	D3DXVECTOR3* m_TargetPoint = nullptr;	// 弾が最終的に命中させたい位置。
	D3DXVECTOR3 m_NowDir;	// 現在の弾の向き。
	float m_MaxAngle;	// 1フレームで弾が回転できる限界量(度)。
	D3DXQUATERNION m_RotationAngle;	// 実際の回転(合計値)。
	D3DXVECTOR3 m_ShotToTarget;	// 発射地点から目標までのベクトル。
};

