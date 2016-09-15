#pragma once
#include "stdafx.h"

//Direct3Dデバイス・オブジェクトを管理するクラス
class CGraphicsDevice
{
public:
	CGraphicsDevice(){
		m_pD3d = nullptr;
		m_pDevice = nullptr;
		//m_pMeshMat = nullptr;
		//m_pMeshTex = nullptr;
		//m_pMaterials = nullptr;
		//m_pMesh = nullptr;
		//m_pFileName = nullptr;
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
	// デバイスの回復を試みる
	//HRESULT RecoverDevice(){
	//	return m_pDevice->Reset(&m_d3dpp);
	//}
	//LPDIRECT3DDEVICE9へのキャストに関するオペレータ演算子
	operator LPDIRECT3DDEVICE9() const { return this->m_pDevice; }
private:
	LPDIRECT3D9 m_pD3d;			//DIRECT3Dオブジェクト
	LPDIRECT3DDEVICE9 m_pDevice;	//Direct3Dデバイス
	//D3DMATERIAL9* m_pMeshMat;			// マテリアル情報
	//LPDIRECT3DTEXTURE9* m_pMeshTex;		// メッシュのテクスチャ
	//D3DXMATRIX m_translation;
	//D3DXMATRIX m_rotation;
	//LPD3DXBUFFER m_pMaterials;
	//DWORD m_NumMaterials;
	//LPD3DXMESH m_pMesh;
	//LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;
	//D3DPRESENT_PARAMETERS m_d3dpp;
	//D3DPRESENT_PARAMETERS m_d3dppWin;
	//D3DPRESENT_PARAMETERS m_d3dppFull;
//protected:
	//LPCTSTR m_pFileName;	// 読み込むXファイルの名前を格納する
	//D3DXVECTOR3 m_position;	// 位置情報
	//D3DXVECTOR3 m_angle;	// 回転情報
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
