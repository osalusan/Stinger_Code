#pragma once
#include <string>
#include <vector>

enum class NODE_STATE 
{
    SUCCESS = 0,
    FAILURE,
    RUNNING
};

// 派生可能体力割合、発生確率、移行時間割合
struct DERIVATION_DATA
{
    float HealthValue = 0.0f;            // 派生可能体力割合 / 1.0が最大
    int Chance = 0;                      // 派生技発生確率 / 100が最大
    float TransTimeValue = 0.0f;	     // 派生移行時間割合 / 1.0が最大
};

// タスクノード、シーケンス、セレクターで使用する物を入れた汎用クラス
class BehaviorNode 
{
private:
    std::vector<DERIVATION_DATA> m_DerivationData = {};             // 派生技のデータを格納 / Getからしか取得できないように
protected:
    std::string m_TaskName = {};                                    // CSVの方と名前を同じに
    std::vector<BehaviorNode*> m_Children = {};
    NODE_STATE m_CurrentState = NODE_STATE::FAILURE;

    std::vector<int> m_ChildDerivChance = {};		                // 派生技の確率 / 合計で100になるように

    // IMGUIで使用するために、基底クラスに変数を移動
    std::string m_AnimName = "";
    float m_UseStaminaValue = 0.0f;				                	// 使用スタミナ倍率
public:
    virtual ~BehaviorNode();
    virtual void Init() = 0;
    virtual NODE_STATE Update(const float& deltaTime) = 0;

    // 現在使用中か、子まで調べる
    bool CheckRunningNode(BehaviorNode* currentNode);

    // ---------------------------------------------
    // タスクノードのみを追加
    // 派生の情報登録の為、複数に分けている
    // ---------------------------------------------
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

    // ---------------------------------------------
    // ノード自体を追加
    // シーケンスやセレクターの時はこっち
    // ---------------------------------------------
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
    // ルートノード用
    void SetTaskName(const std::string& name)
    {
        m_TaskName = name;
    }
    // Nodeでのみ使用
    void SetCurrentState(const NODE_STATE& state)
    {
        m_CurrentState = state;
    }
};