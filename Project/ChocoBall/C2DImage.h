#pragma once
#include "GraphicsDevice.h"
#include "Model.h"
#include "C2DRender.h"
#include "RenderContext.h"
#include "Infomation.h"

class C2DImage :public CModel
{
public:
	C2DImage(){
		m_Split = D3DXVECTOR2(1.0f, 1.0f);
		m_Now = D3DXVECTOR2(0.0f, 0.0f);
		m_pImage = nullptr;
	};
	~C2DImage(){
	};
	void SetImage()override;
	void Initialize()override;
	void Update(const SH_ENGINE::TRANSFORM&)override;

	virtual void SetSplit(const D3DXVECTOR2& split){ m_Split = split; }
	virtual const D3DXVECTOR2& GetSplit(){ return m_Split; };
	virtual void SetNow(const D3DXVECTOR2& now){ m_Now = now; }
	virtual const D3DXVECTOR2& GetNow(){ return m_Now; };
	virtual MODEL::IMAGE2D* GetImage_2D(){ return m_pImage; }
public:
	D3DXVECTOR2 m_Split;		// âÊëúï™äÑêî
	D3DXVECTOR2 m_Now;			// åªç›âΩÉRÉ}ñ⁄Ç©
	MODEL::IMAGE2D* m_pImage;
};

