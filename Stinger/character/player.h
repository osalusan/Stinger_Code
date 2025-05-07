#pragma once
#include "character/character.h"
#include <unordered_map>

class Camera;
class PlayerStateMachine;
class BoxCollisionComponent;
class EquipmentObject;
class Polygon2D;

class Player final:public Character {
private:

	BoxCollisionComponent* m_BoxCollCache = nullptr;
	BoxCollisionComponent* m_SwordBoxCollCache = nullptr;
	Polygon2D* m_PlayerHpCache = nullptr;

	PlayerStateMachine* m_PlayerStateMachine = nullptr;

	EquipmentObject* m_ShiledChache = nullptr;
	EquipmentObject* m_SwordChache = nullptr;

	std::unordered_map<std::string, std::unordered_map<std::string, float>> m_PlayerStateData = {};

	bool m_ChengeAnimation = false;				// �A�j���[�V���������ς����邩�ǂ���
	bool m_UseAttack = false;
	float m_CurrentHealTime = 0.0f;
	float m_HealTime = 0.0f;
	float m_HealSpeed = 0.0f;

	virtual void MoveControl(const float& deltaTime)override;
	virtual void CustomCollisionInfo()override;
	virtual void CollisionControl()override;
	virtual void AnimationControl()override;
	void SwordCollisionControl();
	void PlayerDataLoadCSV(const std::string& filePath);
public:
	Player() = delete;
	Player(const XMFLOAT3& pos);
	virtual ~Player()override;
	virtual void Init()override;
	virtual void Uninit()override;

	virtual void TakeDamage(const float& atk)override;
	void TakeDamageParryPossible(const float& atk);
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

	const std::unordered_map<std::string, float>& GetStateData(const std::string& skillName)const
	{
		if (m_PlayerStateData.count(skillName) >= 1)
		{
			return m_PlayerStateData.at(skillName);
		}
		// �e���|�����x���p
		static const std::unordered_map<std::string, float> emptyMap = {};
		return emptyMap;
	}
};