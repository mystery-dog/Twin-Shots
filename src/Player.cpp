//
// Created by 吳梓暟 on 2026/5/22.
//

#include "Player.hpp"
#include "Util/Input.hpp"         // 負責偵測鍵盤按鍵
#include "Util/Keycode.hpp"       // 定義了 UP, DOWN, LEFT, RIGHT 等按鍵代碼

// 【建構子】
// 當角色被創造出來時，設定它的初始座標，並載入圖片
Player::Player(float startX, float startY, std::shared_ptr<Map> map)
    : Character(startX, startY, map) {
    // 【新增】設定自己的身分是玩家！
    m_IsPlayer = true;

    // 我們只負責幫自己載入這張專屬的「小惡魔圖片」
    m_Image = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/character/stand/player_l0.png");
}

void Player::SetBorder(float Height, float Width) {
    m_BorderHeight = Height;
    m_BorderWidth = Width;
}


// 【邏輯更新】每幀都會被執行一次
void Player::Update() {

    m_VelocityX = 0.0f; // 預設不動

    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        m_VelocityX = -m_Speed;
        m_FacingRight = false;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        m_VelocityX = m_Speed;
        m_FacingRight = true;
    }

    if (Util::Input::IsKeyDown(Util::Keycode::UP)) {
        m_VelocityY = m_JumpForce;
        m_IsGrounded = false;
    }
    // 2. 處理上下左右無縫聯通 (Screen Wrapping)
    // 概念：一旦座標超出了地圖邊界，就強制把座標修改到對面的邊界。

    // 超出左邊界 -> 從右邊出來
    if (m_X < m_BorderWidth) {
        m_X = m_MapWidth;
    }
    // 超出右邊界 -> 從左邊出來
    else if (m_X > m_MapWidth) {
        m_X = m_BorderWidth;
    }

    // 超出上邊界 -> 從下面出來 (我們設定 Y 往下是負數，所以最下面是 -m_MapHeight)
    if (m_Y > m_BorderHeight) {//地圖一
        //地圖二 m_Y > 550.0f
        m_Y = -m_MapHeight;
    }
    // 超出下邊界 -> 從上面出來
    else if (m_Y < -m_MapHeight) {
        m_Y = m_BorderHeight;//地圖一
        //地圖二 m_Y = 550.0f
    }

    Character::Update();

    //扣血測試
    if (Util::Input::IsKeyDown(Util::Keycode::O)) {
        m_life--;
    }
}

void Player::Draw(float cameraX, float cameraY) {
    // 1. 呼叫老爸 (Character) 的 Draw，讓他幫我們畫出「本體」和「除錯紅點探針」！
    Character::Draw(cameraX, cameraY);

    // 2. 玩家專屬的「無縫穿牆視覺分身」邏輯
    if (!m_Image) return;

    Util::Transform transform;
    transform.rotation = 0.0f;
    transform.scale = {2.0f, 2.0f};
    glm::vec2 size = m_Image->GetSize();

    float screenX = m_X - cameraX;
    float screenY = m_Y - cameraY;

    // 計算地圖的總寬度 (右邊界 - 左邊界)
    // 這裡我們用你之前設定的 m_BorderWidth 和 m_MapWidth
    float leftEdge = m_BorderWidth;
    float rightEdge = m_MapWidth;
    float wrapDistance = rightEdge - leftEdge;

    // 如果角色的座標靠近左邊界
    if (m_X < leftEdge + size.x) {
        transform.translation = {screenX + wrapDistance, screenY};
        m_Image->Draw(Util::ConvertToUniformBufferData(transform, size, 1.0f));
    }
    // 如果角色的座標靠近右邊界
    else if (m_X > rightEdge - size.x) {
        transform.translation = {screenX - wrapDistance, screenY};
        m_Image->Draw(Util::ConvertToUniformBufferData(transform, size, 1.0f));
    }
}
