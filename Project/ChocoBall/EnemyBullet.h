#pragma once
#include "stdafx.h"
#include "Bullet.h"


class CEnemyBullet:public CGameObject
{
public:
	CEnemyBullet();
	~CEnemyBullet();
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void BulletCollision();

	inline void SetStartPos(const D3DXVECTOR3& pos) {
		m_Bullet->SetStartPos(pos);
	}
	inline void SetDir(const D3DXVECTOR3& dir) {
		m_Bullet->SetDir(dir);
	}
	inline void SetBulletSpeed(float speed) {
		m_Bullet->SetBulletSpeed(speed);
	}
	inline void SetRange(float range) {
		m_Bullet->SetRange(range);
	}
	inline void SetPintoPos(const D3DXVECTOR3& pos)override {
		m_Bullet->SetPintoPos(pos);
	}
	inline void SetPintoWorld(const D3DXMATRIX& mat)override {
		m_Bullet->SetPintoWorld(mat);
	}
	inline void ChangeBulletClass(Bullet* bullet) {
		m_Bullet.reset(bullet);
	}
	// Bullet�N���X�̃|�C���^�擾�B
	// ����΂ɂ��̃N���X�̊O���ō폜���Ȃ��B
	inline Bullet* GetBullet() const{
		return m_Bullet.get();
	}
private:
	unique_ptr<Bullet> m_Bullet;
};

