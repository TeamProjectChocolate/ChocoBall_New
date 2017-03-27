#pragma once

namespace Collision{
	// コリジョンの属性。
	// こちらは衝突リアクションの際などに参照する。
	enum class Type {
		None = -1,
		Player = 0,			//プレイヤーのコリジョン。
		Map,				//地形コリジョン。
		Chocoball,		//チョコボール。
		ChocoballTrigger,	//チョコボール起動コリジョン。
		Camera,			//カメラのコリジョン。
		Wall,				//チョコ壁のコリジョン。
		Floor,		// 移動床、落ちる床、上昇床。
		Boss,	// ボス。
		Boss_Gost,	// ボスコース判定用。
		Boss_Cource,	// ボスのコース定義。
		Boss_Barrier,	// バリア。
		AttackWall,		// ボスが発生させたチョコ壁。
		Bullet,				//弾のコリジョン。
		Enemy,	// 雑魚エネミー。
		BGM_Changer,	// BGM変更判定用。ボス戦以外で使うな。
		Max
	};

	enum class FilterGroup {
		All = -1,
		None = 0,
		Player = 6,	// キャラクター(プレイヤー)。
		Enemy,	// 敵キャラ(エネミー、ボスなど)。
		Camera,	// カメラ。
		Map,	// フィールドそのもの。
		Ghost,	// 衝突判定のみ(トリガー)。
		Boss_Ghost,	// ボスのゴースト。
		Boss_Cource,	// ボスのコース。
		Chocoball,	// チョコボール。
		Gimmick,	// こちらは壁や床のギミック。
		Barrier,		// バリア(一部のもののみ衝突判定する)。
		Max = 16	// これより先の数字はshortのサイズを超えるため使用できない。
	};
}