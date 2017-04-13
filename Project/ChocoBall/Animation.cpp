#include "stdafx.h"
#include "Animation.h"
#include "Assert.h"


CAnimation::~CAnimation()
{
}

void CAnimation::Initialize(ID3DXAnimationController* anim){
	m_pAnimController = anim;
	ID3DXAnimationSet* animSet;
	m_pAnimController->GetAnimationSet(0, &animSet);
	m_numAnimSet = m_pAnimController->GetMaxNumAnimationSets();
	m_numMaxTracks = m_pAnimController->GetMaxNumTracks();
	m_blendRateTable.reset(new float[m_numMaxTracks]);
	m_animationEndTime.reset(new double[m_numAnimSet]);
	m_AnimationSets.reset(new ID3DXAnimationSet*[m_numAnimSet]);
	for (int i = 0; i < m_numMaxTracks; i++){
		m_blendRateTable[i] = 1.0f;
	}
	// アニメーションセットを初期化
	for (int i = 0; i < m_numAnimSet; i++){
		m_pAnimController->GetAnimationSet(i, &m_AnimationSets[i]);
		m_animationEndTime[i] = -1.0f;
	}
	m_localAnimationTime = 0.0f;
}

#if 0
void CAnimation::BlendAnimation(int animationSetIndex){
	if(m_pAnimController){
		isBlending = true;
		currentTrackNo++;
		currentTrackNo %= numAnimSet;
		m_pAnimController->SetTrackWeight(0,0.0f);
		m_pAnimController->SetTrackWeight(1,1.0f);
		m_pAnimController->SetTrackSpeed(0,1.0f);
		m_pAnimController->SetTrackSpeed(1,1.0f);
		m_pAnimController->SetTrackEnable(currentTrackNo,m_AnimationSets[animationSetIndex]);
	}
}
#endif

void CAnimation::Play(int animationSetIndex,bool IsPlayRepeat) {
	PlayAnimation(animationSetIndex,false ,0.0f,!IsPlayRepeat);
}

void CAnimation::Play(int animationSetIndex, float interpolateTime, bool IsPlayRepeat) {
	PlayAnimation(animationSetIndex, true, interpolateTime, !IsPlayRepeat);
}

void CAnimation::Stop(const unsigned int animationSetIndex) {
	// 現在のトラックを無効にする。
	m_pAnimController->SetTrackEnable(animationSetIndex, false);
}

void CAnimation::StopAll() {
	// すべてのトラックを無効にする。
	for (int i = 0; i < m_numMaxTracks; i++) {
		m_pAnimController->SetTrackEnable(i, false);
	}
}


//void CAnimation::PlayAnimation(int animationSetIndex,const bool IsPlayOnce) {
//	if (animationSetIndex < m_numAnimSet) {
//		if (m_pAnimController) {
//			m_oldAnimationSetNo = m_currentAnimationSetNo;
//			m_currentAnimationSetNo = animationSetIndex;
//			if (m_oldAnimationSetNo == m_currentAnimationSetNo) {
//				// 同じアニメーションを繰り返し再生しようとしていた場合は無視。
//				return;
//			}
//			m_IsOnceEnd = false;
//
//			// ループ再生しないかのフラグ。
//			m_IsPlayOnce = IsPlayOnce;
//
//
//			// 0番目以外のトラックは無効化する
//			for (int i = 1; i < m_numMaxTracks; i++) {
//				m_pAnimController->SetTrackEnable(i, false);
//			}
//			m_currentTrackNo = 0;
//			m_pAnimController->SetTrackWeight(0, 1.0f);
//			m_pAnimController->SetTrackAnimationSet(m_currentTrackNo, m_AnimationSets[m_currentAnimationSetNo]);
//			m_pAnimController->SetTrackEnable(0, true);
//			m_pAnimController->SetTrackSpeed(m_currentTrackNo, 1.0f);
//			m_pAnimController->SetTrackPosition(0, 0.0f);
//			m_localAnimationTime = 0.0;
//		}
//	}
//	else {
//		Assert(animationSetIndex < m_numAnimSet);
//	}
//}

void CAnimation::PlayAnimation(int animationSetIndex,const bool IsInterpolate, float interpolateTime,const bool IsPlayOnce){
	if (animationSetIndex <= -1) {
		abort();
	}

	if (animationSetIndex < m_numAnimSet){
		if (m_pAnimController){
			m_oldAnimationSetNo = m_currentAnimationSetNo;
			m_currentAnimationSetNo = animationSetIndex;
			if (m_oldAnimationSetNo == m_currentAnimationSetNo){
				// 同じアニメーションを繰り返し再生しようとしていた場合は無視。
				return;
			}
			m_IsOnceEnd = false;

			// 再生モードを指定。
			m_IsPlayOnce = IsPlayOnce;

			if (IsInterpolate) {
				// 補間する。
				// 補間中フラグをtrueにする。
				m_isInterpolate = true;
				m_interpolateEndTime = interpolateTime;

				m_currentTrackNo = (m_currentTrackNo + 1) % m_numMaxTracks;
				m_pAnimController->SetTrackEnable(m_currentTrackNo, true);
				m_pAnimController->SetTrackPosition(m_currentTrackNo, 0.0f);
			}
			else {
				// 補間しない。
				// 補間中フラグをfalseにする。
				m_isInterpolate = false;
				// 0番目以外のトラックは無効化する
				for (int i = 1; i < m_numMaxTracks; i++) {
					m_pAnimController->SetTrackEnable(i, false);
				}
				m_currentTrackNo = 0;
				m_pAnimController->SetTrackWeight(0, 1.0f);
				m_pAnimController->SetTrackEnable(0, true);
				m_pAnimController->SetTrackPosition(0, 0.0f);
			}

			m_pAnimController->SetTrackAnimationSet(m_currentTrackNo, m_AnimationSets[m_currentAnimationSetNo]);
			m_pAnimController->SetTrackSpeed(m_currentTrackNo, 1.0f);
			m_localAnimationTime = 0.0;
		}
	}
}

