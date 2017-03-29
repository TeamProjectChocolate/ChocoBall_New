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
	//			 このコリジョンのフィルターグループ(Collision::FilterGroupの値を設定)。
	//			 質量(isKinematicがtrueならば自動的に0.0fになる)。
	//			 フラグ(継承先で詳細決定)。
	//			 生成した後この関数内でワールドに登録するか(trueで登録)。
	// ※このクラスでは大まかな処理の方向性と共通処理のみ実装。
	void InitCollision(
		CGameObject* pObject,
		const SH_ENGINE::TRANSFORM& Transform,
		const D3DXVECTOR3& offset,
		btCollisionShape* pShape,
		Collision::Type Type,
		Collision::FilterGroup group,
		float mass,
		bool,
		bool isAddWorld);

	virtual void Update(D3DXVECTOR3* pos, D3DXQUATERNION* rot);

	// 物理ワールドで衝突したら呼ばれるコールバック(XZ方向)。
	void OnCollisionStay_XZ(CCollisionInterface* pCollision);
	// 物理ワールドで衝突したら呼ばれるコールバック(Y方向)。
	void OnCollisionStay_Y(CCollisionInterface* pCollision);
	// コリジョンワールドで衝突したら呼ばれるコールバック。
	void OnTriggerStay(CCollisionInterface* pCollision);

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
	__inline void BitMask_Off(Collision::FilterGroup group) {
		// type桁目のビットを0にしてマスクをオフにする。
		int mask = 0x00000001;	// 0x00000001。
		// type桁分左シフトしていったん目的のbitを1にする。
		mask = mask << static_cast<int>(group);	// 0x00000001。
		// すべてのbitを反転し、目的のビットのみ0、他は1にする。
		mask = ~mask;	// 0xfffffffe。

		// AND演算で最終的なマスクbitを算出。
		SetLayerMask(GetLayerMask() & mask);
	}

	// 指定したコリジョンタイプのレイヤーマスクオン。
	__inline void BitMask_On(Collision::FilterGroup group) {
		// type桁目のビットを1にしてマスクをオフにする。
		int mask = 0x00000001;	// 0x00000001。
		// type桁分左シフト。
		mask = mask << static_cast<int>(group);

		// OR演算で元のマスクに加算。
		SetLayerMask(GetLayerMask() | mask);
	}

	// コリジョンをアクティベート。
	inline void Activate() {
		m_collisionObject->activate();
	}
	// このコリジョンをワールドに追加する。
	virtual void AddWorld() = 0;

	// このコリジョンをワールドから削除する。
	virtual void RemoveWorld() = 0;


	// このコリジョンを持つゲームオブジェクトを返却。
	inline CGameObject* GetGameObject() const
	{
		return m_GameObject;
	}
	// 横あたりかを取得。
	inline bool GetIsCollisionXZ() const {
		return m_OnCollisionXZ;
	}
	// 縦あたりかを取得。
	inline bool GetIsCollisionY() const {
		return m_OnCollisionY;
	}

	// コリジョンの属性設定。
	// ※この引数がbtCollisionObjectのuserIndexに設定される。
	// ※コリジョンへの属性設定は必ずこの関数で行うように。
	inline void SetCollisionType(Collision::Type type) {
		m_MyType = type;
		if (m_collisionObject) {
			m_collisionObject->setUserIndex(static_cast<int>(type));
		}
	}
	// コリジョンの属性取得。
	inline Collision::Type GetCollisionType() const
	{
		return m_MyType;
	}

	// ※剛体やコリジョンオブジェクトへのフィルターグループ設定は必ずこの関数で行ってください。
	// ※直接btCollisionObjectの値を変更しないように！。
	inline void SetFilterGroup(Collision::FilterGroup group) {
		m_UserFilterGroup = group;
		// bit形式に変換して保存。
		m_FilterGroup = TypeToBit(group);
		// BulletPhysicsに自分の属性を設定。
		ConfigCollisionFilterGroup();
	}
	// フィルターグループ取得。
	inline Collision::FilterGroup SetFilterGroup() const
	{
		return m_UserFilterGroup;
	}

	// コリジョンオブジェクト本体を返却。
	inline const btCollisionObject* GetCollisionObject() const {
		return m_collisionObject.get();
	}

	// コリジョン形状を返却。
	inline btCollisionShape* GetCollisionShape() const
	{
		return m_collisionShape.get();
	}

	// レイヤーマスク返却。
	// 戻り値はbit形式のマスク。
	inline int GetLayerMask() const {
		return m_LayerMask;
	}

	// Transform情報取得。
	inline const SH_ENGINE::TRANSFORM& GetTransform() const{
		return m_transform;
	}

	// コリジョン本体に位置情報設定。
	inline void SetPos(const D3DXVECTOR3& pos) {
		m_transform.position = pos;
		m_collisionObject->getWorldTransform().setOrigin(btVector3(pos.x, pos.y, pos.z));
	}
	// コリジョン本体の位置情報(中心点)返却。
	inline const D3DXVECTOR3& GetPos() const {
		return 	m_transform.position;
	}

	// コリジョン本体の回転情報取得。
	inline const D3DXQUATERNION& GetRotation()const {
		return m_transform.angle;
	}

	// コリジョン形状本体にコリジョンサイズ設定。
	inline void SetScale(const D3DXVECTOR3& scale) {
		m_transform.scale = scale;
		m_collisionObject->getCollisionShape()->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
	}
	// コリジョン形状本体にコリジョンサイズ返却(半分)。
	inline const D3DXVECTOR3& GetScale()const {
		return 	m_transform.scale;
	}

protected:

	// レイヤーマスクを渡す。
	__inline void ConfigCollisionFilterMask() {
		if (m_collisionObject) {
			// コリジョンオブジェクトが生成されている。
			btBroadphaseProxy* Proxy = m_collisionObject->getBroadphaseHandle();
			if (Proxy) {
				// ブロードフェーズハンドルが生成されている。
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
				Proxy->m_collisionFilterGroup = m_FilterGroup;
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
	__inline int TypeToBit(Collision::FilterGroup group) {
		//if (static_cast<int>(type) >= 10) {
		//	// マスクに設定できるのはデフォルトのマスクを除いて26bitまで。
		//	//abort();
		//	type = CollisionType::Boss_Cource;
		//}
		int ret = 0x00000001;
		// タイプの数字分左シフト。
		ret = ret << static_cast<int>(group);
		return ret;
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
	Collision::Type m_MyType = Collision::Type::None;	// コリジョンの属性。
	Collision::FilterGroup m_UserFilterGroup = Collision::FilterGroup::None;	// コリジョンのフィルターグループ(ユーザーが設定)。
	int m_FilterGroup = 0x00000000;	// コリジョンのフィルターグループ(このクラス内でのみ使用)。
	// あたりを無視する属性をbit情報で格納(衝突する属性のbitに1を立てる)。
	// ※コリジョンをワールドに登録したときにはじめて通知する。
	int m_LayerMask = btBroadphaseProxy::CollisionFilterGroups::AllFilter;
protected:
	CBulletPhysics* m_pBulletPhysics;	// 物理ワールドとコリジョンワールドを持つクラス。
	bool m_OnCollisionXZ = false;	// XZ方向で衝突したか。
	bool m_OnCollisionY = false;	// Y方向で衝突したか。

	CGameObject* m_GameObject = nullptr;
	SH_ENGINE::TRANSFORM m_transform;
};
