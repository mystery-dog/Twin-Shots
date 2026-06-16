//
// Created by 吳梓暟 on 2026/5/22.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "pch.hpp"
#include "GameObject.hpp"    // 繼承遊戲物件介面
#include "Util/Image.hpp"    // 用來載入角色的圖片
#include <memory>
#include "Map.hpp" // 【新增】引入地圖
#include "Character.hpp"
#include "Emeny.hpp"
#include "Arrow.hpp"
#include "Util/Transform.hpp"     // 負責大小與位置
#include "Util/TransformUtils.hpp"// 助教提供的矩陣轉換工具

class Player : public Character {
public:
    // 建構子：在產生角色時，設定它的初始位置
    Player(float startX, float startY, std::shared_ptr<Map> map);

    // 實作 GameObject 介面規定的兩個函式
    void Update() override;

    int GetLife() { return  m_life; }
    // <Ying>====== 【新增：讓外部獲取玩家圖片尺寸的函式】 ======
    glm::vec2 GetSize() {
        if (m_Image) {
            return m_Image->GetSize();
        }
        return glm::vec2(0.0f, 0.0f); // 防呆，如果沒圖片就回傳 0
    }
    // </Ying>==================================================
    void SetLife() { m_life = 3; }

    void SetBorder(float Height, float Width);

    // 【新增】受傷函式，讓外部（例如 Map 或 App）在偵測到碰撞時呼叫
    void Hurt();

    // 【新增】讓外部可以檢查玩家目前是不是無敵狀態
    bool IsInvincible() const { return m_IsInvincible; }
    // 【新增】宣告玩家要覆寫自己的 Draw 邏輯！
    void Draw(float cameraX, float cameraY) override;
    // 【新增】讓外部（App）可以讀取玩家目前射出的所有箭矢
    std::vector<std::shared_ptr<Arrow>>& GetArrows() { return m_PlayerArrows; }
protected:
    //Arrow
    std::vector<std::shared_ptr<Arrow>> m_PlayerArrows;
    bool m_IsOnArrow = false; // 紀錄有沒有踩在箭上
private:
    //<Ying>==============================================
    // ====== 【新增：動畫相關成員變數】 ======
    // 儲存走動動畫的圖片陣列（假設左右各有 2 張巡迴播放）
    std::vector<std::shared_ptr<Util::Image>> m_WalkLeftImages;
    std::vector<std::shared_ptr<Util::Image>> m_WalkRightImages;
    std::vector<std::shared_ptr<Util::Image>> m_StandLeftImage;
    std::vector<std::shared_ptr<Util::Image>> m_StandRightImage;
    // ====== 【新增：跳躍與下落動畫陣列】 ======
    std::vector<std::shared_ptr<Util::Image>> m_JumpLeftImages;
    std::vector<std::shared_ptr<Util::Image>> m_JumpRightImages;

    // 如果你有分「下落」動畫，可以加這兩行；如果沒有，就跟跳躍共用
    std::vector<std::shared_ptr<Util::Image>> m_FallLeftImages;
    std::vector<std::shared_ptr<Util::Image>> m_FallRightImages;
    float m_AnimTimer = 0.0f;       // 動畫累計時間
    float m_AnimFrameDuration = 0.15f; // 每張圖片播 0.15 秒
    int m_CurrentFrame = 0;         // 目前播到第幾張
    bool m_IsMoving = false;
    // 新增跳躍動畫的速度控制
    float m_WalkFrameDuration = 0.07f;  // 【新增】走路動畫速度（數值小 = 腳步快）
    float m_StandFrameDuration = 0.08f; // 【新增】站立待機速度（數值大 = 呼吸慢）
    float m_JumpFrameDuration = 0.20f;
    // 為了精準捕捉「狀態切換」重置動畫，我們記錄一下原本在不在地面
    bool wasGroundedOrOnArrow = (m_IsGrounded || m_IsOnArrow);

    // ====== 【新增：受傷與無敵時間變數】 ======
    std::shared_ptr<Util::Image> m_HurtLeftImage;
    std::shared_ptr<Util::Image> m_HurtRightImage;
    // ====== 🎯【新增：射擊動畫相關變數】 ======
    std::vector<std::shared_ptr<Util::Image>> m_ShootLeftImages;
    std::vector<std::shared_ptr<Util::Image>> m_ShootRightImages;
    bool m_IsShooting = false;         // 目前是否正在射擊狀態
    bool m_HasSpawnedArrow = false;    // 確保這一次射擊動畫中，只會生出一支箭
    float m_ShootFrameDuration = 0.06f;
    // 射擊動畫每格的切換速度（可依手感調整
    bool m_IsInvincible = false;       // 是否處於無敵狀態
    float m_InvincibleTimer = 0.0f;    // 無敵時間計時器
    float m_InvincibleDuration = 1.5f; // 無敵時間長度（1.5 秒）

    float m_FlashTimer = 0.0f;         // 閃爍計時器
    float m_FlashInterval = 0.1f;      // 每 0.1 秒閃爍一次
    bool m_IsVisible = true;           // 這一幀角色是否可見（用來做閃爍效果）

    // ====== 🎯【新增：動態路徑與物理變數】 ======
    float m_Acceleration = 0.7f;   // 每幀增加的速度（數值越大越快衝到最高速）
    float m_Friction = 0.82f;
    //</Ying>=====================================



    int m_life = 3;

    float m_BorderWidth = -640.0f;
    float m_BorderHeight = 400.0f;
};

#endif