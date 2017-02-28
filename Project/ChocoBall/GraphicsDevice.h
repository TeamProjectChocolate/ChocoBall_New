#pragma once
#include "stdafx.h"

//Direct3Dデバイス・オブジェクトを管理するクラス
class CGraphicsDevice
{
public:
	CGraphicsDevice(){
		m_pD3d = nullptr;
		m_pDevice = nullptr;
	}
	~CGraphicsDevice(){
		FreeDX();
	}
	HRESULT InitD3d(HWND);	//Direct3Dオブジェクトおよびデバイス初期化
	void ChangeDisplayMode(HWND, HMENU, bool*, bool*, RECT*);	// ディスプレイモードの変更
	void FreeDX();		//Direct3Dデバイス・オブジェクト解放

	bool GetState(void){
		if (!m_pD3d || !m_pDevice){
			return false;
		}
	}
	// デバイスの協調状態を確認する
	HRESULT GetCooperativeLevel(void){
		return m_pDevice->TestCooperativeLevel();
	}
	// シングルトン
	operator LPDIRECT3DDEVICE9() const { return this->m_pDevice; }
private:
	LPDIRECT3D9 m_pD3d;			//DIRECT3Dオブジェクト
	LPDIRECT3DDEVICE9 m_pDevice;	//Direct3Dデバイス
};

//グローバル関数を定義してどこかにある実体を返す(ここでいうとcGraphicsDeviceクラスのインスタンスを返す)
inline CGraphicsDevice& graphicsDevice()
{
	extern CGraphicsDevice g_graphicsDevice;
	return g_graphicsDevice;
}

struct CUSTOMVERTEX{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL)
