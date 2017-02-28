#include "stdafx.h"
#include "DirectFont.h"
#include "GraphicsDevice.h"

CDirectFont* CDirectFont::m_instance = nullptr;

void CDirectFont::Initialize() {
	D3DXCreateFont(
		graphicsDevice(),
		FONT_HEIGHT,
		FONT_WIDTH,
		FW_REGULAR,	// フォントの太さ。
		NULL,	// 下線。
		false,	// 斜体。
		SHIFTJIS_CHARSET,	// 文字コード。
		OUT_DEFAULT_PRECIS,	// 出力制度は普通。
		PROOF_QUALITY,		// 文字品質を重視。
		FIXED_PITCH | FF_SCRIPT,	// ピッチとファミリ。
		TEXT("MS Pゴシック"),	// フォント名。
		&m_FontDevice
	);
}

void CDirectFont::Draw(const char* text,int pos_X,int pos_Y) {
	if (m_FontDevice) {
		int textLen = static_cast<int>(strlen(text));
		RECT rc = {
			pos_X,
			pos_Y,
			pos_X + textLen * FONT_WIDTH,
			pos_Y + FONT_HEIGHT
		};
		m_FontDevice->DrawText(
			nullptr,
			text,
			-1,
			&rc,
			DT_LEFT,
			D3DCOLOR_XRGB(255, 255, 255)	// 白色。
		);
	}
}

void CDirectFont::DeleteAll() {
	if (m_FontDevice != nullptr) {
		m_FontDevice->Release();
		m_FontDevice = nullptr;
	}
}