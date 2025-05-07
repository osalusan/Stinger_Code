#pragma once
#include <string>
#include <vector>

enum class NODE_STATE 
{
    SUCCESS = 0,
    FAILURE,
    RUNNING
};

// �h���\�̗͊����A�����m���A�ڍs���Ԋ���
struct DERIVATION_DATA
{
    float HealthValue = 0.0f;            // �h���\�̗͊��� / 1.0���ő�
    int Chance = 0;                      // �h���Z�����m�� / 100���ő�
    float TransTimeValue = 0.0f;	     // �h���ڍs���Ԋ��� / 1.0���ő�
};

class BehaviorNode 
{
private:
    std::vector<DERIVATION_DATA> m_DerivationData = {};             // �h���Z�̃f�[�^���i�[ / Get���炵���擾�ł��Ȃ��悤��
protected:
    std::string m_TaskName = {};                                    // CSV�̕��Ɩ��O�𓯂���
    std::vector<BehaviorNode*> m_Children = {};
    NODE_STATE m_CurrentState = NODE_STATE::FAILURE;

    std::vector<int> m_ChildDerivChance = {};		                // �h���Z�̊m�� / ���v��100�ɂȂ�悤��

    // IMGUI�Ŏg�p���邽�߂ɁA���N���X�ɕϐ����ړ�
    std::string m_AnimName = "";
    float m_UseStaminaValue = 0.0f;				                	// �g�p�X�^�~�i�{��
public:
    virtual ~BehaviorNode();
    virtual void Init() = 0;
    virtual NODE_STATE Update(const float& deltaTime) = 0;

    // ���Ȃ�������true���Ԃ�
    bool CheckRunningNode(BehaviorNode* currentNode);

    template <typename T, typename... Arg>
    BehaviorNode* AddTaskChild(Arg&&...args)
    {
        BehaviorNode* node = new T(std::forward<Arg>(args)...);
        if (node == nullptr) return nullptr;
        node->Init();
        m_Children.emplace_back(node);
        return node;
    }

    template <typename T, typename... Args>
    BehaviorNode* AddTaskChild(int derivChance, Args&&... args)
    {
        m_ChildDerivChance.emplace_back(derivChance);
        m_DerivationData.emplace_back(DERIVATION_DATA());
        
        return AddTaskChild<T>(std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    BehaviorNode* AddTaskChild(DERIVATION_DATA derivData, Args&&... args)
    {
        m_DerivationData.emplace_back(derivData);

        return AddTaskChild<T>(std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    BehaviorNode* AddTaskChild(DERIVATION_DATA derivData, int derivChance, Args&&... args)
    {
        m_ChildDerivChance.emplace_back(derivChance);
        m_DerivationData.emplace_back(derivData);

        return AddTaskChild<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    BehaviorNode* AddNodeChild(const std::string& name)
    {
        BehaviorNode* node = new T;
        if (node != nullptr)
        {
            node->SetTaskName(name);
            node->Init();
            m_Children.emplace_back(node);
        }
        return node;
    }

    const std::vector<BehaviorNode*> GetChildren()const
    {
        return m_Children;
    }
    const std::string GetTaskName()const
    {
        return m_TaskName;
    }
    const NODE_STATE& GetNodeState()const
    {
        return m_CurrentState;
    }
    const std::string& GetAnimName()const
    {
        return m_AnimName;
    }
    const float& GetUseStaminaValue()const
    {
        return m_UseStaminaValue;
    }

    DERIVATION_DATA GetDerivationData(const int& num)const;

    const std::vector<DERIVATION_DATA>& GetDerivationData()const
    {
        return m_DerivationData;
    }


    int GetTotalDerivChance()const;
    int GetDerivChance(const int& num)const;
    // ���[�g�m�[�h�p
    void SetTaskName(const std::string& name)
    {
        m_TaskName = name;
    }
    // Node�ł̂ݎg�p
    void SetCurrentState(const NODE_STATE& state)
    {
        m_CurrentState = state;
    }
};