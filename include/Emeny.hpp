//
// Created by 吳梓暟 on 2026/5/22.
//

#ifndef EMENY_HPP
#define EMENY_HPP

#include "Character.hpp" // 引入萬物之父
#include "Emeny_ID.hpp"

class Emeny : public Character {
public:
    Emeny(ENEMY_ID::Type, float startX, float startY, std::shared_ptr<Map> map);

    ENEMY_ID::Type GetType() const { return m_Type; }

    // 實作怪物專屬的 AI 大腦
    void Update() override;

    void DarkDevilMove();

    // ==========================================
    // 【新增】判斷生死狀態的公開函式
    // ==========================================
    bool IsDead() const { return m_IsDead; }

private:
    // 【新增】用來儲存這隻怪物是什麼種類
    ENEMY_ID::Type m_Type;

    // 怪物的行為狀態：預設往左走
    bool m_MovingLeft = true;
    // 【新增】跳躍冷卻計時器
    int m_JumpCooldown = 0;
    int m_MaxJumpTimer = 0;
    bool m_IsJump = false;
    // 【新增】怪物是否死亡的旗標，預設為活著 (false)
    bool m_IsDead = false;

    // ==========================================
    // 【新增】動畫資源與計時器
    // ==========================================
    // 裝載不同狀態的圖片
    std::vector<std::shared_ptr<Util::Image>> m_SlimeLift;
    std::vector<std::shared_ptr<Util::Image>> m_SlimeRight;
    std::vector<std::shared_ptr<Util::Image>> m_SlimeDead;

    std::vector<std::shared_ptr<Util::Image>> m_FlymonsterLift;
    std::vector<std::shared_ptr<Util::Image>> m_FlymonsterRight;
    std::vector<std::shared_ptr<Util::Image>> m_FlymonsterDead;

    std::vector<std::shared_ptr<Util::Image>> m_DarkDevilLift;
    std::vector<std::shared_ptr<Util::Image>> m_DarkDevilRight;
    std::vector<std::shared_ptr<Util::Image>> m_DarkDevilLiftJump;
    std::vector<std::shared_ptr<Util::Image>> m_DarkDevilRightJump;
    std::vector<std::shared_ptr<Util::Image>> m_DarkDevilDead;

    // 動畫計時器
    int m_FrameIndex = 0;       // 目前播到第幾張圖
    int m_FrameTimer = 0;       // 計時器 (用來算經過了幾幀)
    int m_AnimationSpeed = 7.0f;  // 播放速度 (數字越大，動畫播得越慢。10 代表每 10 幀換一張圖)
};

#endif