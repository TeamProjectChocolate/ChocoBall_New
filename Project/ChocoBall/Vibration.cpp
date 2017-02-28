#include "stdafx.h"
#include "Vibration.h"


CVibration::CVibration()
{
}


CVibration::~CVibration()
{
}

void CVibration::Initialize(){
	m_dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_VibrationAmount = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_VibrationTime = 0.0f;
	m_currentDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_IsVibration = false;
	m_ReflectionTime = 0.0f;
	m_VibrationCounter = 0.0f;
	m_ReflectionCounter = 0.0f;
	m_CourceDef.Initialize();
}

void CVibration::Update(){
	if (m_IsVibration){
		if (m_VibrationCounter <= m_VibrationTime){
			if (D3DXVec3Length(&(*m_Target - m_OrgPos)) >= m_length){
				m_dir *= -1;
			}
			*m_Target += (m_dir * m_MoveSpeed);
			//m_ReflectionCounter += 1.0f / 60.0f;
			//if (m_ReflectionCounter >= m_ReflectionTime){
			//	m_VibrationAmount *= -1.0f;
			//	m_ReflectionCounter = 0.0f;
			//}
			//m_dir.y = 0.0f;
			//D3DXVec3Cross(&m_currentDir, &m_dir, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
			//*m_Target += m_VibrationAmount.x * m_currentDir;
			//m_Target->y += m_VibrationAmount.y;
			//*m_Target += m_dir * m_VibrationAmount.z;
		}
		else{
			m_VibrationCounter = 0.0f;
			m_OrgPos.y = m_Target->y;
			*m_Target = m_OrgPos;
			m_IsVibration = false;
			return;
		}
		m_VibrationCounter += 1.0f / 60.0f;	
	}
}

void CVibration::ThisVibration(D3DXVECTOR3* pos,D3DXVECTOR3 Amount,float Time,float ReflectionTime){
	COURCE_BLOCK Cource = m_CourceDef.FindCource(*pos);
	m_IsVibration = true;
	m_VibrationTime = Time;
	m_ReflectionTime = ReflectionTime;
	m_VibrationAmount = Amount;
	m_Target = pos;
	m_OrgPos = *m_Target;

	D3DXVECTOR3 Vec = (m_OrgPos + m_VibrationAmount) - m_OrgPos;
	m_length = D3DXVec3Length(&Vec);
	m_length /= 2;
	m_MoveSpeed = m_length / (m_ReflectionTime / 2);
	D3DXVec3Normalize(&m_dir, &m_VibrationAmount);
}