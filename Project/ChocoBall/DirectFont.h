#pragma once
#include "Infomation.h"

#define FONT_WIDTH 16	// 一文字分の横幅。
#define FONT_HEIGHT 24	// 一文字分の高さ。

class CDirectFont
{
	SINGLETON_DECL(CDirectFont)
public:
	void Initialize();
	/*!
	* @brief	描画。
	*@param[in]	text		表示するテキスト
	*@param[in]	pos_x		表示するx座標。
	*@param[in]	pos_y		表示するy座標。
	*/
	void Draw(const char*,int,int);
private:
	LPD3DXFONT m_FontDevice = nullptr;
private:
	void DeleteAll();
};

