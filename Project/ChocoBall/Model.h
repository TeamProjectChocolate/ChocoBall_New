#pragma once

#include "stdafx.h"
#include "Assert.h"
#include "Infomation.h"
#include "Render.h"
#include "Animation.h"


#define BoneMatrixMax 128

enum MODEL_TYPE{ T_2D = 0,T_3D };

class CModel
{
public:
	CModel(){
		m_alpha = 1.0f;
		m_luminance = 0.0f;
		m_Refractive = 1.000293f;/*地球の大気の屈折率*/
		m_Horizon.clear();
	};
	virtual ~CModel(){};
	virtual void Initialize();
	virtual void Update(const SH_ENGINE::TRANSFORM&);
	virtual void SetImage() = 0;

	void SetAlpha(float alpha){
		m_alpha = alpha;
	}
	float GetAlpha(){
		return m_alpha;
	}
	void SetFileName(LPCSTR Name){
		CH_ASSERT(strlen(Name) < MAX_FILENAME);
		strcpy(m_pFileName, Name);
	}
	const D3DXMATRIX& GetWorldMatrix(){
		return m_World;
	}
	void SetPintoWorld(const D3DXMATRIX& mat){
		m_PintoWorld = mat;
	}
	const D3DXMATRIX& GetPintoWorld(){
		return m_PintoWorld;
	}
	void SetPintoPos(const D3DXVECTOR3& pos){
		m_PintoPos = pos;
	}
	const D3DXVECTOR3& GetPintoPos(){
		return m_PintoPos;
	}

	// 3D用
	virtual CAnimation* GetAnimation(){ return nullptr; }
	virtual void SetUseBorn(bool){}
	virtual bool GetUseBorn(){ return false; }
	virtual void SetCurrentAnimNo(int){};
	virtual int GetCurrentAnimNo(){ return 0; }
	virtual const D3DXMATRIX& GetRotation(){ return D3DXMATRIX(); }
	virtual CSkinModelData* GetImage_3D(){ return nullptr; }

	// 影描画(境界線)用。
	// 境界線を頂点ごとに指定する関数。
	// ※頂点数分の配列を渡してください。
	void CopyHorizon(const vector<float>& horizon) {
		m_IsVerticesNumHorizon = true;
		for (float work : horizon) {
			m_Horizon.push_back(work);
		}
	}
	void CopyHorizon(float horizon) {
		m_IsVerticesNumHorizon = false;
		m_HorizonSingle = horizon;
	}
	void SetIsHorizon(bool flg) {
		m_IsHorizon = flg;
	}
	const vector<float>& GetHorizonArray() {
		return m_Horizon;
	}
	float GetHorizonSingle() {
		return m_HorizonSingle;
	}

	// 2D用
	virtual void SetSplit(const D3DXVECTOR2&){}
	virtual const D3DXVECTOR2& GetSplit(){ return D3DXVECTOR2(0.0f, 0.0f); };
	virtual void SetNow(const D3DXVECTOR2&){}
	virtual const D3DXVECTOR2& GetNow(){ return D3DXVECTOR2(0.0f, 0.0f); };
	virtual MODEL::IMAGE2D* GetImage_2D(){ return nullptr; }

	// 手軽に扱いたいためpublicとする
public:
	// 共通
	D3DXMATRIX m_World;
	float m_alpha;
	float m_luminance;	// 輝度
	MODEL_TYPE m_Type;
	float m_Refractive;	// 屈折率
	// アニメーション関連。
	UINT m_NumBoneMatricesMax = 0;
	D3DXMATRIXA16 m_pBoneMatrices[BoneMatrixMax];
	// 境界線使用関連。
	bool m_IsHorizon = false;
	bool m_IsVerticesNumHorizon;	// 頂点ごとに境界線を指定するかのフラグ。
	vector<float> m_Horizon;	// 頂点ごとに境界線を設定する場合使用する。
	float m_HorizonSingle;	// 境界線をすべて共通のものにする場合使用する。
protected:
	CHAR m_pFileName[MAX_FILENAME + 1];		// 読み込むファイルの名前を格納する
	D3DXMATRIX m_PintoWorld;
	D3DXVECTOR3 m_PintoPos;
};

