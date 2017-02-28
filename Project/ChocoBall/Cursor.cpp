#include "Cursor.h"

CCursor::CCursor() { }

CCursor::~CCursor(){ }

void CCursor::Initialize()
{
	m_move = 30;
	C2DImage::Initialize();
}

void CCursor::Update()
{
	m_angle += 30;
	this->Select();
	this->Index();
	this->SetupMatrices();
} 

void CCursor::Setnum(short num){
	MAX_COMMAND_NUM = num;
}

void CCursor::Select(){

	m_dir = 0;

	if (m_Input.KBGetTrg(DIK_DOWN)){
		if (m_selectIndex < MAX_COMMAND_NUM - 1){
			m_dir = 1;
		}
	}
	else if (m_Input.KBGetTrg(DIK_UP)){
		if (m_selectIndex > 0){
			m_dir = -1;
		}
	}

	m_selectIndex += m_dir;
	m_position.y += m_move * (float)m_dir;
}

void CCursor::Index(){
	if (m_Input.KBGetTrg(DIK_RETURN)){
		switch (m_selectIndex){
		case COMMAND_SELECT::COMMAND1:
			break;
		case COMMAND_SELECT::COMMAND2:
			break;
		case COMMAND_SELECT::COMMAND3:
			break;
		case COMMAND_SELECT::COMMAND4:
			break;
		}
	}
}