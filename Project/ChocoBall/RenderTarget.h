#pragma once
#include "stdafx.h"


typedef struct STargetParam{
	unsigned int Size_Width;
	unsigned int Size_Height;
	D3DFORMAT TargetFormat;
	_D3DMULTISAMPLE_TYPE MultiSampleType;
	DWORD MultiSampleQuality;
	bool Discard;
	unsigned int Levels;
	DWORD Usage;
	D3DFORMAT TexFormat;
	D3DPOOL Pool;
}STargetParam;

class CRenderTarget
{
public:
	CRenderTarget();
	~CRenderTarget();
	void CreateRenderingTarget(STargetParam);
	void CreateRenderingTarget(unsigned int,unsigned int,D3DFORMAT,_D3DMULTISAMPLE_TYPE,DWORD,bool,unsigned int,DWORD,D3DFORMAT,D3DPOOL);
	LPDIRECT3DSURFACE9 GetZMap(){
		return m_pZMap;
	}
	LPDIRECT3DTEXTURE9 GetTexture(){
		return m_pTexture;
	}
	LPDIRECT3DSURFACE9 GetSurface(){
		return m_pSurface;
	}
private:
	LPDIRECT3DSURFACE9 m_pZMap = nullptr;		// 深度バッファ
	LPDIRECT3DTEXTURE9 m_pTexture = nullptr;		// バッファにコピーするためのテクスチャ
	LPDIRECT3DSURFACE9 m_pSurface = nullptr;	// サーフェス
	STargetParam m_Param;
};

