//
// Created by 吳梓暟 on 2026/5/22.
//

#include "Emeny.hpp"
#include <cmath> // 給 std::abs 用

#include "spdlog/fmt/bundled/core.h"

Emeny::Emeny(ENEMY_ID::Type type, float startX, float startY, std::shared_ptr<Map> map)
    : Character(startX, startY, map) {
    if (m_SlimeLift.empty()) {
        for (int i = 0; i < 10; i++) {
            if (i < 4) {
                m_SlimeDead.push_back(std::make_shared<Util::Image>(
                    std::string(RESOURCE_DIR) + "/Image/enemy/e_dead" + std::to_string(i) + ".png"));
            }
            m_SlimeLift.push_back(std::make_shared<Util::Image>(
                std::string(RESOURCE_DIR) + "/Image/enemy/e_l" + std::to_string(i) + ".png"));
            m_SlimeRight.push_back(std::make_shared<Util::Image>(
                std::string(RESOURCE_DIR) + "/Image/enemy/e_r" + std::to_string(i) + ".png"));
        }
    }

    if (m_FlymonsterLift.empty()) {
        for (int i = 0; i < 8; i++) {
            if (i < 4) {
                m_FlymonsterDead.push_back(std::make_shared<Util::Image>(
                    std::string(RESOURCE_DIR) + "/Image/enemy/fly_e_dead" + std::to_string(i) + ".png"));
            }
            m_FlymonsterLift.push_back(std::make_shared<Util::Image>(
                std::string(RESOURCE_DIR) + "/Image/enemy/fly_e_l" + std::to_string(i) + ".png"));
            m_FlymonsterRight.push_back(std::make_shared<Util::Image>(
                std::string(RESOURCE_DIR) + "/Image/enemy/fly_e_r" + std::to_string(i) + ".png"));
        }
    }

    if (m_DarkDevilLift.empty()) {
        for (int i = 0; i < 8; i++) {
            if (i < 2) {
                m_DarkDevilLiftJump.push_back(std::make_shared<Util::Image>(
                std::string(RESOURCE_DIR) + "/Image/enemy/dark_e_jump_l" + std::to_string(i) + ".png"));
                m_DarkDevilRightJump.push_back(std::make_shared<Util::Image>(
                    std::string(RESOURCE_DIR) + "/Image/enemy/dark_e_jump_r" + std::to_string(i) + ".png"));
            }
            if (i < 4) {
                m_DarkDevilDead.push_back(std::make_shared<Util::Image>(
                    std::string(RESOURCE_DIR) + "/Image/enemy/dark_e_dead" + ".png"));
            }
            m_DarkDevilLift.push_back(std::make_shared<Util::Image>(
                std::string(RESOURCE_DIR) + "/Image/enemy/dark_e_l" + std::to_string(i) + ".png"));
            m_DarkDevilRight.push_back(std::make_shared<Util::Image>(
                std::string(RESOURCE_DIR) + "/Image/enemy/dark_e_r" + std::to_string(i) + ".png"));
        }
    }

    // 怪物通常走得比較慢
    if (type == ENEMY_ID::Slime) {
        m_Speed = 3.0f;
        m_Type = ENEMY_ID::Slime;
    }else if (type == ENEMY_ID::Flymonster) {
        m_Speed = 1.0f;
        m_Type = ENEMY_ID::Flymonster;
    }else if (type == ENEMY_ID::DarkDevil) {
        m_Speed = 1.0f;
        m_Type = ENEMY_ID::DarkDevil;
    }
}

void Emeny::Update() {
    // ==========================================
    // 1. 大腦決策：決定這回合要往哪走
    // ==========================================
    m_VelocityX = m_MovingLeft ? -m_Speed : m_Speed;
    m_FacingRight = !m_MovingLeft; // 面朝方向跟著改

    // ==========================================
    // 2. 身體執行：老爸，幫我算物理！
    // ==========================================
    Character::Update();

    // ==========================================
    // 3. AI 反應：判斷是否撞牆
    // ==========================================
    // 如果算完物理後，現在的 X 座標跟預期的不一樣 (座標幾乎沒變)
    // 代表被 IsWall() 擋下來了，立刻轉頭！
    if (m_IsHittingWall) {
        m_MovingLeft = !m_MovingLeft; // 撞牆了，立刻轉頭！
    }

    // ==========================================
    // 4. 【新增】動畫狀態機 (Animation State Machine)
    // ==========================================
    // 根據老爸算出來的物理狀態，決定現在要播什麼動畫！

    // 狀態 A：踩在地上 (需要播放走路動畫)

    // 計時器推進
    m_FrameTimer++;
    if (m_FrameTimer >= m_AnimationSpeed) {
        m_FrameTimer = 0; // 計時器歸零
        m_FrameIndex++;   // 換下一張圖
    }

    // 根據面向的方向，決定要拿哪一組陣列來播
    if (m_MovingLeft) {
        // 利用 % (取餘數) 讓索引在 0, 1, 0, 1 之間無限循環！
        if (GetType() == ENEMY_ID::Slime) {
            int currentIndex = m_FrameIndex % m_SlimeLift.size();
            m_Image = m_SlimeLift[currentIndex];
        }else if (GetType() == ENEMY_ID::Flymonster) {
            int currentIndex = m_FrameIndex % m_FlymonsterLift.size();
            m_Image = m_FlymonsterLift[currentIndex];
        }else {
            int currentIndex = m_FrameIndex % m_DarkDevilLift.size();
            m_Image = m_DarkDevilLift[currentIndex];
        }

    }
    else if (!m_MovingLeft) {
        if (GetType() == ENEMY_ID::Slime) {
            int currentIndex = m_FrameIndex % m_SlimeRight.size();
            m_Image = m_SlimeRight[currentIndex];
        }else if (GetType() == ENEMY_ID::Flymonster) {
            int currentIndex = m_FrameIndex % m_FlymonsterRight.size();
            m_Image = m_FlymonsterRight[currentIndex];
        }else {
            int currentIndex = m_FrameIndex % m_DarkDevilLift.size();
            m_Image = m_DarkDevilRight[currentIndex];
        }
    }

    if ((m_FrameIndex % 10 > 8 || m_FrameIndex % 10 < 3) && GetType() == ENEMY_ID::Slime) {
        m_Speed = 0.0f;
    } else {
        m_Speed = 3.0f;
    }
}
