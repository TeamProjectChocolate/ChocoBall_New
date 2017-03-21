#pragma once
#include "CollisionInterface.h"

class CCollisionObject :
	public CCollisionInterface
{
public:
	CCollisionObject();
	~CCollisionObject()override;
	void Update(D3DXVECTOR3* pos, D3DXQUATERNION* rot)override;

	// このコリジョンオブジェクトをコリジョンワールドに追加する。
	void AddWorld()override;
	// このコリジョンオブジェクトをコリジョンワールドから削除する。
	void RemoveWorld()override;
private:
	// パラメータを参照して剛体生成。
	// 引数：オブジェクトのトランスフォーム情報。
	//		 質量(isKinematicがtrueならば自動的に0.0fになる)。
	//		 使用されていません。
	void Build(
		const btTransform& transform,
		float mass,
		bool flg)override;
};

