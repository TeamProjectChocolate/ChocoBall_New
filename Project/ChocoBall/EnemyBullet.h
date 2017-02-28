#pragma once
#include "stdafx.h"
#include "Bullet.h"


class CEnemyBullet:public Bullet
{
public:
	CEnemyBullet();
	~CEnemyBullet();
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void BulletCollision()override;
};

