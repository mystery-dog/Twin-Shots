//
// Created by 吳梓暟 on 2026/5/26.
//

#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "pch.hpp"
#include "GameObject.hpp"
#include "Util/Image.hpp"
#include "Map.hpp"
#include <memory>

// Character 繼承 GameObject，成為所有「實體角色」的基底類別
class Character : public GameObject {
public:
    Character(float startX, float startY, std::shared_ptr<Map> map);

    // Update 依然保持為「純虛擬函式 (= 0)」，
    // 因為玩家是用鍵盤控制，敵人是用 AI 控制，所以必須留給小孩們自己實作！
    virtual void Update() override;

    // Draw 我們可以直接在這邊實作掉！
    // 因為不管玩家還是敵人，畫出來的邏輯（扣掉相機座標、畫除錯探針）都是一模一樣的！
    void Draw(float cameraX, float cameraY) override;

    float GetX() const { return m_X; }
    float GetY() const { return m_Y; }

    bool IsFacingRight() const { return m_FacingRight; }

    void SetMapmax(float MapHeight, float MapWidth);

protected:
    // =======================================================
    // 【關鍵修改】：把 private 變成 protected！
    // 這樣繼承它的 Player 和 Enemy 才能直接修改這些物理變數
    // =======================================================
    float m_X;
    float m_Y;
    float m_Speed = 7.0f; // 預設速度，小孩可以自己改

    float m_VelocityX = 0.0f;
    float m_VelocityY = 0.0f;
    float m_Gravity = -0.5f;
    float m_JumpForce = 15.0f;
    bool m_IsGrounded = false;

    float m_MapWidth = 700.0f;
    float m_MapHeight = 400.0f;//地圖一
    //地圖二 m_MapHeight = 450.0f
    bool m_FacingRight = true;
    // 【新增】記錄這回合有沒有撞到水平牆壁！
    bool m_IsHittingWall = false;
    // 【新增】身分證：預設大家都是非玩家 (怪物)
    bool m_IsPlayer = false;
    //是否踩得是跳躍方塊
    bool m_IsstandJumpLift = false;
    bool m_IsstandJumpRight = false;
    bool m_JumpMax = false;

    std::shared_ptr<Map> m_Map;
    std::shared_ptr<Util::Image> m_Image;

};

#endif