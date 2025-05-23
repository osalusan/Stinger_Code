#pragma once
#include "renderer/renderer.h"
#include <unordered_map>
// 読み込んでいるテクスチャの一覧
enum class TEXTURE
{
	TITLE = 0,
	PARTICLE_ORIGIN,
	GROUND_TIGGER_ROCK_TEX,
	GROUND_TIGGER_ROCK_NOR,
	GROUND_GREEN_TEX,
	GROUND_GREEN_NOR,
	LOADING,
	WHITE,
	BLACK,
	SHADOW,
	HPBAR_PLAYER,
	HPBAR_BOSS,
	HPBAR_BG_BOSS,
	HPBAR_FRAME_BOSS,
	EFFECT_LIGHTNING_BALL,
	EFFECT_LIGHTNING_FALL,
	PUSHSPACEKEY,
	GAME_CLEAR,
	GAME_OVER,
	RETRY,
	GO_TITLE,
	SELECTBACK1,
	SELECTBACK2,
	EFFECT_SLASH,
	PARRYGAGE,
	BATTLESTART_TEXT,
	MAX
};
// ゲーム内すべてのテクスチャを管理するクラス
class TextureManager 
{
private:
	static std::unordered_map<TEXTURE, ID3D11ShaderResourceView*> m_LoadTexturePool;
	static std::unordered_map<TEXTURE, const wchar_t*> m_ReservTexturePool;
	static std::unordered_map<TEXTURE, const wchar_t*> m_ReservDDSTexturePool;

	static void LoadTexture(const TEXTURE& textureName, const wchar_t* fileName);
	static void LoadDDSTexture(const TEXTURE& textureName, const wchar_t* fileName);
public:
	static void Init();			// 予約されたテクスチャをSceneの最後に一括で読み込む
	static void Uninit();
	// pngとjpgのみ読み込み可
	static void ReservTexture(const TEXTURE& texture, const wchar_t* fileName);
	// ddsのみ読み込み可
	static void ReservDDSTexture(const TEXTURE& texture, const wchar_t* fileName);
	static ID3D11ShaderResourceView*& GetTexture(const TEXTURE& texture);
};