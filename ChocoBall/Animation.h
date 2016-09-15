#pragma once
#include "stdafx.h"


class CAnimation
{
public:
	CAnimation(){
		m_pAnimController = nullptr;
		m_numAnimSet = 0;
		m_currentAnimationSetNo = -1;
		m_oldAnimationSetNo = -1;
		m_currentTrackNo = 0;
		m_numMaxTracks = 0;
		m_isBlending = false;
		m_isInterpolate = false;
		m_interpolateEndTime = 0.0f;
		m_interpolateTime = 0.0f;
	};
	~CAnimation();

	void Initialize(ID3DXAnimationController*);
	void Update(float);

	// アニメーション再生関数
	// 引き数: int型 再生したいアニメーションのインデックス
	void PlayAnimation(int);

	// アニメーション再生関数
	// 引き数: int型 再生したいアニメーションのインデックス
	//		　 float型 補完時間
	void PlayAnimation(int, float);

	// アニメーション再生速度を設定
	void SetAnimSpeed(float speed);

	//アニメーションの終了タイムを設定する関数
	//引き数:int型 アニメーションインデックス
	//      :double型 アニメーションの終了タイム
	//※-1.0を指定するとデフォルトの終了時間が設定されます
	void SetAnimationEndtime(int animationIndex, double endTime){
		m_animationEndTime[animationIndex] = endTime;
	}
#if 0
	// アニメーションのブレンディング再生
	// 引き数: int型 再生したいアニメーションのインデックス
	void BlendAnimation(int);
#endif
	int GetNumAnimationSet() const{
		return m_numAnimSet;
	}
	bool IsHasAnimationController() const
	{
		return m_pAnimController;
	}
private:
	// アニメーション用メンバ変数
	ID3DXAnimationController* m_pAnimController;	// アニメーションコントローラ
	int m_numAnimSet;	// アニメーションセットの数(モーションの種類数)
	unique_ptr<ID3DXAnimationSet*[]> m_AnimationSets;	// アニメーションセットの配列
	unique_ptr<float[]> m_blendRateTable;		// ブレンディングレートのテーブル
	int m_currentAnimationSetNo;			// 現在再生中のアニメーションセットの番号
	int m_oldAnimationSetNo;				// 1フレーム前のアニメーションセットの番号
	int m_currentTrackNo;			// 現在のトラックの番号
	int m_numMaxTracks;				// アニメーショントラックの最大数
	bool m_isBlending;				// アニメーションブレンディング中かどうかのフラグ
	bool m_isInterpolate;		    // 補間中かのフラグ
	float m_interpolateEndTime;		// 補間終了時間
	float m_interpolateTime;		// 補間時間
	unique_ptr<double[]> m_animationEndTime;	// アニメーションの終了タイム。デフォルト値は-1。指定しない場合はID3DXAnimationSetのアニメーション終了タイムが優先される。
	float m_localAnimationTime;	// ローカルアニメーションタイム
};

