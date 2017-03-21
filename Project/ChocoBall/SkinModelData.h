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
	* @brief	モデルデータのクローンを作成。
	* @details
	*	スケルトンは新しく作成され、メッシュデータは共通のものが使用されます。
	*@param[in]		modelData		モデルデータ。
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

	// 骨のワールド行列を名前で検索。
	// 引数: ボーンの名前。
	// ※処理が重いため頻繁に呼ばない。
	D3DXMATRIX* FindBoneWorldMatrix(LPCSTR name);
private:
	// 骨のワールド行列を名前で検索。
	// 引数: ボーンの名前。
	//		 フレーム。
	D3DXMATRIX* FindBoneWorldMatrix(LPCSTR name,LPD3DXFRAME frame);

	/*!
	* @brief	スケルトンのクローンを作成。
	*@param[out]	dstFrame		クローン先のスケルトンの格納先。
	*@param[in]		srcFrame		クローン元のスケルトン。
	*/
	void CloneSkeleton(LPD3DXFRAME& dstFrame, LPD3DXFRAME srcFrame);
	// アニメーションコントローラーにアニメーションするボーンをノードを追って登録していく関数。
	// ※モデルを読み込んだときは自動で行われるため、手動で呼び出す必要があるのはメッシュコンテナーのクローンを作ったとき。
	void SetUpOutputAnimationRegist(LPD3DXFRAME frame, ID3DXAnimationController* animCtr);
	/*!
	* @brief	オリジナルのスケルトンを削除。
	*@param[in]	frame	スケルトン。
	*/
	void DeleteSkeleton(LPD3DXFRAME frame);
	/*!
	* @brief	スケルトンのクローンを削除。
	*@param[in]	frame		削除するスケルトン。
	*/
	void DeleteCloneSkeleton(LPD3DXFRAME frame);
	void InnerDestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase);
private:
	LPD3DXFRAME m_frameRoot = NULL;		// フレームルート
	ID3DXAnimationController* m_pAnimController;	// アニメーションコントローラ
	ANIMATION::D3DXMESHCONTAINER_DERIVED* m_pMeshContainer;
	bool m_isClone;	// このモデルデータはクローンか。
};

