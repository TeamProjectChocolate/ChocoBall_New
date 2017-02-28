#include "stdafx.h"
#include "DirectFont.h"
#include "GraphicsDevice.h"

CDirectFont* CDirectFont::m_instance = nullptr;

void CDirectFont::Initialize() {
	D3DXCreateFont(
		graphicsDevice(),
		FONT_HEIGHT,
		FONT_WIDTH,
		FW_REGULAR,	// �t�H���g�̑����B
		NULL,	// �����B
		false,	// �ΆB
		SHIFTJIS_CHARSET,	// �����R�[�h�B
		OUT_DEFAULT_PRECIS,	// �o�͐��x�͕��ʁB
		PROOF_QUALITY,		// �����i�����d���B
		FIXED_PITCH | FF_SCRIPT,	// �s�b�`�ƃt�@�~���B
		TEXT("MS P�S�V�b�N"),	// �t�H���g���B
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
			D3DCOLOR_XRGB(255, 255, 255)	// ���F�B
		);
	}
}

void CDirectFont::DeleteAll() {
	if (m_FontDevice != nullptr) {
		m_FontDevice->Release();
		m_FontDevice = nullptr;
	}
}