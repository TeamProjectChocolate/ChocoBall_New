#pragma once

#include "stdafx.h"
#include "Assert.h"
#include "Infomation.h"
#include "Render.h"
#include "Animation.h"

enum MODEL_TYPE{ T_2D = 0,T_3D };

class CModel
{
public:
	CModel(){
		m_alpha = 1.0f;
		m_luminance = 0.0f;
		m_Refractive = 1.000293f;/*�n���̑�C�̋��ܗ�*/
	};
	~CModel(){};
	virtual void Initialize();
	virtual void Update(const TRANSFORM&);
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

	// 3D�p
	virtual CAnimation* GetAnimation(){ return nullptr; }
	virtual void SetUseBorn(bool){}
	virtual bool GetUseBorn(){ return false; }
	virtual void SetCurrentAnimNo(int){};
	virtual int GetCurrentAnimNo(){ return 0; }
	virtual const D3DXMATRIX& GetRotation(){ return D3DXMATRIX(); }
	virtual IMAGE3D* GetImage_3D(){ return nullptr; }

	// 2D�p
	virtual void SetSplit(const D3DXVECTOR2&){}
	virtual const D3DXVECTOR2& GetSplit(){ return D3DXVECTOR2(0.0f, 0.0f); };
	virtual void SetNow(const D3DXVECTOR2&){}
	virtual const D3DXVECTOR2& GetNow(){ return D3DXVECTOR2(0.0f, 0.0f); };
	virtual IMAGE2D* GetImage_2D(){ return nullptr; }

	// ��y�Ɉ�����������public�Ƃ���
public:
	// ����
	D3DXMATRIX m_World;
	float m_alpha;
	float m_luminance;	// �P�x
	MODEL_TYPE m_Type;
	float m_Refractive;	// ���ܗ�
protected:
	CHAR m_pFileName[MAX_FILENAME + 1];		// �ǂݍ��ރt�@�C���̖��O���i�[����
	D3DXMATRIX m_PintoWorld;
	D3DXVECTOR3 m_PintoPos;
};

