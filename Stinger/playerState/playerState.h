#pragma once
#include "playerState/playerStateMachine.h"

class Player;
class GameObject;

class PlayerState
{
protected:
	PlayerStateMachine* m_PlayerMachine = nullptr;
	const Player* m_PlayerCache = nullptr;

	bool m_Load = false;
	std::string m_AnimName = {};
	bool m_LoadAnimation = false;
	float m_CurrentTime = 0.0f;
	float m_MaxAnimTime = 0.0f;
	float m_BlendTime = 0.0f;
	float m_AnimSpeedValue = 1.0f;	// �A�j���[�V�������x

	// 2��ڈȍ~�͓����Œe��
	void LoadAnimation(const std::string& fileName, const std::string& animationName);
	// �J�����̌����ɉ�]
	void RotToCameraDirection(const float& deltaTime);
	// �J�������猩�āA�L�[�̓��͕����Ɍ���
	void RotToInputKeyDirection(const float& deltaTime,const bool& rotAway = false);
	// �^�[�Q�b�g�̌����ɉ�]
	void RotToTarget(const GameObject* obj,const float& deltaTime);

	float FindStateData(const std::unordered_map<std::string, float>& stateData,const std::string& dataName);
public:
	virtual ~PlayerState();
	PlayerState(PlayerStateMachine* machine);
	void ChangePlayerState(const PLAYER_STATE& playerState);

	virtual void Init() = 0;
	virtual void Unit() = 0;
	virtual void Update(const float& deltaTime);
	virtual void ChangeStateControl() = 0;		// �X�e�[�g�̐؂�ւ�����
};