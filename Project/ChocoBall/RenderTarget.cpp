#include "stdafx.h"
#include "RenderTarget.h"
#include "GraphicsDevice.h"


CRenderTarget::CRenderTarget()
{
}


CRenderTarget::~CRenderTarget()
{
}

void CRenderTarget::CreateRenderingTarget(STargetParam param){
	// レンダリングターゲット生成
	(*graphicsDevice()).CreateDepthStencilSurface(param.Size_Width, param.Size_Height,param.TargetFormat,param.MultiSampleType, param.MultiSampleQuality, param.Discard, &m_pZMap, NULL);
	(*graphicsDevice()).CreateTexture(param.Size_Width, param.Size_Height, param.Levels, param.Usage, param.TexFormat, param.Pool, &m_pTexture, NULL);
	m_pTexture->GetSurfaceLevel(0, &m_pSurface);
	m_Param = param;
}

void CRenderTarget::CreateRenderingTarget(
	unsigned int Size_Width,
	unsigned int Size_Height,
	D3DFORMAT TargetFormat,
	_D3DMULTISAMPLE_TYPE MultiSampleType,
	DWORD MultiSampleQuality,
	bool Discard,
	unsigned int Levels,
	DWORD Usage,
	D3DFORMAT TexFormat,
	D3DPOOL Pool
	){
	// レンダリングターゲット生成
	(*graphicsDevice()).CreateDepthStencilSurface(Size_Width, Size_Height, TargetFormat, MultiSampleType, MultiSampleQuality, Discard, &m_pZMap, NULL);
	(*graphicsDevice()).CreateTexture(Size_Width,Size_Height, Levels, Usage, TexFormat, Pool, &m_pTexture, NULL);
	m_pTexture->GetSurfaceLevel(0, &m_pSurface);

	STargetParam param = {
		Size_Width,
		Size_Height,
		TargetFormat,
		MultiSampleType,
		MultiSampleQuality,
		Discard,
		Levels,
		Usage,
		TexFormat,
		Pool
	};

	m_Param = param;
}
