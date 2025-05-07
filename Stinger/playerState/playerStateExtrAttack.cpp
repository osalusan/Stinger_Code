#include "playerStateExtrAttack.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "manager/fbxModelManager.h"
#include "manager/audioManager.h"
#include "manager/textureManager.h"
#include "polygon2D/polygon2D.h"
#include "renderer/fbxModelRenderer.h"
#include "scene/scene.h"
#include "character/bossEnemy.h"
#include "camera/playerCamera.h"
#include "billboard/extrSlashEffect.h"
#include "particle/extrSwordBarst.h"

constexpr float QUICK_ANIMATION_SPEED = 1.3f;

void PlayerStateExtrAttack::Init()
{
	m_CurrentTime = 0.0f;
	m_AttackAccept = false;
	m_StopAnim = false;
	m_CurrentStopAnimTime = 0.0f;
	m_CurrentMoveTime = 0.0f;
	m_AnimSpeedValue = 1.0f;
	m_MoveSpeedVec = {};
	m_AttackEnable = false;
	m_ChageSound = false;

	if (m_PlayerMachine == nullptr) return;

	m_PlayerMachine->UseExtrAttack();

	if (m_PlayerCache == nullptr)
	{
		// Get�̂� / �ҏW�s��
		const Player* playerCache = m_PlayerMachine->GetPlayerCache();
		m_PlayerCache = playerCache;
	}

	if (!m_Load && m_PlayerCache != nullptr)
	{
		LoadAnimation("asset\\model\\player\\swordAndShieldExtrAttack_PaladinJNordstrom.fbx", "extrAttack");

		AudioManager::ReservAudio(AUDIO::SOWRD_CHAGE, "asset\\audio\\se\\sowrdChage.wav");
		AudioManager::ReservAudio(AUDIO::SLASH3_SE, "asset\\audio\\se\\slash3.wav");

		const std::unordered_map<std::string, float>& parryAttak = m_PlayerCache->GetStateData("�G�N�X�g���U��");

		m_DamageValue = FindStateData(parryAttak, "�_���[�W_�{��");
		m_CutInTimeMin = FindStateData(parryAttak, "�J�b�g�C������_�ŏ�");
		m_CutInTimeMax = FindStateData(parryAttak, "�J�b�g�C������_�ő�");
		m_MoveTimeMax = FindStateData(parryAttak, "�ړ�����_�ő�");
		m_StartStopAnimTimeValue = FindStateData(parryAttak, "�J�b�g�C�����x����J�n����_����");
		m_StopAnimTimeValue = FindStateData(parryAttak, "�J�b�g�C�����x���䎞��_����");
		m_SlowAnimSpeedValue = FindStateData(parryAttak, "�X���[_�{��");

		Scene* scene = SceneManager::GetScene();
		if (scene == nullptr) return;
		ObjectManager* objManager = scene->GetObjectManager();
		if (objManager == nullptr) return;

		if (m_ExtrSlashEfCache == nullptr)
		{
			m_ExtrSlashEfCache = objManager->AddGameObjectArg<ExtrSlashEffect>(OBJECT::BILLBOARD, objManager->GetPlayer());
		}
		if (m_ExtrSwordBarstCache == nullptr)
		{
			m_ExtrSwordBarstCache = objManager->AddGameObjectArg<ExtrSwordBarst>(OBJECT::PARTICLE, false);
		}
		m_Load = true;
	}

	if (m_ObjManagerCache == nullptr)
	{
		Scene* scene = SceneManager::GetScene();
		if (scene == nullptr) return;
		m_ObjManagerCache = scene->GetObjectManager();
	}

	if (m_BossCache == nullptr)
	{
		if (m_ObjManagerCache != nullptr)
		{
			m_BossCache = m_ObjManagerCache->GetBossEnemy();
		}

		if (m_ExtrSlashEfCache != nullptr)
		{
			m_ExtrSlashEfCache->SetEnemyPointer(m_BossCache);
		}
	}

	if (m_CameraCache == nullptr)
	{
		if (m_ObjManagerCache != nullptr)
		{
			m_CameraCache = dynamic_cast<PlayerCamera*>(m_ObjManagerCache->GetCamera());
		}
	}	

	if (m_BossCache != nullptr)
	{
		// �{�X�Ɍ������Ĉړ�
		const XMFLOAT3& myPos = m_PlayerCache->GetPos();
		const XMFLOAT3& targetPos = m_BossCache->GetPos();

		m_MoveSpeedVec.x = (targetPos.x - myPos.x) / m_MoveTimeMax;
		m_MoveSpeedVec.z = (targetPos.z - myPos.z) / m_MoveTimeMax;

		// ����̂ݓ���Ȃ��悤��
		if (m_ExtrSwordBarstCache != nullptr)
		{
			m_ExtrSwordBarstCache->Start();
		}
	}

	// �J�b�g�C�����ɓG���~�܂�悤��
	if (m_ObjManagerCache != nullptr && m_PlayerMachine->GetIsExtrAttack())
	{
		m_ObjManagerCache->SetSlowTimeEnemy(((m_CutInTimeMax - m_StartStopAnimTimeValue) * m_StopAnimTimeValue) / m_SlowAnimSpeedValue + m_MoveTimeMax);
		m_ObjManagerCache->SetSlowValue(0.0f);
	}
}

