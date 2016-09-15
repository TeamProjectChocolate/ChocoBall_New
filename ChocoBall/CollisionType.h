#pragma once

enum CollisionType{
	CollisionType_Player,			//プレイヤーのコリジョン。
	CollisionType_Map,				//地形コリジョン。
	CollisionType_Chocoball,		//チョコボール。
	CollisionType_ChocoballTrigger,	//チョコボール起動コリジョン。
	CollisionType_Camera,			//カメラのコリジョン
	CollisionType_Wall,				//チョコ壁のコリジョン
	CollisionType_Bullet,				//弾のコリジョン
	CollisionType_Enemy
};