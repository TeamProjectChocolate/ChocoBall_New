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

	void Draw();

	// 全レイヤーマスクオン。
	__inline void BitMask_AllOn() {
		SetLayerMask(0x80000000);
		// マスク設定。
		ConfigCollisionFilterMask();
	}

	// 全レイヤーマスクオフ。
	// すべてのコリジョンと当たり判定を行う。
	__inline void BitMask_AllOff() {
		SetLayerMask(btBroadphaseProxy::CollisionFilterGroups::AllFilter);
		// マスク設定。
		ConfigCollisionFilterMask();
	}

	// 指定したコリジョンタイプのレイヤーマスクオン。
	__inline void BitMask_On(CollisionType type) {
		if (static_cast<int>(type) >= 26) {
			// マスクに設定できるのはデフォルトのマスクを除いて26bitまで。
			abort();
		}
		// type桁目のビットを0にしてマスクをオンにする。
		int mask = 0x00000001;	// 0x00000001。
		// type桁分左シフトしていったん目的のbitを1にする。
		mask = mask << static_cast<int>(type);	// 0x00000001。
		// すべてのbitを反転し、目的のビットのみ0、他は1にする。
		mask = ~mask;	// 0xfffffffe。

		//// 最上位ビットは符号ビットのため0にする。
		//int work = 0xffffffff;
		//mask = mask & work;	// 0x7ffffffe。

		if (m_LayerMask != -1) {
			int test = m_LayerMask | mask;	// 消したいbitが1だった場合はtestの値は0xffffffffとなる。
			if (test != 0xffffffff) {
				// 消したいbitがもともと0だった。
				return;
			}
		}
		else {
			//// 0xffffffffとなっているため、符号ビット以外を立ててから編集。
			//m_LayerMask = 0x7fffffff;
			// 設定時にあらかじめBulletPhysics内で宣言されていた分は左シフトするため、こちらでは逆の処理を行う。
			m_LayerMask = (m_LayerMask >> 1) / btBroadphaseProxy::CharacterFilter;
		}
		// AND演算で最終的なマスクbitを算出。
		SetLayerMask(GetLayerMask() & mask);
		// マスク設定。
		ConfigCollisionFilterMask();
	}

	// 指定したコリジョンタイプのレイヤーマスクオフ。
	__inline void BitMask_Off(CollisionType type) {
		if (static_cast<int>(type) >= 26) {
			// マスクに設定できるのはデフォルトのマスクを除いて26bitまで。
			abort();
		}
		// type桁目のビットを1にしてマスクをオフにする。
		int mask = 0x00000001;	// 0x00000001。
		// type桁分左シフト。
		mask = mask << static_cast<int>(type);
		// OR演算で元のマスクに加算。
		SetLayerMask(GetLayerMask() | mask);
		// マスク設定。
		ConfigCollisionFilterMask();
	}


	// 外部からコリジョンの属性を設定。
	// ※剛体やコリジョンオブジェクトへの属性設定は必ずこの関数で行ってください。
	// ※直接btCollisionObjectのsetUserIndex関数を呼ばないように！
	__inline void ConfigCollisionType(CollisionType type){
		// コリジョンタイプ形式で保存。
		SetMyType(type);
		// bit形式に変換して保存。
		int bitType = TypeToBitType(type);
		SetMyBitGroup(bitType);
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
				Proxy->m_collisionFilterMask = m_LayerMask;
			}
		}
	}

	// コリジョンの属性を渡す。
	__inline void ConfigCollisionFilterGroup() {
		// コリジョンオブジェクトにそのままの属性を設定。
		m_collisionObject->setUserIndex(static_cast<int>(m_MyType));

		if (m_collisionObject) {
			// コリジョンオブジェクトが生成されている。
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
		if (static_cast<int>(type) >= 26) {
			// マスクに設定できるのはデフォルトのマスクを除いて26bitまで。
			abort();
		}
		int ret = 0x0001;
		// タイプの数字分左シフト。
		ret = ret << static_cast<int>(type);
		return ret;
	}


	// コリジョンの属性を属性形式で保存。
	inline void SetMyType(CollisionType type) {
		m_MyType = type;
	}
	// コリジョンの属性をbit形式で保存。
	__inline void SetMyBitGroup(int bit) {
		// 内部的にされている値は使用できないため、其の分左シフト変換して設定。
		if (bit != btBroadphaseProxy::CollisionFilterGroups::AllFilter && bit != 0x80000000/*0*/) {
			// BulletPhysicsで既に使用されている列挙体を上書きするため、元の列挙子の最高値に2以上をかけて一つ左のbitを立てる。
			bit = (bit << 1) * btBroadphaseProxy::CharacterFilter;
		}
		m_MyBitGroup = bit;
	}
	// レイヤーマスクセット。
	// 引数はbit形式のマスク。
	__inline void SetLayerMask(int mask) {
		if (mask != btBroadphaseProxy::CollisionFilterGroups::AllFilter && mask != 0x80000000/*0*/) {
			// BulletPhysicsで既に使用されている列挙体を上書きするため、元の列挙子の最高値に2以上をかけて一つ左のbitを立てる。
			mask = (mask << 1) * btBroadphaseProxy::CharacterFilter;
		}
		//// 符号ビットを立ててから代入。
		//mask = mask | 0x8000;
		m_LayerMask = mask;
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
};

