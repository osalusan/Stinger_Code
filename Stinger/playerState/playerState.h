#pragma once
#include "playerState/playerStateMachine.h"

class Player;
class GameObject;
// �v���C���[�X�e�[�g�̊��N���X
class PlayerState
{
protected:
	PlayerStateMachine* m_PlayerMachine = nullptr;		// �X�e�[�g�}�V���ɏ���`������悤��
	const Player* m_PlayerCache = nullptr;				// �v���C���[�̃f�[�^�𒼐ڕύX�ł��Ȃ��悤�� / �ύX�̓X�e�[�g�}�V���ōs��

	bool m_IsLoad = false;				// ��x�����ǂݍ��ނ���
	std::string m_AnimName = {};
	bool m_IsLoadAnimation = false;		// �A�j���[�V��������x�����ǂݍ��ނ���
	float m_CurrentTime = 0.0f;			// �o�ߎ���
	float m_MaxAnimTime = 0.0f;			// �A�j���[�V�����ő厞��
	float m_BlendTime = 0.0f;			// ���[�V�����u�����h���x
	float m_AnimSpeedValue = 1.0f;		// �A�j���[�V�������x / 1.0�����{

	// 2��ڈȍ~�͓����Œe��
	void LoadAnimation(const std::string& fileName, const std::string& animationName);
	// �J�����̌����ɉ�]
	void RotToCameraDirection(const float& deltaTime);
	// �J�������猩�āA�L�[�̓��͕����Ɍ���
	void RotToInputKeyDirection(const float& deltaTime,const bool& rotAway = false);
	// �^�[�Q�b�g�̌����ɉ�]
	void RotToTarget(const GameObject* obj,const float& deltaTime);
	// �v���C���[�N���X�œǂݍ��񂾃f�[�^���擾
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