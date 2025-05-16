#pragma once
#include "main/main.h"
#include <unordered_map>
#include <string>

enum class PLAYER_STATE
{
	NONE = 0,
	IDLE,
	HITDAMAGE,
	ROLLING,
	ATTACK_PARRY,
	ATTACK_NORMAL,
	ATTACK_EXTR,
	RUN,
	DEAD,
	MAX
};

enum class MOVE_DIRECTION
{
	NONE = 0,
	RIGHT,
	LEFT,
	FORWARD,
	BACKWARD,
	MAX
};
// �O���錾
class PlayerState;
class Player;
class Camera;
class PlayerStateParryAttack;
class PlayerStateRolling;
class PlayerStateNormalAttack;
class PlayerStateExtrAttack;
class PlayerStateHitDamage;
class Polygon2D;
class BossEnemy;
// �v���C���[�̃X�e�[�g���Ǘ�����N���X
class PlayerStateMachine final
{
private:
	Player* m_PlayerCache = nullptr;						// �X�e�[�g�̐��l���v���C���[�ɓ`����
	Camera* m_CameraCache = nullptr;						// �J�����̃x�N�g���擾�p
	Polygon2D* m_ParryGageCache = nullptr;					// �p���B�������̃Q�[�W����p
	BossEnemy* m_BossCache = nullptr;						// �_���[�W�ʒm�p

	PlayerState* m_CurrentPlayerState = nullptr;			// ���݂̃X�e�[�g
	PlayerStateParryAttack* m_ParryCache = nullptr;			// �p���B����p
	PlayerStateNormalAttack* m_NormalAttackCache = nullptr;	// �U������p
	PlayerStateExtrAttack* m_ExtrAttackCache = nullptr;		// �U������p

	PlayerStateRolling* m_RollingCache = nullptr;			// ��𒆂̗͑͂�����Ȃ������̈�
	PLAYER_STATE m_CurrentState = PLAYER_STATE::NONE;
	std::unordered_map<PLAYER_STATE, PlayerState*> m_PlayerStatePool = {};	

	// �ړ������L�^�p
	MOVE_DIRECTION m_RandL = MOVE_DIRECTION::NONE;	// �E�����̂ǂ��炩�Ɉړ����Ă���
	MOVE_DIRECTION m_FandB = MOVE_DIRECTION::NONE;	// �O�ƌ��̂ǂ��炩�Ɉړ����Ă���

	XMFLOAT3 m_Velocity = {};
	XMFLOAT3 m_Rotation = {};

	float m_AnimeBlendTimeValue = 0.0f;				// �A�j���[�V�����̃u�����h���x
	int m_ParryCount = 0;							// �p���B�J�E���g

	bool m_IsGround = false;						// �n�ʂɐG��Ă��邩
	bool m_IsJamp = false;							// �W�����v������
	bool m_IsParryAttackButton = false;				// �p���B�U����������
	bool m_IsRollingButton = false;					// ��]������
	bool m_IsNormalAttackButton = false;			// �ʏ�U����������
	bool m_IsHitAttacked = false;					// �U�����󂯂���
	bool m_IsInvincible = false;					// ���G��Ԃ��ǂ���
	bool m_IsExtrAttack = false;					// �G�N�X�g���U���������\��
	bool m_IsParry = false;							// �p���B���Ă��邩

	std::string m_NextAnimationName = "";			// �A�j���[�V�����̖��O

	// ���͏��̏�����
	void InputReset();
public:
	PlayerStateMachine() = delete;
	PlayerStateMachine(Player* player);
	~PlayerStateMachine();
	void Init();
	void Uninit();
	void Update(const float& deltaTime);

	void SetPlayerState(const PLAYER_STATE& state);
	void InitVelocity();
	void SetAnimationSpeedValue(const float& value);	// �A�j���[�V�����̑��x�ύX / �X�e�[�g�ύX���Ƀ��Z�b�g

	bool CheckParry();									// �p���B�����m�F
	bool CheckRolling();								// ��𒆂��m�F
	bool CheckAttack();									// �U�����̊m�F
	XMFLOAT3 GetCameraForward()const;
	XMFLOAT3 GetCameraRight()const;
	void HitGround();									// �n�ʂɓ�������

	void UsedExtrAttack();								// �G�N�X�g���U�����g�p������Ƀf�[�^�����Z�b�g

	const Player* GetPlayerCache()const
	{
		return m_PlayerCache;
	}
	const PLAYER_STATE& GetCurrentState()const
	{
		return m_CurrentState;
	}

	const XMFLOAT3& GetVelocity()const
	{
		return m_Velocity;
	}
	const XMFLOAT3& GetRotation()const
	{
		return m_Rotation;
	}

	const MOVE_DIRECTION& GetMoveRandL()const
	{
		return m_RandL;
	}
	const MOVE_DIRECTION& GetMoveFandB()const
	{
		return m_FandB;
	}
	const bool& GetIsJump()const
	{
		return m_IsJamp;
	}
	const bool& GetIsGround()const
	{
		return m_IsGround;
	}
	const bool& GetIsParryAttackButton()const
	{
		return m_IsParryAttackButton;
	}
	const bool& GetIsNormalAttackButton()const
	{
		return m_IsNormalAttackButton;
	}
	const bool& GetIsRollingButton()const
	{
		return m_IsRollingButton;
	}
	const bool& GetIsInvincible()const
	{
		return m_IsInvincible;
	}
	const std::string& GetAnimation()const
	{
		return m_NextAnimationName;
	}
	const bool& GetIsHitAttack()const
	{
		return m_IsHitAttacked;
	}	
	const bool& GetIsExtrAttack()const
	{
		return m_IsExtrAttack;
	}

	void SetIsHitAttack(const bool& hit)
	{
		m_IsHitAttacked = hit;
	}
	void SetAnimation(const std::string& anime)
	{
		m_NextAnimationName = anime;
	}
	void SetVelocityX(const float& x)
	{
		m_Velocity.x = x;
	}
	void SetVelocityZ(const float& z)
	{
		m_Velocity.z = z;
	}
	void SetVelocityY(const float& y)
	{
		m_Velocity.y = y;
	}
	void SetRotationY(const float& y)
	{
		m_Rotation.y = y;
	}
	void SetAnimeBlendTimeValue(const float& value)
	{
		m_AnimeBlendTimeValue = value;
	}
};