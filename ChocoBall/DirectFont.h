#pragma once
#include "Infomation.h"

#define FONT_WIDTH 16	// �ꕶ�����̉����B
#define FONT_HEIGHT 24	// �ꕶ�����̍����B

class CDirectFont
{
	SINGLETON_DECL(CDirectFont)
public:
	void Initialize();
	/*!
	* @brief	�`��B
	*@param[in]	text		�\������e�L�X�g
	*@param[in]	pos_x		�\������x���W�B
	*@param[in]	pos_y		�\������y���W�B
	*/
	void Draw(const char*,int,int);
private:
	LPD3DXFONT m_FontDevice = nullptr;
private:
	void DeleteAll();
};

