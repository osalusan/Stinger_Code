#pragma once
#include <unordered_map>
#include <string>

// �O���錾
struct MODEL;
struct MODEL_OBJ;
struct MODEL_MATERIAL;
class ObjModelRenderer;

enum class STATICMESH_MODEL
{
	SKYDOME_TITLE = 0,
	SKYDOME_BATTLE_MAWJ,
	BOX,
	BOX_CENTER,
	TREEGATE,
	COLLISION_BOX,
	RUINS,
	SOWRD,
	SHIELD,
	SPHERE,
	MAX
};

class ObjModelManager final
{
private:
	static std::unordered_map<STATICMESH_MODEL, ObjModelRenderer*> m_LoadModelPool;
	static std::unordered_map<STATICMESH_MODEL, std::string> m_ReservModelPool;

	static void LoadModel(const char* FileName, MODEL* Model);
	static void LoadObj(const char* FileName, MODEL_OBJ* ModelObj);
	static void LoadMaterial(const char* FileName, MODEL_MATERIAL** MaterialArray, unsigned int* MaterialNum);
	static void Load(const STATICMESH_MODEL& model,const std::string& fileName);
public:
	static void Init();
	static void Uninit();
	static void ReservModel(const STATICMESH_MODEL& model, const std::string& fileName);
	static ObjModelRenderer* GetModel(const STATICMESH_MODEL& model);
};