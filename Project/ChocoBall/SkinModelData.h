#pragma once

#include "stdafx.h"
#include "Infomation.h"
#include "Animation.h"


class CSkinModelData
{
public:
	CSkinModelData();
	~CSkinModelData();
	void LoadModelData(LPCSTR,CAnimation*);
	void SetUpBoneMatrixPointers(LPD3DXFRAME, LPD3DXFRAME);
	void SetUpBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER,LPD3DXFRAME);
	/*!
	* @brief	���f���f�[�^�̃N���[�����쐬�B
	* @details
	*	�X�P���g���͐V�����쐬����A���b�V���f�[�^�͋��ʂ̂��̂��g�p����܂��B
	*@param[in]		modelData		���f���f�[�^�B
	*/
	void CloneModelData(const CSkinModelData& modelData, CAnimation* anim);
	LPD3DXFRAME GetFrameRoot()
	{
		return m_frameRoot;
	}
	ID3DXAnimationController* GetAnimationController()
	{
		return m_pAnimController;
	}
	ANIMATION::D3DXMESHCONTAINER_DERIVED* GetContainer(){
		return m_pMeshContainer;
	}
	void UpdateBoneMatrix(const D3DXMATRIX* matWorld);
	void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, const D3DXMATRIX* pParentMatrix);

	// ���̃��[���h�s��𖼑O�Ō����B
	// ����: �{�[���̖��O�B
	// ���������d�����ߕp�ɂɌĂ΂Ȃ��B
	D3DXMATRIX* FindBoneWorldMatrix(LPCSTR name);
private:
	// ���̃��[���h�s��𖼑O�Ō����B
	// ����: �{�[���̖��O�B
	//		 �t���[���B
	D3DXMATRIX* FindBoneWorldMatrix(LPCSTR name,LPD3DXFRAME frame);

	/*!
	* @brief	�X�P���g���̃N���[�����쐬�B
	*@param[out]	dstFrame		�N���[����̃X�P���g���̊i�[��B
	*@param[in]		srcFrame		�N���[�����̃X�P���g���B
	*/
	void CloneSkeleton(LPD3DXFRAME& dstFrame, LPD3DXFRAME srcFrame);
	// �A�j���[�V�����R���g���[���[�ɃA�j���[�V��������{�[�����m�[�h��ǂ��ēo�^���Ă����֐��B
	// �����f����ǂݍ��񂾂Ƃ��͎����ōs���邽�߁A�蓮�ŌĂяo���K�v������̂̓��b�V���R���e�i�[�̃N���[����������Ƃ��B
	void SetUpOutputAnimationRegist(LPD3DXFRAME frame, ID3DXAnimationController* animCtr);
	/*!
	* @brief	�I���W�i���̃X�P���g�����폜�B
	*@param[in]	frame	�X�P���g���B
	*/
	void DeleteSkeleton(LPD3DXFRAME frame);
	/*!
	* @brief	�X�P���g���̃N���[�����폜�B
	*@param[in]	frame		�폜����X�P���g���B
	*/
	void DeleteCloneSkeleton(LPD3DXFRAME frame);
	void InnerDestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase);
private:
	LPD3DXFRAME m_frameRoot = NULL;		// �t���[�����[�g
	ID3DXAnimationController* m_pAnimController;	// �A�j���[�V�����R���g���[��
	ANIMATION::D3DXMESHCONTAINER_DERIVED* m_pMeshContainer;
	bool m_isClone;	// ���̃��f���f�[�^�̓N���[�����B
};

