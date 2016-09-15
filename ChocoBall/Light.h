#pragma once
#include "stdafx.h"
#include "Infomation.h"
#include "Assert.h"


class CLight
{
public:
	// 各種初期化(ゴミデータが紛れ込まないように)
	CLight(){
		for (int i = 0; i < NUM_DIFFUSE_LIGHT; i++){
			m_diffuseLightDirection[i] = D3DXVECTOR4(0.0f, 0.0f, 0.0f,0.0f);
			m_diffuseLightColor[i] = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
		}
		m_ambientLight = D3DXVECTOR4(0.1f, 0.1f, 0.1f,1.0f);
	};
	~CLight(){};

	//ディフューズライトの向きを設定する関数
	//引き数: int型 どのライトの向きを設定するか
	//	　　: D3DXVECTOR3型 設定する向きの値(正規化されたベクトル、最大値1、最小値-1)
	//返り値: なし
	void SetDiffuseLightDirection(int lightNum, D3DXVECTOR4 dir){
		CH_ASSERT(lightNum < NUM_DIFFUSE_LIGHT);
		D3DXVec4Normalize(&dir,&dir);
		m_diffuseLightDirection[lightNum] = dir;
	}

	//ディフューズライトの向きを取得する関数
	//引き数: なし
	//返り値: D3DXVECTOR3*型 各ディフューズライトの向きを格納した配列の先頭アドレス
	D3DXVECTOR4* GetDiffuseLightDirection(){
		return m_diffuseLightDirection;
	}

	//ディフューズライトのカラーを設定する関数
	//引き数: int型 どのライトのカラーを設定するか
	//	　　: D3DXVECTOR4型 設定するカラーの値
	//返り値: なし
	void SetDiffuseLightColor(int lightNum, D3DXVECTOR4 color){
		CH_ASSERT(lightNum < NUM_DIFFUSE_LIGHT);
		m_diffuseLightColor[lightNum] = color;
	}

	//ディフューズライトのカラーを取得する関数
	//引き数: なし
	//返り値: D3DXVECTOR4*型 各ディフューズライトのカラーを格納した配列の先頭アドレス
	D3DXVECTOR4* GetDiffuseLightColor(){
		return m_diffuseLightColor;
	}

	//環境光を設定する関数
	//引き数: D3DXVECTOR3型 環境光？
	//返り値: なし
	void SetAmbientLight(D3DXVECTOR4 ambient){
		m_ambientLight = ambient;
	}

	//環境光を取得する関数
	//引き数: なし
	//返り値: D3DXVECTOR3&型 環境光
	D3DXVECTOR4& GetAmbientLight(){
		return m_ambientLight;
	}

	void SetLight(LPD3DXEFFECT);
private:
	D3DXVECTOR4 m_diffuseLightDirection[NUM_DIFFUSE_LIGHT];		// ディフューズライトの向き
	D3DXVECTOR4 m_diffuseLightColor[NUM_DIFFUSE_LIGHT];			// ディフューズライトのカラー
	D3DXVECTOR4 m_ambientLight;									// 環境光
};

