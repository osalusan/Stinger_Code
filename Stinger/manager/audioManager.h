#pragma once
#include <xaudio2.h>
#include <unordered_map>

enum class AUDIO
{
	MAWJ_BATTLE_BGM = 0,
	TITLE_BGM,
	PARRY_SE,
	SLASH1_SE,
	SLASH2_SE,
	SLASH3_SE,
	SOWRD_CHAGE,
	PLAYER_HIT_DAMAGE,
	MAX
};
// �Đ�����̂ɕK�v�ȃf�[�^
struct AUDIO_DATA
{
	IXAudio2SourceVoice* s_SourceVoice = nullptr;
	BYTE* s_SoundData = nullptr;
	int s_Length = 0;
	int s_PlayLength = 0;
};
// �ǂݍ��ݎ��ɕK�v�ȃf�[�^
struct AUDIO_RESERVE_DATA
{
	const char* s_FileName = {};
	bool m_PlaySoundOnStart = false;			// �ǂݍ���ł����ɉ���炷
	bool s_Loop = false;
};
// �Q�[�������ׂẲ����Ǘ�����N���X
class AudioManager final
{
private:
	static IXAudio2*				m_Xaudio;
	static IXAudio2MasteringVoice*	m_MasteringVoice;

	static std::unordered_map<AUDIO, AUDIO_DATA*> m_LoadAudioPool;
	static std::unordered_map<AUDIO, AUDIO_RESERVE_DATA> m_ReservAudioPool;

	static void LoadAudio(const AUDIO& audio,const char* FileName);
public:
	static void InitMaster();		// �Q�[�����N�����Ă����x�̂݌Ă�
	static void UninitMaster();		// �Q�[�����N�����Ă����x�̂݌Ă�
	static void Init();				// �\�񂳂ꂽ����Scene�̍Ō�Ɉꊇ�œǂݍ���
	static void Uninit();
	static void ReservAudio(const AUDIO& audio, const char* fileName,const bool& startSound = false,const bool& loopSound = false);

	static void Play(const AUDIO& audio,const bool& Loop = false,const float& volume = 1.0f);
	static void Finish(const AUDIO& audio);
};

