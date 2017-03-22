#pragma once

//namespace {
	enum class CollisionType {
		None = -1,
		Player = 6,			//プレイヤーのコリジョン。
		Map,				//地形コリジョン。
		Chocoball,		//チョコボール。
		ChocoballTrigger,	//チョコボール起動コリジョン。
		Camera,			//カメラのコリジョン
		Wall,				//チョコ壁のコリジョン
		Floor,
		Boss,
		Boss_Gost,
		Boss_Cource,
		Boss_Barrier,
		AttackWall,		// ボスが発生させたチョコ壁。
		Bullet,				//弾のコリジョン
		Enemy,
		Max
	};
//}