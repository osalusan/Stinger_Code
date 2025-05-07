#pragma once
#include <vector>
#include <string>
#include <Windows.h>

// �O���錾
class BehaviorNode;
class Scene;
class BossEnemy;
class Player;
class PlayerStateMachine;
struct ImFont;

class ImguiWindow final
{
private:
    const BehaviorNode* m_RootNodeCache = nullptr;
    const BossEnemy* m_BossEnemyCache = nullptr;
    const Player* m_PlayerCache = nullptr;
    const PlayerStateMachine* m_PlayerStateMachineCache = nullptr;
    std::vector<std::string> m_TaskName = {};

    ImFont* m_Font;

    std::wstring ToWString(const std::string& str, UINT codePage);
    std::string ToUtf8(const std::wstring& wstr);
    std::string GetCurrentPlayerStateName();
    std::string GetCurrentEnemyAttackParts();

    int m_ShowStyleDerivationInfo = 1;
    int m_ShowDerivationInfo = 1;
public:
    ImguiWindow();
    ~ImguiWindow();

    void GetRootNode(Scene* scene);
    void Update(const float& deltaTime);
    void Draw();
    void DrawBehaviorTree(const BehaviorNode* root,const BehaviorNode* parentNode = nullptr,const int& parentChildNum = 0);
    void ClearNode();
};
