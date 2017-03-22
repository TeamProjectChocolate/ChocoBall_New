#pragma once
#include "stdafx.h"
#include "btBulletDynamicsCommon.h"
#include "CollisionType.h"
#include "Infomation.h"

class CGameObject;

class CBulletPhysics;

// 当たり判定や物理挙動に用いるクラスのインターフェース。
// テンプレートメソッドパターンの規定クラスとして使用。
class CCollisionInterface
{
public:
	CCollisionInterface();
	virtual ~CCollisionInterface();
	// パラメータを参照してコリジョンを生成。
	// 引数：	 この剛体を持つオブジェクトのポインタ。
	//			 コリジョンに設定するトランスフォーム情報。
	//			 モデルの原点とコリジョンの中心の差分。
	//			 外部で定義したコリジョンの形状。
	//			 コリジョンの属性(内部でBulletPhysicsのUserIndexに登録される)。
	//			 質量(isKinematicがtrueならば自動的に0.0fになる)。
	//			 フラグ(継承先で詳細決定)。
	//			 生成した後この関数内でワールドに登録するか(trueで登録)。
	// ※このクラスでは大まかな処理の方向性と共通処理のみ実装。
	void InitCollision(
		CGameObject* pObject,
		const SH_ENGINE::TRANSFORM& Transform,
		const D3DXVECTOR3& offset,
		btCollisionShape* pShape,
		CollisionType Type,
		float mass,
		bool,
		bool isAddWorld);

	virtual void Update(D3DXVECTOR3* pos, D3DXQUATERNION* rot) = 0;

	// 全レイヤーマスクオフ。
	// すべての衝突を無視。
	__inline void BitMask_AllOff() {
		SetLayerMask(0x00000000);
	}

	// 全レイヤーマスクオン。
	// すべてのコリジョンと当たり判定を行う。
	__inline void BitMask_AllOn() {
		SetLayerMask(btBroadphaseProxy::CollisionFilterGroups::AllFilter);
	}

	// 指定したコリジョンタイプのレイヤーマスクオフ。
	// 指定したコリジョンとの衝突無視。
	__inline void BitMask_Off(CollisionType type) {
		// type桁目のビットを0にしてマスクをオフにする。
		int mask = 0x00000001;	// 0x00000001。
		// type桁分左シフトしていったん目的のbitを1にする。
		mask = mask << static_cast<int>(type);	// 0x00000001。
		// すべてのbitを反転し、目的のビットのみ0、他は1にする。
		mask = ~mask;	// 0xfffffffe。

		// AND演算で最終的なマスクbitを算出。
		SetLayerMask(GetLayerMask() & mask);
	}

	// 指定したコリジョンタイプのレイヤーマスクオン。
	__inline void BitMask_On(CollisionType type) {
		// type桁目のビットを1にしてマスクをオフにする。
		int mask = 0x00000001;	// 0x00000001。
		// type桁分左シフト。
		mask = mask << static_cast<int>(type);

		// OR演算で元のマスクに加算。
		SetLayerMask(GetLayerMask() | mask);
	}


	// 外部からコリジョンの属性を設定。
	// ※剛体やコリジョンオブジェクトへの属性設定は必ずこの関数で行ってください。
	// ※直接btCollisionObjectのsetUserIndex関数を呼ばないように！
	__inline void ConfigCollisionType(CollisionType type){
		// コリジョンタイプ形式で保存。
		SetMyType(type);
		// bit形式に変換して保存。
		int bitType = TypeToBitType(type);
		SetFilterGroup(bitType);
		// BulletPhysicsに自分の属性を設定。
		ConfigCollisionFilterGroup();
	}


	// このコリジョンをワールドに追加する。
	virtual void AddWorld() = 0;

	// このコリジョンをワールドから削除する。
	virtual void RemoveWorld() = 0;


	// コリジョンオブジェクト本体を返却。
	inline btCollisionObject* GetCollision() const {
		return m_collisionObject.get();
	}

	// レイヤーマスク返却。
	// 戻り値はbit形式のマスク。
	inline int GetLayerMask() const{
		return m_LayerMask;
	}

	// コリジョン本体に位置情報設定。
	inline void SetPos(const D3DXVECTOR3& pos) {
		m_collisionObject->getWorldTransform().setOrigin(btVector3(pos.x, pos.y, pos.z));
	}

