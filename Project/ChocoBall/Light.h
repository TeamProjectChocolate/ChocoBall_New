#pragma once
#include "stdafx.h"
#include "Infomation.h"
#include "Assert.h"


class CLight
{
public:
	// 各種初期化(ゴミデータが紛れ込まないように)
	CLight(){
		for (short idx = 0; idx < NUM_DIFFUSE_LIGHT; idx++) {
			this->SetDiffuseLightColor(idx, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f));
			this->SetDiffuseLightDirection(idx, D3DXVECTOR3(0.0f, -1.0f, 0.0f));
		}
		this->SetAmbientLight(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this->SetLimLightDirection(D3DXVECTOR3(0.0f, -1.0f, 0.0f));
		this->SetLimLightColor(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f));
	};
	~CLight(){};

	//ディフューズライトの向きを設定する関数
	//引き数: int型 どのライトの向きを設定するか
	//	　　: D3DXVECTOR3型 設定する向きの値(正規化されたベクトル、最大値1、最小値-1)
	//返り値: なし
	void SetDiffuseLightDirection(int lightNum, D3DXVECTOR3 dir){
		CH_ASSERT(lightNum < NUM_DIFFUSE_LIGHT);
		D3DXVec3Normalize(&dir,&dir);
		m_diffuseLightDirection[lightNum] = dir;
	}

	//ディフューズライトの向きを取得する関数
	//引き数: なし
	//返り値: D3DXVECTOR3*型 各ディフューズライトの向きを格納した配列の先頭アドレス
	D3DXVECTOR3* GetDiffuseLightDirection(){
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
	void SetAmbientLight(D3DXVECTOR3 ambient){
		m_ambientLight = ambient;
	}

	//環境光を取得する関数
	//引き数: なし
	//返り値: D3DXVECTOR3&型 環境光
	D3DXVECTOR3& GetAmbientLight(){
		return m_ambientLight;
	}

	/*!
	*@brief	リムライトの方向を設定。
	*/
	void SetLimLightDirection(const D3DXVECTOR3& dir)
	{
		m_limLightDir = dir;
	}
	/*!
	*@brief	リムライトの方向を取得。
	*/
	const D3DXVECTOR3& GetLimLightDirection() const
	{
		return m_limLightDir;
	}
	/*!
	*@brief	リムライトの色を設定。
	*/
	void SetLimLightColor(const D3DXVECTOR4& color)
	{
		m_limLightColor = color;
	}
	/*!
	*@brief	リムライトの色を取得。
	*/
	const D3DXVECTOR4& GetLimLightColor() const
	{
		return m_limLightColor;
	}

	void SetLight(LPD3DXEFFECT);
private:
	D3DXVECTOR3 m_diffuseLightDirection[NUM_DIFFUSE_LIGHT];		// ディフューズライトの向き
	D3DXVECTOR4 m_diffuseLightColor[NUM_DIFFUSE_LIGHT];			// ディフューズライトのカラー
	D3DXVECTOR3	m_limLightDir;									//!<リムライトの方向。
	D3DXVECTOR4	m_limLightColor;								//!<リムライトの色。
	D3DXVECTOR3 m_ambientLight;									// 環境光
};

