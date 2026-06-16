#ifndef ARROW_HPP
#define ARROW_HPP

#include "pch.hpp"
#include "GameObject.hpp"
#include "Util/Image.hpp"
#include "Map.hpp" // 【新增】引入地圖
#include <memory>

class Arrow : public GameObject {
public:
    // 【修改】建構子多傳入 std::shared_ptr<Map> map
    Arrow(float x, float y, bool facingRight, std::shared_ptr<Map> map);

    void Update() override;
    void Draw(float cameraX, float cameraY) override;

    bool IsDead() const { return m_IsDead; }
    // 【新增】讓外部可以把箭矢賜死（消失）
    void Die() { m_IsDead = true; }
    float GetX() const { return m_X; }
    float GetY() const { return m_Y; }
    bool IsStuck() const { return m_IsStuck; } // 【新增】讓玩家知道這支箭是不是已經插牆了
    glm::vec2 GetSize() const {
        if (m_Image) {
            // 🎯【核心修改 2】：這裡乘上的倍數（0.3f）一定要跟 Draw 裡面的 arrowScale 一模一樣！
            return m_Image->GetSize() * 0.3f;
        }
        return glm::vec2(0.0f, 0.0f); }
private:
    float m_X;
    float m_Y;
    float m_Speed = 12.0f;
    bool m_FacingRight;
    bool m_IsDead = false;
    bool m_IsStuck = false;
    bool m_IsSliding = false;// 【新增】標記箭矢是否插在牆上靜止了
    // ====== 【新增：滑翔物理與消失計時變數】 =====
    float m_VelocityY = 0.0f;     // 初始往上的微小弧度（讓箭剛射出時稍微有一點點往上飄再下墜）
    float m_Gravity = 0.03f;      // 每幀下墜的重力加速度（數值越大下墜越快）
    float m_StuckTimer = 0.0f;
    float m_Rotation = 0.0f;// 插牆計時器
    float m_MaxStuckTime = 5.0f;  // 5秒後消失

    bool m_IsVisible = true;       // 這一幀箭矢是否看得到
    float m_FlashTimer = 0.0f;     // 閃爍計時器
    float m_FlashInterval = 0.16f; // 閃爍頻率（數值越小閃越快，0.08秒很適合快消失的節奏）

    std::shared_ptr<Map> m_Map; // 【新增】儲存地圖指標
    std::shared_ptr<Util::Image> m_Image;
};

#endif