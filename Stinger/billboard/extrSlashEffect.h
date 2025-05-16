#pragma once
#include "billboard.h"

// �O���錾
class Camera;
// �G�N�X�g���X���b�V���̃r���{�[�h�G�t�F�N�g
class ExtrSlashEffect :public BillBoard
{
private:
	const GameObject* m_FollowObject = nullptr;
	const GameObject* m_HitEnemyCache = nullptr;
	const Camera* m_CameraCache = nullptr;

public:
	ExtrSlashEffect() = delete;
	ExtrSlashEffect(const GameObject* followObj);
	virtual void Update(const float& deltaTime)override;
	// �쐬���ԓI�ɁA�ォ��i�[���Ȃ��Ƃ����Ȃ�
	void SetEnemyPointer(const GameObject* hitEnemy);
};