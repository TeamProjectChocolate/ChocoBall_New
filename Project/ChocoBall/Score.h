#pragma once
#include "Number.h"
#include "Player.h"

#define TIMEBONUS 10
#define ENEMY_BONUS 20

class CScore
{
public:

	CScore();
	~CScore();
	void Update();
private:
	int score;
	CPlayer* m_player;
	CNumber* m_number;
};