#ifndef ARROW_HPP
#define ARROW_HPP

#include "pch.hpp"
#include "GameObject.hpp"
#include "Util/Image.hpp"
#include "Map.hpp"
#include <memory>

class Arrow : public GameObject {
public:
    Arrow(float x, float y, bool facingRight, std::shared_ptr<Map> map);

    void Update() override;
    void Draw(float cameraX, float cameraY) override;

    bool IsDead() const { return m_IsDead; }
    void Die() { m_IsDead = true; }
    float GetX() const { return m_X; }
    float GetY() const { return m_Y; }
    bool IsStuck() const { return m_IsStuck; }
    bool IsSliding() const { return m_IsSliding; }
    glm::vec2 GetSize() const {
        if (m_Image) {
            return m_Image->GetSize() * 1.8f;
        }
        return glm::vec2(0.0f, 0.0f);
    }
    // 在 Arrow.hpp 的 public 區塊新增一個方法：
    void Fall() {
        if (!m_IsStuck && !m_IsSliding && !m_IsDropped) {
            m_IsDropped = true;
            m_Speed = 0.0f;       // 立刻失去水平速度（停住）
            m_VelocityY = -6.0f;  // 給一個微小的向下初速，讓它往下掉
        }
    }

private:
    float m_X;
    float m_Y;
    float m_Speed = 14.0f;
    bool m_FacingRight;
    bool m_IsDead = false;
    bool m_IsStuck = false;
    bool m_IsSliding = false;

    // ====== 物理與計時變數 ======
    float m_VelocityY = 0.0f;
    float m_Gravity = 0.03f;
    float m_StuckTimer = 0.0f;
    float m_Rotation = 0.0f;
    float m_MaxStuckTime = 5.0f;

    bool m_IsVisible = true;
    float m_FlashTimer = 0.0f;
    float m_FlashInterval = 0.16f;

    // 🎯【新增】用於儲存滑行時要吸附的目標地面 Y 座標
    float m_TargetGroundY = 0.0f;

    std::shared_ptr<Map> m_Map;
    std::shared_ptr<Util::Image> m_Image;
    bool m_IsDropped = false; // 標記是否被怪物撞落
};

#endif