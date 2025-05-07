#pragma once
#include "object/gameObject.h"

#define DEFAULT_BLEND_VALUE (8.0f)

// �O���錾
enum class ANIMETION_MODEL;

class MeshFiled;
class Character :public GameObject
{
protected:
	XMFLOAT3 m_Velocity= {};
	XMFLOAT3 m_RecordPosition = {};					// �ߋ����W
	ANIMETION_MODEL m_Model;						// ���f���{�� / �R���X�g���N�^�ŏ�����

	MeshFiled* m_MeshFiled = nullptr;

	XMFLOAT3 m_WorldWall = {};						// �ړ����E
	// �d��
	bool m_EnableGravity = false;
	float m_GravityValue = 0.0f;
	float m_CurrentGravityValue = 0.0f;

	// ���ʂ̃p�����[�^
	float m_Health = 0;								// �̗�
	float m_MaxHealth = 0;							// �ő�̗�
	float m_RotSpeed = 0.0f;						// ��]���x
	float m_Attack = 0.0f;							// �U����
	float m_MoveSpeed = 0.0f;						// �ړ����x
	bool m_IsDead = false;							// ����

	// ���o�I���t���O
	bool m_FinishPerformance = false;

	// �A�j���[�V�����p
	float m_AnimationTime = 0.0f;					// �A�j���[�V�����̃t���[��
	float m_NextAnimationTime = 0.0f;				// ���̃A�j���[�V�����̃t���[��
	std::string m_AnimationName = "";				// �A�j���[�V�����̖��O
	std::string m_NextAnimationName = "";			// �u�����h�p�A���̃A�j���[�V�����̖��O
	float m_BlendRatio = 0.0f;						// �A�j���[�V�����u�����h�̐��l
	float m_BlendTimeValue = 0.0f;					// �ǂ̂��炢�u�����h�𑁂��s����
	float m_MaxAnimeTime = 0.0f;					// ���݂̃A�j���[�V�����̍ő厞��
	float m_AnimSpeedValue = 1.0f;					// �A�j���[�V�����̑��x�ύX�p

	virtual void MoveControl(const float& deltaTime) = 0;
	virtual void CollisionControl() = 0;
	virtual void CustomCollisionInfo() = 0;
	virtual void AnimationControl() = 0;

	virtual void ParameterControl(const float& deltaTime);

	void ReservModel(const ANIMETION_MODEL& animeModel, const std::string& path);
	void CheckWorldWallPos();		// ���[���h�͈̔͊O�ɏo����͈͓��ɖ߂�
public:
	Character();
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override final;
	virtual void Draw()override;

	virtual void TakeDamage(const float& atk) = 0;
	void InitGravity();
	void IsDead();

	void ChangeAnimation(const std::string& anime)
	{
		m_NextAnimationName = anime;
	}

	void SetAnimeTime(const float& time)
	{
		m_AnimationTime = time;
	}
	void SetNextAnimationTime(const float& time)
	{
		m_NextAnimationTime = time;
	}
	void SetHealth(const float& health)
	{
		m_Health = health;
	}
	void SetBlendTimeValue(const float& value)
	{
		m_BlendTimeValue = value;
	}
	void SetAnimationSpeedValue(const float& value)
	{
		m_AnimSpeedValue = value;
	}
	const ANIMETION_MODEL& GetAnimeModel()const
	{
		return m_Model;
	}
	const std::string& GetAnimeName()const
	{
		return m_AnimationName;
	}
	const std::string& GetNextAnimeName()const
	{
		return m_NextAnimationName;
	}
	const float& GetAnimeTime()const
	{
		return m_AnimationTime;
	}
	const float& GetBlendRatio()const
	{
		return m_BlendRatio;
	}
	const float& GetMaxAnimeTime()const
	{
		return m_MaxAnimeTime;
	}
	
	// �p�����[�^��Get
	const float& GetHealth()const
	{
		return m_Health;
	}	
	const float& GetMaxHealth()const
	{
		return m_MaxHealth;
	}
	const float& GetAttack()const
	{
		return m_Attack;
	}
	const float& GetMoveSpeed()const
	{
		return m_MoveSpeed;
	}
	const float& GetRotSpeed()const
	{
		return m_RotSpeed;
	}
	const bool& GetIsDead()const
	{
		return m_IsDead;
	}
};