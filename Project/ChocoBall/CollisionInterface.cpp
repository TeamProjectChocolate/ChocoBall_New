#include "stdafx.h"
#include "CollisionInterface.h"
#include "ObjectManager.h"
#include "BulletPhysics.h"
#include "Infomation.h"

CCollisionInterface::CCollisionInterface()
{
	// オブジェクトマネージャーからBulletPhysicsクラスのインスタンスを保存。
	m_pBulletPhysics = SINSTANCE(CObjectManager)->FindGameObject<CBulletPhysics>(_T("BulletPhysics"));
	m_transform.Identity();
}

CCollisionInterface::~CCollisionInterface()
{
	// ※以下のコメントアウトしてある関数は純粋仮想関数となっており、継承先で上書きされている。
	//   しかし継承クラスのデストラクタから先に呼ばれるため、基底クラスのデストラクタが呼ばれるときにはすでに関数本体のメモリ領域は解放されている。
	//   このため基底クラスにこの処理を書いた場合は未解決シンボルとなりコンパイルエラーとなる。
	//   ポリモーフィズムは活用できない。
	// ※また以下の関数を内部で呼び出す他の関数も基底クラスでは呼ばない。
	//   呼んでしまった場合はコンパイルエラーとならずプログラムが実行され、基底クラスのデストラクタが呼ばれた瞬間にメモリ外アクセスが発生する。

	//RemoveWorld();
}

void CCollisionInterface::InitCollision(
	CGameObject* pObject,
	const SH_ENGINE::TRANSFORM& Transform,
	const D3DXVECTOR3& offset,
	btCollisionShape* pShape,
	Collision::Type Type,
	Collision::FilterGroup group,
	float mass,
	bool flg,
	bool isAddWorld)
{
	// コリジョンとモデルの中心座標の差分を格納。
	m_OriginOffset = btVector3(offset.x, offset.y, offset.z);
	//ここで剛体の形状を決定。
	m_collisionShape.reset(pShape);

	// コリジョンの属性保存。
	SetCollisionType(Type);
	// フィルターグループ保存。
	SetFilterGroup(group);
	// デフォルトではすべての属性のコリジョンと当たり判定を取る。
	// ※ただしすべてのものと当たり判定を行うと重いため、状況に応じて外部からマスクを設定してください。
	this->BitMask_AllOn();

	btTransform rbTransform;
	rbTransform.setIdentity();
	rbTransform.setOrigin(btVector3(Transform.position.x, Transform.position.y, Transform.position.z) + m_OriginOffset);
	rbTransform.setRotation(btQuaternion(Transform.angle.x, Transform.angle.y, Transform.angle.z, Transform.angle.w));

	// Transform情報を保存。
	m_transform = Transform;
	m_transform.scale = D3DXVECTOR3(pShape->getLocalScaling());
	{
		// 純粋仮想関数。
		Build(rbTransform, mass, flg);
		if (isAddWorld) {
			// 純粋仮想関数。
			// 生成したコリジョンをワールドに登録。
			AddWorld();
		}
	}

	// コリジョンを持つオブジェクトのポインタを設定。
	m_collisionObject->setUserPointer(pObject);
	m_collisionObject->setUserIndex(static_cast<int>(m_MyType));
	m_collisionObject->setActivationState(DISABLE_DEACTIVATION);
	m_collisionObject->activate();
}

