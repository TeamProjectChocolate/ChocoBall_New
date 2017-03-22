#pragma once

//namespace {
	enum class CollisionType {
		None = -1,
		Player = 6,			//�v���C���[�̃R���W�����B
		Map,				//�n�`�R���W�����B
		Chocoball,		//�`���R�{�[���B
		ChocoballTrigger,	//�`���R�{�[���N���R���W�����B
		Camera,			//�J�����̃R���W����
		Wall,				//�`���R�ǂ̃R���W����
		Floor,
		Boss,
		Boss_Gost,
		Boss_Cource,
		Boss_Barrier,
		AttackWall,		// �{�X�������������`���R�ǁB
		Bullet,				//�e�̃R���W����
		Enemy,
		Max
	};
//}