#pragma once

namespace Collision{
	// �R���W�����̑����B
	// ������͏Փ˃��A�N�V�����̍ۂȂǂɎQ�Ƃ���B
	enum class Type {
		None = -1,
		Player = 0,			//�v���C���[�̃R���W�����B
		Map,				//�n�`�R���W�����B
		Chocoball,		//�`���R�{�[���B
		ChocoballTrigger,	//�`���R�{�[���N���R���W�����B
		Camera,			//�J�����̃R���W�����B
		Wall,				//�`���R�ǂ̃R���W�����B
		Floor,		// �ړ����A�����鏰�A�㏸���B
		Boss,	// �{�X�B
		Boss_Gost,	// �{�X�R�[�X����p�B
		Boss_Cource,	// �{�X�̃R�[�X��`�B
		Boss_Barrier,	// �o���A�B
		AttackWall,		// �{�X�������������`���R�ǁB
		Bullet,				//�e�̃R���W�����B
		Enemy,	// �G���G�l�~�[�B
		BGM_Changer,	// BGM�ύX����p�B�{�X��ȊO�Ŏg���ȁB
		Max
	};

	enum class FilterGroup {
		All = -1,
		None = 0,
		Player = 6,	// �L�����N�^�[(�v���C���[)�B
		Enemy,	// �G�L����(�G�l�~�[�A�{�X�Ȃ�)�B
		Camera,	// �J�����B
		Map,	// �t�B�[���h���̂��́B
		Ghost,	// �Փ˔���̂�(�g���K�[)�B
		Boss_Ghost,	// �{�X�̃S�[�X�g�B
		Boss_Cource,	// �{�X�̃R�[�X�B
		Chocoball,	// �`���R�{�[���B
		Gimmick,	// ������͕ǂ⏰�̃M�~�b�N�B
		Barrier,		// �o���A(�ꕔ�̂��̂̂ݏՓ˔��肷��)�B
		Max = 16	// �������̐�����short�̃T�C�Y�𒴂��邽�ߎg�p�ł��Ȃ��B
	};
}