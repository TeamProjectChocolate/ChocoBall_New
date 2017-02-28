#include "stdafx.h"
#include "GraphicsDevice.h"


/************************************************/
/*					InitD3d関数					*/
/*機能：Direct3Dデバイス・オブジェクト初期化　  */
/*引数：										*/
/*  hWnd：ウィンドウハンドル					*/
/*戻り値：成功(S_OK)か失敗(E_FAIL)か			*/
/************************************************/
HRESULT CGraphicsDevice::InitD3d(HWND hWnd)
{
	//Direct3Dオブジェクト生成不可か？
	if((this->m_pD3d = Direct3DCreate9( D3D_SDK_VERSION )) == NULL)
	{
	MessageBox(0, TEXT("Direct3D初期化失敗"), NULL, MB_OK);
	return E_FAIL;	//失敗返却
	}

	D3DPRESENT_PARAMETERS d3dpp;	//プレゼンテーションパラメータ
	ZeroMemory( &d3dpp, sizeof(d3dpp));	//プレゼンテーションパラメータ初期化

	//バックバッファフォーマットを未定義にセット
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;	//バックバッファ数をセット
	//スワップエフェクトをセット
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; 
	d3dpp.Windowed = TRUE;//ウィンドウモードをTRUE(ウィンドウサイズ)にセット
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if(FAILED(this->m_pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &this->m_pDevice)))
	{
		if(FAILED(this->m_pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &this->m_pDevice)))
		{
			MessageBox(0, TEXT("HALモードでDirect3Dデバイスを作成できません\nREFモードで再試行します"), NULL, MB_OK);
			if(FAILED(this->m_pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &this->m_pDevice)))
			{
				if(FAILED(this->m_pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &this->m_pDevice)))
				{
					MessageBox(0, TEXT("Direct3Dデバイスの作成に失敗しました"), NULL, MB_OK);
					return E_FAIL;
				}
			}
		}
	}

	return S_OK;
}

/************************************************/
/*		FreeDX関数								*/
/*機能：Direct3Dデバイス・オブジェクト解放		*/
/*引数：なし									*/
/*戻り値：なし									*/
/************************************************/
void CGraphicsDevice::FreeDX(void)
{
	SAFE_RELEASE(this->m_pDevice);	//Direct3Dデバイス解放
	SAFE_RELEASE(this->m_pD3d);	//Direct3Dオブジェクト解放
}

