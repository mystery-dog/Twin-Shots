//
// Created by 吳梓暟 on 2026/5/26.
//

#include "Character.hpp"
#include "Util/Transform.hpp"
#include "Util/TransformUtils.hpp"

// 【建構子實作】
// 把初始化的 X, Y, 以及地圖指標存進 protected 變數中
Character::Character(float startX, float startY, std::shared_ptr<Map> map)
    : m_X(startX), m_Y(startY), m_Map(map) {
}

void Character::SetMapmax(float MapHeight, float MapWidth) {
    m_MapHeight = MapHeight;
    m_MapWidth = MapWidth;
}


void Character::Update() {
    if (!m_Image || !m_Map) return; // 保護機制

    float trueHalfWidth = (m_Image->GetSize().x * 0.5f);

    // 1. 根據大腦 (Player/Enemy) 傳來的 m_VelocityX，預測下一步
    float nextX = m_X + m_VelocityX;
    float bodyY = m_Y;

    // 2. 水平碰撞判定
    m_IsHittingWall = false; // 每次更新前，先把牌子放下

    if (m_VelocityX < 0.0f) { // 正在往左走
        float leftSideX = nextX - trueHalfWidth;
        int leftTile = m_Map->GetTileID(leftSideX, bodyY);

        if (m_Map->IsWall(leftTile, m_IsPlayer)) {
            int gridX = std::floor((leftSideX + 610.0f + m_Map->GetTileSize() / 2.0f) / m_Map->GetTileSize());
            float tileCenterX = (gridX * m_Map->GetTileSize()) - 610.0f;
            float tileRightEdge = tileCenterX + m_Map->GetTileSize() / 2.0f;
            nextX = tileRightEdge + trueHalfWidth * 1.1f;
            // 【新增】撞到左邊牆壁了！
            m_IsHittingWall = true;
        }
    }
    else if (m_VelocityX > 0.0f) { // 正在往右走
        float rightSideX = nextX + trueHalfWidth;
        int rightTile = m_Map->GetTileID(rightSideX, bodyY);

        if (m_Map->IsWall(rightTile, m_IsPlayer)) {
            int gridX = std::floor((rightSideX + 610.0f + m_Map->GetTileSize() / 2.0f) / m_Map->GetTileSize());
            float tileCenterX = (gridX * m_Map->GetTileSize()) - 610.0f;
            float tileLeftEdge = tileCenterX - m_Map->GetTileSize() / 2.0f;
            nextX = tileLeftEdge - trueHalfWidth * 1.1f;
            // 【新增】撞到右邊牆壁了！
            m_IsHittingWall = true;
        }
    }

    m_X = nextX; // 套用 X

    // 3. 垂直碰撞與重力判定
    m_VelocityY += m_Gravity;
    if (m_VelocityY < -11.5f) { m_VelocityY = -11.5f; }

    float nextY = m_Y + m_VelocityY;
    float halfHeight = m_Image->GetSize().y;
    float halfWidth  = m_Image->GetSize().x;

    m_IsGrounded = false;

    if (m_VelocityY <= 0.0f) {
        float leftFootX = m_X - halfWidth * 0.5f;
        float rightFootX = m_X + halfWidth * 0.5f;
        float feetY = nextY - halfHeight;

        int leftTile = m_Map->GetTileID(leftFootX, feetY);
        int rightTile = m_Map->GetTileID(rightFootX, feetY);

        if (m_Map->IsSolid(leftTile, m_IsPlayer) || m_Map->IsSolid(rightTile, m_IsPlayer)) {
            int gridY = std::floor((-(feetY) + m_Map->GetTileSize() / 2.0f) / m_Map->GetTileSize());
            float tileTopY = -(gridY * m_Map->GetTileSize()) + m_Map->GetTileSize() / 2.0f;
            nextY = tileTopY;
            m_VelocityY = 0.0f;
            m_IsGrounded = true;
        }
    }

    m_Y = nextY; // 套用 Y

}


// 【共用畫圖邏輯】
// 不管是 Player 還是 Enemy，畫圖與畫探針的數學邏輯都是一模一樣的，
void Character::Draw(float cameraX, float cameraY) {
    // 保護機制：如果有圖片才畫
    if (!m_Image) return;

    Util::Transform transform;
    transform.rotation = 0.0f;
    // 預設放大兩倍 (未來如果有怪物大小不同，可以把這個 scale 抽成變數)
    transform.scale = {2.0f, 2.0f};

    // 1. 世界座標轉螢幕座標
    float screenX = m_X - cameraX;
    float screenY = m_Y - cameraY;

    transform.translation = {screenX, screenY};

    glm::vec2 size = m_Image->GetSize();
    Core::Matrices matricesData = Util::ConvertToUniformBufferData(transform, size, 1.0f);
    m_Image->Draw(matricesData);

    // ==========================================
    // 2. 共用的除錯渲染：畫出所有物理探針！
    // ==========================================
    if (m_ShowDebug) {
        static auto debugPoint = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/life_0.png");

        float halfWidth = m_Image->GetSize().x;
        float halfHeight = m_Image->GetSize().y;

        // ------------------------------------------
        // (1) 垂直碰撞探針 (腳底)
        // ------------------------------------------
        float leftFootX = m_X - halfWidth * 0.5f;
        float rightFootX = m_X + halfWidth * 0.5f;
        float feetY = m_Y - halfHeight;

        Util::Transform probeTransform;
        probeTransform.rotation = 0.0f;
        probeTransform.scale = {0.3f, 0.3f};

        probeTransform.translation = {leftFootX - cameraX, feetY - cameraY};
        debugPoint->Draw(Util::ConvertToUniformBufferData(probeTransform, debugPoint->GetSize(), 2.0f));

        probeTransform.translation = {rightFootX - cameraX, feetY - cameraY};
        debugPoint->Draw(Util::ConvertToUniformBufferData(probeTransform, debugPoint->GetSize(), 2.0f));

        // ------------------------------------------
        // (2) 水平碰撞探針 (腰部)
        // ------------------------------------------
        float trueHalfWidth = halfWidth * 0.5f;
        float bodyY = m_Y + 10.0f;

        float leftSideX = m_X - trueHalfWidth;
        float rightSideX = m_X + trueHalfWidth;

        probeTransform.translation = {leftSideX - cameraX, bodyY - cameraY};
        debugPoint->Draw(Util::ConvertToUniformBufferData(probeTransform, debugPoint->GetSize(), 2.0f));

        probeTransform.translation = {rightSideX - cameraX, bodyY - cameraY};
        debugPoint->Draw(Util::ConvertToUniformBufferData(probeTransform, debugPoint->GetSize(), 2.0f));
    }
}
