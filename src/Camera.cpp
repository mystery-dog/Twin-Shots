//
// Created by 吳梓暟 on 2026/5/22.
//

#include "Camera.hpp"

// 設定緩衝區
void Camera::SetDeadzone(float deadzoneX, float deadzoneY) {
    m_DeadzoneX = deadzoneX;
    m_DeadzoneY = deadzoneY;
}

// 設定邊界
void Camera::SetBounds(float minX, float maxX, float minY, float maxY) {
    m_MinX = minX;
    m_MaxX = maxX;
    m_MinY = minY;
    m_MaxY = maxY;
}

// 核心更新邏輯
void Camera::Update(float targetX, float targetY) {

    // ==========================================
    // 1. 處理緩衝區 (Deadzone) - 臨界值推擠
    // ==========================================
    // 計算目標(玩家)與相機目前的距離差
    float diffX = targetX - m_X;
    float diffY = targetY - m_Y;

    // 如果玩家往右走，且超出右邊的臨界值
    if (diffX > m_DeadzoneX) {
        m_X += (diffX - m_DeadzoneX); // 相機往右追
    }
    // 如果玩家往左走，且超出左邊的臨界值
    else if (diffX < -m_DeadzoneX) {
        m_X += (diffX + m_DeadzoneX); // 相機往左追
    }

    // Y 軸也是一樣的邏輯 (如果你的遊戲上下視角不需要移動，可以把 Y 的 Deadzone 設大一點)
    if (diffY > m_DeadzoneY) {
        m_Y += (diffY - m_DeadzoneY);
    }
    else if (diffY < -m_DeadzoneY) {
        m_Y += (diffY + m_DeadzoneY);
    }

    // ==========================================
    // 2. 處理邊界限制 (Clamping) - 不拍到虛空
    // ==========================================
    // 強制把相機的 X 鎖死在 [m_MinX, m_MaxX] 範圍內
    if (m_X < m_MinX) m_X = m_MinX;
    if (m_X > m_MaxX) m_X = m_MaxX;

    // 強制把相機的 Y 鎖死在 [m_MinY, m_MaxY] 範圍內
    if (m_Y < m_MinY) m_Y = m_MinY;
    if (m_Y > m_MaxY) m_Y = m_MaxY;
}
