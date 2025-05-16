#pragma once
#include "character/character.h"
#include <unordered_map>

class Camera;
class PlayerStateMachine;
class BoxCollisionComponent;
class EquipmentObject;
class Polygon2D;
// �v���C���[�̓����蔻��̐����⃂�f�����Ǘ����Ă���N���X
class Player final:public Character {
private:

	BoxCollisionComponent* m_BoxCollCache = nullptr;			// �v���C���[���g�̓����蔻��
	Polygon2D* m_PlayerHpCache = nullptr;						// �v���C���[�̗͕̑\�L / HP���ω��������݂̂ɔ��f������ׁA�v���C���[�ŊǗ�
	PlayerStateMachine* m_PlayerStateMachine = nullptr;			// �v���C���[�X�e�[�g�}�V�[��

	// �v���C���[�̑���
	EquipmentObject* m_SwordChache = nullptr;					// �U���̏������v���C���[�̃p�����[�^�ōs�����߁A�v���C���[�ŊǗ�
	EquipmentObject* m_ShiledChache = nullptr;					// �p���B�̏������v���C���[�ōs���ׁA�v���C���[�ŊǗ�
	BoxCollisionComponent* m_SwordBoxCollCache = nullptr;		// ���̓����蔻��̂� / �������邽�ь��̃I�u�W�F�N�g����GetComponent���Ȃ��Ă����悤��

	// CSV����ǂݍ��񂾃X�e�[�^�X�������ɕۑ�
	std::unordered_map<std::string, std::unordered_map<std::string, float>> m_PlayerStateData = {};

	bool m_ChengeAnimation = false;				// �A�j���[�V���������ς����邩�ǂ���			
	float m_CurTakeNoDmgTime = 0.0f;			// �_���[�W������Ȃ��ł��鎞��
	float m_HealStartTime = 0.0f;				// �_���[�W������Ȃ��ł������́A�񕜊J�n���� 
	float m_HealSpeed = 0.0f;					// �񕜑��x

	virtual void MoveControl(const float& deltaTime)override;
	virtual void CustomCollisionInfo()override;
	virtual void CollisionControl()override;
	virtual void AnimationControl()override;

	void SwordCollisionControl();								// ���̓����蔻�萧��ƓG�Ƀ_���[�W��^���鏈��
	void PlayerDataLoadCSV(const std::string& filePath);		// CSV����v���C���[�p�����[�^�̎擾
public:
	Player() = delete;
	Player(const XMFLOAT3& pos);
	virtual ~Player()override;
	virtual void Init()override;
	virtual void Uninit()override;
	// ��𒆂̓_���[�W���󂯂Ȃ��悤�� / �G�l�~�[���p���B�s�̍U�����g�p����ꍇ�́A���̏������Ă�
	virtual void TakeDamage(const float& atk)override;
	// �p���B���̓_���[�W���󂯂Ȃ��悤�� / �G�l�~�[���p���B�\�ȍU�����g�p����ꍇ�́A���̏������Ă�
	void TakeDamageParryPossible(const float& atk);
	// �G�l�~�[�̕��ŏ��ɓ������Ă�����_���[�W�����ł͂Ȃ��A���̏������ĂԂ悤��
	void HitShiled(BoxCollisionComponent* boxColl);

	const PlayerStateMachine* GetPlayerStateMachine()
	{
		return m_PlayerStateMachine;
	}
	const XMFLOAT3& GetVelocity()const
	{
		return m_Velocity;
	}
	EquipmentObject* GetShiledCache()
	{
		return m_ShiledChache;
	}	
	EquipmentObject* GetSwordChache()const
	{
		return m_SwordChache;
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
	// �e�v���C���[�X�e�[�g�ŁA�Z�̖��O���w�肵�ăX�e�[�^�X���擾����
	const std::unordered_map<std::string, float>& GetStatusData(const std::string& skillName)const
	{
		if (m_PlayerStateData.count(skillName) >= 1)
		{
			return m_PlayerStateData.at(skillName);
		}
		// �e���|�����x�����폜����ׂɍ쐬
		static const std::unordered_map<std::string, float> emptyMap = {};
		return emptyMap;
	}
};