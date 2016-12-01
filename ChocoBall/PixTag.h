#pragma once
#include <stdlib.h>

class CPixTag
{
public:
	CPixTag();
	~CPixTag();
	void Start(LPCWSTR name) {
		// PIXに開始タグを打ち込む。
		D3DPERF_BeginEvent(D3DCOLOR_RGBA(0, 0, 0, 0), name);
	}
	void End() {
		// PIXに終了タグを打ち込む。
		D3DPERF_EndEvent();
	}
};

