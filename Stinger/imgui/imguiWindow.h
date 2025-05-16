#pragma once
#include <vector>
#include <string>
#include <windows.h>

// �O���錾
class BehaviorNode;
class Scene;
class BossEnemy;
class Player;
class PlayerStateMachine;
struct ImFont;
// �f�o�b�O���
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
    ImguiWindow();  // Renderer�N���X��Init���Ă񂾌�ɍ쐬
    ~ImguiWindow();

    void GetRootNode(Scene* scene);         // Scene���̃I�u�W�F�N�g��S�ď�����������ɌĂ�
    void Update(const float& deltaTime);
    void Draw();
    // �ċN�Ăяo���ŁA�c���[�ɂ���S�Ẵ^�X�N���Ăяo���悤��
    void DrawBehaviorTree(const BehaviorNode* root,const BehaviorNode* parentNode = nullptr,const int& parentChildNum = 0);
    void ClearNode();
};
