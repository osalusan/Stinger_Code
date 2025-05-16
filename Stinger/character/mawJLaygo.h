#pragma once
#include "bossEnemy.h"
#include <unordered_map>

enum class MAWJ_ANIMATION
{
	NONE = 0,
	ROARING
};

// �O���錾
struct BONE;
// MawJLaygo�̓����蔻��̐����⃂�f�����Ǘ����Ă���N���X
class MawJLaygo final:public BossEnemy
{
private:
	// �g�p�A�j���[�V�����Ǘ��p
	std::unordered_map<MAWJ_ANIMATION, std::string> m_MawJAnimationName = {};

	virtual void CustomCollisionInfo()override;
	virtual void CollisionControl()override;
	virtual void AnimationControl()override;
public:
	using BossEnemy::BossEnemy;

	virtual ~MawJLaygo()override;
	virtual void Init()override;


	const std::string GetMawJAnimationName(const MAWJ_ANIMATION& name)
	{
		return m_MawJAnimationName[name];
	}
};