#include "stdafx.h"
#include "Score.h"
#include "Player.h"
#include "Number.h"


CScore::CScore()
{
	//m_player = (SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("Player")));
	m_player = NULL;
	m_number = NULL;
}

CScore::~CScore()
{

}

void CScore::Update()
{	
	if (m_player == NULL)
	{
		m_player = (SINSTANCE(CObjectManager)->FindGameObject<CPlayer>(_T("Player")));
	}
	if (m_number == NULL)
	{
		m_number =(SINSTANCE(CObjectManager)->FindGameObject<CNumber>(_T("Number")));
	}
	if (m_player != NULL&&m_number!=NULL){
		//CNumber* pNumber = SINSTANCE(CObjectManager)->FindGameObject<CNumber>(_T("ResultNumber"));
		score = ENEMY_BONUS * m_player->GetBusterEnemyNum();
		m_number->SetValue(score);
	}
}