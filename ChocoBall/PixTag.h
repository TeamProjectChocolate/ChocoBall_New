#pragma once
#include <stdlib.h>

class CPixTag
{
public:
	CPixTag();
	~CPixTag();
	void Start(LPCWSTR name) {
		// PIX�ɊJ�n�^�O��ł����ށB
		D3DPERF_BeginEvent(D3DCOLOR_RGBA(0, 0, 0, 0), name);
	}
	void End() {
		// PIX�ɏI���^�O��ł����ށB
		D3DPERF_EndEvent();
	}
};

