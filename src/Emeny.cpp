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
    m_Speed = 3.0f;
    m_Type = type;
}

void Emeny::Update() {
    //<Ying>
    if (m_life <= 0) {
        if (!m_IsDeadTriggered) {
            m_FrameIndex = 0;
            m_FrameTimer = 0;
            m_IsDeadTriggered = true;
        }
        m_Speed = 0.0f;
        m_VelocityX = 0.0f;
        m_VelocityY = 0.0f;

        // 播放死亡動畫
        m_FrameTimer++;
        if (m_FrameTimer >= m_AnimationSpeed) {
            m_FrameTimer = 0;
            m_FrameIndex++;
        }

        if (m_Type == ENEMY_ID::Slime) {
            if (m_FrameIndex < m_SlimeDead.size()) {
                m_Image = m_SlimeDead[m_FrameIndex];
            } else {
                // 動畫播完了！將 Character 的 m_IsActive 設為 false，讓 App 把它移出遊戲
                m_IsActive = false;
            }
        }
        else if (m_Type == ENEMY_ID::Flymonster) {
            if (m_FrameIndex < m_FlymonsterDead.size()) {
                m_Image = m_FlymonsterDead[m_FrameIndex];
            } else {
                m_IsActive = false;
            }
        }
        else if (m_Type == ENEMY_ID::DarkDevil) {
            if (m_FrameIndex < m_DarkDevilDead.size()) {
                m_Image = m_DarkDevilDead[m_FrameIndex];
            } else {
                m_IsActive = false;
            }
        }

        // 既然死掉了，後面的移動、碰撞、走路動畫就通通不用算了，直接 return 跳出！
        return;
    }
    //</Ying>



    // ==========================================
    // 1. 大腦決策：決定這回合要往哪走
    // ==========================================
    m_VelocityX = m_MovingLeft ? -m_Speed : m_Speed;
    m_FacingRight = !m_MovingLeft; // 面朝方向跟著改

    // ==========================================
    // 2. 身體執行：老爸，幫我算物理！
    // ==========================================
    Character::Update();

    if (m_Type == ENEMY_ID::DarkDevil) {
        DarkDevilMove();
    }

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
            if (!m_IsGrounded) {
                int currentIndex = m_FrameIndex % m_DarkDevilLiftJump.size();
                m_Image = m_DarkDevilLiftJump[currentIndex];
            }else {
                int currentIndex = m_FrameIndex % m_DarkDevilLift.size();
                m_Image = m_DarkDevilLift[currentIndex];
            }
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
            if (!m_IsGrounded) {
                int currentIndex = m_FrameIndex % m_DarkDevilRightJump.size();
                m_Image = m_DarkDevilRightJump[currentIndex];
            }else {
                int currentIndex = m_FrameIndex % m_DarkDevilRight.size();
                m_Image = m_DarkDevilRight[currentIndex];
            }
        }
    }

    if (GetType() == ENEMY_ID::Slime) {
        if (m_FrameIndex % 10 > 8 || m_FrameIndex % 10 < 3) {
            m_Speed = 0.0f;
        } else {
            m_Speed = 3.0f;
        }
    }
}

void Emeny::DarkDevilMove() {
    // 1. 冷卻計時器推進
    m_JumpCooldown++;
    if (m_IsGrounded) {
        m_IsPlayer = false;
        m_Speed = 3.0f;
    }
    if (m_IsGrounded  && m_JumpCooldown > m_MaxJumpTimer) {
        if (IsFacingRight() && m_IsstandJumpRight) {
            m_IsJump = true;
        }else if (!IsFacingRight() && m_IsstandJumpLift) {
            m_IsJump = true;
        }else {
            m_IsJump = false;
        }
    }
    int FaceMode = rand() % 100;
    if (FaceMode == 2 && m_IsGrounded) m_MovingLeft = !m_MovingLeft;
    // 2. 判斷是否踩在地上 (m_IsGrounded) 且冷卻時間完畢 (例如 120 幀 = 2秒)
    if (m_IsJump) {
        // 賦予向上的垂直速度！(老爸的 m_JumpForce 預設是 15.0f)
        m_VelocityY = 13.5f;

        // 【核心魔法：決定兩種跳躍模式】
        // 產生 0 或 1，機率各一半 (50%)
        int jumpMode = rand() % 4;

        if (jumpMode == 0 && !m_JumpMax) {
            m_Speed = 2.0f;
        }else if (jumpMode == 1) {
            m_MaxJumpTimer = 300.0f;
            m_Speed = 5.2f;
        }else {
            m_Speed = 5.2f;
            m_MaxJumpTimer = 120.0f;
        }

        // 標記為離地，這樣物理引擎就會開始對牠套用重力往下掉
        m_IsGrounded = false;
        m_IsJump = false;
        m_IsPlayer = true;

        // 重置冷卻時間
        m_JumpCooldown = 0;
    }
}


int Emeny::GetScore() const {
    switch (m_Type) {
        case ENEMY_ID::Slime:      return 100;
        case ENEMY_ID::Flymonster: return 200;
        case ENEMY_ID::DarkDevil:  return 300;
        default:                   return 0;
    }
}