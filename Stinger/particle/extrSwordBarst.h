#pragma once
#include "particleEmitter.h"

// �O���錾
class EquipmentObject;

class ExtrSwordBarst final :public ParticleEmiter
{
private:
	const EquipmentObject* m_PlayerSwordCache = nullptr;

	virtual void CreateParticleEffect(const float& deltaTime)override;
	virtual void UpdateParticleEffect(const float& deltaTime)override;

	virtual void ReservTexture()override {};
public:
	using ParticleEmiter::ParticleEmiter;
};