#pragma once
#include <unordered_map>
#include <string>

enum class ANIMETION_MODEL
{
	PLAYER = 0,
	MAWJLAYGO,
	MAX
};

// �O���錾
class FbxModelRenderer;

class FbxModelManager final
{
private:
	static std::unordered_map<ANIMETION_MODEL, FbxModelRenderer*> m_LoadModelPool;
	static std::unordered_map<ANIMETION_MODEL, std::string> m_ReservModelPool;
	static std::unordered_map<std::string, ANIMETION_MODEL> m_ReservAnimationPool;
	static std::unordered_map<std::string, std::string> m_ReservAnimationNamePool;		// ����̓V�[���X�V���ɏ��������Ȃ�

	static void LoadModel(const ANIMETION_MODEL& model, const std::string& fileName);
	static void LoadAnimation(const ANIMETION_MODEL& model, const std::string& fileName, const std::string& animationName);
public:
	static void Init();
	static void Uninit();
	static void ReservModel(const ANIMETION_MODEL& model, const std::string& fileName);
	static void ReservAnimation(const ANIMETION_MODEL& model, const std::string& fileName, const std::string& animationName);
	static FbxModelRenderer* GetAnimationModel(const ANIMETION_MODEL& model);
};