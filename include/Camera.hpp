//
// Created by 吳梓暟 on 2026/5/22.
//

#ifndef CAMERA_HPP
#define CAMERA_HPP

class Camera {
public:
    void Update(float targetX, float targetY);

    float GetX() const { return m_X; }
    float GetY() const { return m_Y; }

    // 【新增】設定相機的緩衝區大小 (臨界值)
    // deadzoneX: 左右各留多少像素不移動
    // deadzoneY: 上下各留多少像素不移動
    void SetDeadzone(float deadzoneX, float deadzoneY);

    // 【新增】設定相機的極限邊界 (不看到虛空)
    // 限制相機的 X 只能在 minX ~ maxX 之間，Y 只能在 minY ~ maxY 之間
    void SetBounds(float minX, float maxX, float minY, float maxY);

private:
    float m_X = 0.0f;
    float m_Y = 0.0f;

    // 緩衝區預設值 (0 代表死死鎖定)
    float m_DeadzoneX = 0.0f;
    float m_DeadzoneY = 0.0f;

    // 邊界預設值 (設為極大/極小值代表預設不限制)
    float m_MinX = -99999.0f;
    float m_MaxX =  99999.0f;
    float m_MinY = -99999.0f;
    float m_MaxY =  99999.0f;
};

#endif