	// コリジョン形状本体にコリジョンサイズ設定。
	inline void SetScale(const D3DXVECTOR3& scale) {
		m_collisionObject->getCollisionShape()->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
	}

protected:

	// レイヤーマスクを渡す。
	__inline void ConfigCollisionFilterMask() {
		if (m_collisionObject) {
			// コリジョンオブジェクトが生成されている。
			btBroadphaseProxy* Proxy = m_collisionObject->getBroadphaseHandle();
			if (Proxy) {
				// ブロードフェーズハンドルが生成されている。
				if (m_MyType == CollisionType::Map || m_MyType == CollisionType::Player) {
					OutputDebugString("マップもしくはプレイヤーのレイヤーマスク設定。");
				}
				// キャラクター剛体とキネマティック剛体もあたりを取る。
				Proxy->m_collisionFilterMask = m_LayerMask;
			}
		}
	}

	// コリジョンの属性を渡す。
	__inline void ConfigCollisionFilterGroup() {
		if (m_collisionObject) {
			// コリジョンオブジェクトが生成されている。
			// コリジョンオブジェクトにそのままの属性を設定。
			m_collisionObject->setUserIndex(static_cast<int>(m_MyType));
			btBroadphaseProxy* Proxy = m_collisionObject->getBroadphaseHandle();
			if (Proxy) {
				// ブロードフェーズハンドルが生成されている。
				if (m_MyType == CollisionType::Map || m_MyType == CollisionType::Player) {
					OutputDebugString("マップもしくはプレイヤーのフィルターグループ設定。");
				}
				Proxy->m_collisionFilterGroup = m_MyBitGroup;
			}
		}
	}


private:
	// 実際にコリジョンオブジェクトを生成する関数。
	// 継承先のクラスに詳細処理を委譲。
	virtual void Build(
		const btTransform& transform,
		float mass,
		bool) = 0;


	// 受け取ったコリジョンの属性をビット上での属性に変換して返却。
	__inline int TypeToBitType(CollisionType type) {
		//if (static_cast<int>(type) >= 10) {
		//	// マスクに設定できるのはデフォルトのマスクを除いて26bitまで。
		//	//abort();
		//	type = CollisionType::Boss_Cource;
		//}
		int ret = 0x00000001;
		// タイプの数字分左シフト。
		ret = ret << static_cast<int>(type);
		return ret;
	}


	// コリジョンの属性を属性形式で保存。
	inline void SetMyType(CollisionType type) {
		//if (static_cast<int>(type) >= 9) {
		//	// マスクに設定できるのはデフォルトのマスクを除いて26bitまで。
		//	OutputDebugString("タイプがオーバー。");
		//	//abort();
		//}
		m_MyType = type;
	}
	// コリジョンの属性をbit形式で保存。
	// 引数はbit形式。
	// ※この関数を呼んだら引数の値がフィルターグループとしてBulletPhysicsに通知されます。
	__inline void SetFilterGroup(int bit) {
		m_MyBitGroup = bit;
		ConfigCollisionFilterGroup();
	}

	// レイヤーマスクセット。
	// 引数はbit形式のマスク。
	// ※この関数を呼んだら引数の値がレイヤーマスクとしてBulletPhysicsに通知されます。
	__inline void SetLayerMask(int mask) {
		m_LayerMask = mask;
		ConfigCollisionFilterMask();
	}
protected:
	//ここからbulletPhysicsの剛体を使用するために必要な変数。
	btVector3 m_OriginOffset;		// オブジェクトの中心と剛体の中心の差分。
	unique_ptr<btCollisionShape>	m_collisionShape;	//コリジョンの形状。
	unique_ptr<btCollisionObject>		m_collisionObject;		//剛体。
	CollisionType m_MyType = CollisionType::None;
	// このコリジョンの属性(bit)。
	// ※コリジョンをワールドに登録したときにはじめて通知する。
	int m_MyBitGroup = 0x0000;
	// あたりを無視する属性をbit情報で格納(衝突する属性のbitに1を立てる)。
	// ※コリジョンをワールドに登録したときにはじめて通知する。
	int m_LayerMask = btBroadphaseProxy::CollisionFilterGroups::AllFilter;
	CBulletPhysics* m_pBulletPhysics;	// 物理ワールドとコリジョンワールドを持つクラス。
	protected:
		static int testRemove;
		static int testDelete;
		int addTest = 0;
};
