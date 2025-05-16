#pragma once
class Polygon2D;
// ��{�I�ȃt�F�[�h / ����ȃt�F�[�h�����ꍇ�͊��N���X�𑝂₷
class Fade final
{
private:
	Polygon2D* m_FadeTexture = nullptr;

	bool m_IsFadeIn = false;		// �t�F�[�h�C����
	bool m_IsFadeOut = false;		// �t�F�[�h�A�E�g��
	float m_CurrentTime = 0.0f;		// �t�F�[�h�̌o�ߎ���

public:
	~Fade();
	void Init();
	void Uninit();
	void Update(const float& deltaTime);
	void Draw();

	void StartFadeIn();					// �t�F�[�h�C�����n�߂������ɌĂ�
	void StartFadeOut();				// �t�F�[�h�̊Ǘ����s���Ă��鏊�ŁAGetFadeInComplete()��true�ɂȂ������ɁA������ĂԂ悤��

	const bool GetFadeOutComplete();	// �o�ߎ��Ԃ���t�F�[�h�A�E�g�����̔�����s��
	const bool GetFadeInComplete();		// �o�ߎ��Ԃ���t�F�[�h�C�������̔�����s��
}; 