//void CAnimation::PlayAnimation_Once(int animationSetIndex){
//	if (animationSetIndex < m_numAnimSet){
//		if (m_pAnimController){
//			m_oldAnimationSetNo = m_currentAnimationSetNo;
//			m_currentAnimationSetNo = animationSetIndex;
//			if (m_oldAnimationSetNo == m_currentAnimationSetNo){
//				return;
//			}
//			if (m_currentAnimationSetNo <= -1) {
//				return;
//			}
//			m_IsOnceEnd = false;
//
//
//			m_IsPlayOnce = true;
//
//
//			// 0番目以外のトラックは無効化する
//			for (int i = 1; i < m_numMaxTracks; i++){
//				m_pAnimController->SetTrackEnable(i, false);
//			}
//			m_currentTrackNo = 0;
//			m_pAnimController->SetTrackWeight(0, 1.0f);
//			m_pAnimController->SetTrackAnimationSet(m_currentTrackNo, m_AnimationSets[m_currentAnimationSetNo]);
//			m_pAnimController->SetTrackEnable(0, true);
//			m_pAnimController->SetTrackSpeed(m_currentTrackNo, 1.0f);
//			m_pAnimController->SetTrackPosition(0, 0.0f);
//			m_localAnimationTime = 0.0;
//		}
//	}
//	else{
//		Assert(animationSetIndex < m_numAnimSet);
//	}
//}
//
//void CAnimation::PlayAnimation_Once(int animationSetIndex, float interpolateTime){
//	if (animationSetIndex < m_numAnimSet){
//		if (m_pAnimController){
//			m_oldAnimationSetNo = m_currentAnimationSetNo;
//			m_currentAnimationSetNo = animationSetIndex;
//			if (m_oldAnimationSetNo == m_currentAnimationSetNo){
//				return;
//			}
//			if (m_currentAnimationSetNo == -1){
//				return;
//			}
//			m_IsOnceEnd = false;
//
//
//			m_IsPlayOnce = true;
//
//
//			// 補間中フラグをtrueにする
//			m_isInterpolate = true;
//
//
//			this->m_interpolateTime = 0.0f;
//			m_interpolateEndTime = interpolateTime;
//			m_currentTrackNo = (m_currentTrackNo + 1) % m_numMaxTracks;
//			m_pAnimController->SetTrackAnimationSet(m_currentTrackNo, m_AnimationSets[m_currentAnimationSetNo]);
//			m_pAnimController->SetTrackEnable(m_currentTrackNo, true);
//			m_pAnimController->SetTrackSpeed(m_currentTrackNo, 1.0f);
//			m_pAnimController->SetTrackPosition(m_currentTrackNo, 0.0f);
//			m_localAnimationTime = 0.0;
//		}
//	}
//}

// アニメーション再生速度を設定
void CAnimation::SetAnimSpeed(float speed)
{
	if (m_pAnimController){
		m_pAnimController->SetTrackSpeed(m_currentTrackNo, speed);
	}
}

void CAnimation::Update(float deltaTime){
	m_localAnimationTime += deltaTime;

	if (m_animationEndTime[m_currentAnimationSetNo] > 0.0f	// アニメーションの終了時間が設定されている
		&& m_localAnimationTime > m_animationEndTime[m_currentAnimationSetNo]){	// アニメーションの終了時間を超えた
		if (m_IsPlayOnce){
			m_localAnimationTime -= deltaTime;
			// 一度再生し終わった。
			m_IsOnceEnd = true;
		}
		else{
			m_localAnimationTime -= m_animationEndTime[m_currentAnimationSetNo];
		}
		m_pAnimController->SetTrackPosition(m_currentTrackNo, m_localAnimationTime);
		m_pAnimController->AdvanceTime(0, nullptr);
	}
	else{
		// 普通に再生
		m_pAnimController->AdvanceTime(deltaTime, nullptr);
	}
	if (m_isInterpolate){
		// 補間中
		m_interpolateTime += deltaTime;
		float weight = 0.0f;
		if (m_interpolateTime > m_interpolateEndTime){
			// 補間終了
			m_isInterpolate = false;
			weight = 1.0f;
			// 現在のトラック以外を無効にする
			for (int i = 0; i < m_numMaxTracks; i++){
				if (i != m_currentTrackNo){
					m_pAnimController->SetTrackEnable(i, false);
				}
			}
		}
		else{
			weight = m_interpolateTime / m_interpolateEndTime;
			float invWeight = 1.0f - weight;
			// ウェイトを設定していく
			for (int i = 0; i < m_numMaxTracks; i++){
				if (i != m_currentTrackNo){
					m_pAnimController->SetTrackWeight(i, m_blendRateTable[i] * invWeight);
				}
				else{
					m_pAnimController->SetTrackWeight(i, weight);
				}
			}
		}
	}
}