void PlayerStateExtrAttack::Unit()
{
	m_CurrentTime = 0.0f;
}

void PlayerStateExtrAttack::Update(const float& deltaTime)
{
	PlayerState::Update(deltaTime);

	m_CurrentTime += deltaTime * m_AnimSpeedValue;

	if (m_MaxAnimTime == 0.0f)
	{
		if (FbxModelRenderer* model = FbxModelManager::GetAnimationModel(ANIMETION_MODEL::PLAYER))
		{
			m_MaxAnimTime = model->GetMaxAnimeTime(m_AnimName);
		}
	}

	// �J�b�g�C������
	if (m_CurrentTime >= m_MaxAnimTime * m_CutInTimeMin)
	{
		if (m_CameraCache != nullptr)
		{
			m_CameraCache->StartCutIn();

			if (!m_ChageSound && m_CurrentTime >= (m_MaxAnimTime * m_CutInTimeMax) * 0.5f)
			{
				m_ChageSound = true;
				AudioManager::Play(AUDIO::SOWRD_CHAGE, false, 0.75f);
			}
		}

		// �J�b�g�C���̎����������悤��
		if (m_CurrentMoveTime == 0.0f && m_BossCache != nullptr && m_PlayerCache != nullptr)
		{
			const XMFLOAT3& myPos = m_PlayerCache->GetPos();
			const XMFLOAT3& targetPos = m_BossCache->GetPos();
			const float& directionx = targetPos.x - myPos.x;
			const float& directionz = targetPos.z - myPos.z;

			float rotY = std::atan2(directionx, directionz);
			m_PlayerMachine->SetRotationY(rotY);
		}
	}

	if (m_PlayerMachine == nullptr) return;

	// �A�j���[�V�����̒�~
	if (m_CurrentStopAnimTime == 0.0f)
	{
		if (m_CurrentTime >= m_MaxAnimTime * m_StartStopAnimTimeValue)
		{
			m_AnimSpeedValue = m_SlowAnimSpeedValue;
			m_PlayerMachine->SetAnimationSpeedValue(m_AnimSpeedValue);
			m_StopAnim = true;
		}
	}

	// �J�b�g�C���I����
	if (m_StopAnim)
	{
		m_CurrentStopAnimTime += deltaTime;

		if (m_CurrentStopAnimTime >= m_StopAnimTimeValue)
		{
			m_AnimSpeedValue = 0.0f;
			m_PlayerMachine->SetAnimationSpeedValue(m_AnimSpeedValue);
			m_StopAnim = false;
			m_CameraCache->StartEnemyDirection();
		}
	}
	// �G�l�~�[�̎�O�܂ňړ�
	else
	{
		if (m_CurrentStopAnimTime >= m_StopAnimTimeValue)
		{
			m_CurrentMoveTime += deltaTime;
		}

		// �ړ�
		if (m_CurrentMoveTime != 0.0f && m_CurrentMoveTime <= m_MoveTimeMax)
		{
			m_PlayerMachine->SetVelocityX(m_MoveSpeedVec.x);
			m_PlayerMachine->SetVelocityZ(m_MoveSpeedVec.z);
		}
		else if (m_CurrentMoveTime >= m_MoveTimeMax)
		{
			m_AttackEnable = true;
			m_PlayerMachine->InitVelocity();
			m_AnimSpeedValue = QUICK_ANIMATION_SPEED;
			m_PlayerMachine->SetAnimationSpeedValue(m_AnimSpeedValue);
		}
	}
}

void PlayerStateExtrAttack::ChangeStateControl()
{
	if (m_CurrentTime >= m_MaxAnimTime)
	{		
		if (m_PlayerMachine != nullptr)
		{
			m_AnimSpeedValue = 1.0f;
			m_PlayerMachine->SetAnimationSpeedValue(m_AnimSpeedValue);
		}

		// �I���ݒ�
		if (m_CameraCache != nullptr)
		{
			m_CameraCache->EndCutIn();
		}
		if (m_ExtrSwordBarstCache != nullptr)
		{
			m_ExtrSwordBarstCache->End();
		}

		ChangePlayerState(PLAYER_STATE::IDLE);
	}
}

bool PlayerStateExtrAttack::CheckAttackAccept()
{
	// �J�b�g�C���I����ɔ�����o��
	if (!m_AttackAccept && m_CurrentTime >= m_MaxAnimTime * m_CutInTimeMin)
	{
		if (m_BossCache == nullptr || m_PlayerCache == nullptr)
		{
			return false;
		}

		m_BossCache->TakeDamage(m_PlayerCache->GetAttack() * m_DamageValue);
		m_BossCache->SetParryRecoil(true);
		
		m_AttackAccept = true;

		AudioManager::Play(AUDIO::SLASH3_SE, false, 0.85f);

		if (m_ExtrSwordBarstCache != nullptr)
		{
			m_ExtrSwordBarstCache->End();
		}
		if (m_ExtrSlashEfCache != nullptr)
		{
			m_ExtrSlashEfCache->UseBillboard();
		}
	}
	return m_AttackAccept;
}
