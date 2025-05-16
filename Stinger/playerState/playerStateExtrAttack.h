#pragma once
#include "playerState.h"

// �O���錾
class ObjectManager;
class BossEnemy;
class PlayerCamera;
class ExtrSlashEffect;
class ExtrSwordBarst;
// �G�N�X�g���U���̃X�e�[�g
class PlayerStateExtrAttack final :public PlayerState
{
private:
	BossEnemy* m_BossCache = nullptr;					// �_���[�W�ʒm�p
	PlayerCamera* m_PlayerCameraCache = nullptr;		// �J�����̈ʒu�����p
	ExtrSlashEffect* m_ExtrSlashEfCache = nullptr;		// �r���{�[�h�̃G�t�F�N�g
	ExtrSwordBarst* m_ExtrSwordBarstCache = nullptr;	// ���̃I�[���̃p�[�e�B�N��

	// ����p�p�����[�^
	bool m_AttackAccept = false;
	bool m_AttackEnable = false;
	bool m_ChageSound = false;
	float m_CurrentMoveTime = 0.0f;
	XMFLOAT3 m_MoveSpeedVec = {};

	// �A�j���[�V��������p�p�����[�^
	bool m_StopAnim = false;
	float m_CurrentStopAnimTime = 0.0f;

	// CSV����̓ǂݍ���
	float m_DamageValue = 0.0f;
	float m_CutInTimeMax = 0.0f;
	float m_CutInTimeMin = 0.0f;
	float m_MoveTimeMax = 0.0f;
	float m_StartStopAnimTimeValue = 0.0f;
	float m_StopAnimTimeValue = 0.0f;
	float m_SlowAnimSpeedValue = 0.0f;

public:
	using PlayerState::PlayerState;
	virtual void Init()override;
	virtual void Unit()override;
	virtual void Update(const float& deltaTime)override;
	virtual void ChangeStateControl()override;
	bool CheckAttackAccept();

	const bool& GetAttackEnable()const
	{
		return m_AttackEnable;
	}
};