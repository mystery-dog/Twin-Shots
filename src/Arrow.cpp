#include "Arrow.hpp"
#include "Util/Transform.hpp"
#include "Util/TransformUtils.hpp"
#include <cmath>

Arrow::Arrow(float x, float y, bool facingRight, std::shared_ptr<Map> map)
    : m_X(x), m_Y(y), m_FacingRight(facingRight), m_Map(map) {
    m_Image = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/character/attack/arrow.png");
}

void Arrow::Update() {
    if (m_IsDead) return;

    // ====== 插牆計時與閃爍邏輯 ======
    if (m_IsStuck) {
        m_StuckTimer += 0.016f;

        if (m_StuckTimer >= 3.0f) {
            m_FlashTimer += 0.016f;
            if (m_FlashTimer >= m_FlashInterval) {
                m_FlashTimer = 0.0f;
                m_IsVisible = !m_IsVisible;
            }
        }

        if (m_StuckTimer >= m_MaxStuckTime) {
            m_IsDead = true;
        }
        return;
    }

    // 取得實際繪製尺寸（用於碰撞）
    glm::vec2 drawSize = GetSize();
    float halfW = drawSize.x * 0.5f;
    float halfH = drawSize.y * 0.5f;
    // ====== 滑翔狀態（箭矢在地面滑行） ======
    if (m_IsSliding) {
        // 🎯【核心改進 1】: 減速更緩慢，讓箭滑更遠
        m_Speed *= 0.97f;  // 從 0.92f 改為 0.97f

        // 水平前進
        if (m_FacingRight) m_X += m_Speed;
        else m_X -= m_Speed;

        // 🎯【核心改進 2】: 更快的旋轉過渡到水平
        // 目標角度為 0（水平），用更快的速度逼近
        float targetRotation = 0.0f;
        m_Rotation += (targetRotation - m_Rotation) * 0.15f;  // 從 0.18f 改為 0.15f 更快貼平

        // 如果角度已經很小，直接歸零
        if (std::abs(m_Rotation) < 0.01f) {
            m_Rotation = 0.0f;
        }

        // 當速度過慢，視為完全停穩
        if (m_Speed < 0.2f) {  // 從 0.3f 改為 0.2f
            m_Speed = 0.0f;
            m_IsSliding = false;
            m_IsStuck = true;
            m_Rotation = 0.0f;  // 確保完全水平
            return;
        }

        // 滑行時的碰撞偵測
        if (m_Map) {
             float checkX = m_X;
            float checkY = m_Y - halfH - 2.0f; // 往下探測一點
            float arrowHalfWidth = (m_Image->GetSize().x * 0.5f) * 0.5f;
            float arrowHalfHeight = m_Image->GetSize().y * 0.5f;
            float frontX = m_FacingRight ? (m_X + arrowHalfWidth) : (m_X - arrowHalfWidth);

            // 偵測前方牆壁
            int tileID = m_Map->GetTileID(frontX, m_Y);
            if (m_Map->IsWall(tileID, true)) {
                m_IsSliding = false;
                m_IsStuck = true;
                m_X = m_FacingRight ? (m_X - 35.0f) : (m_X + 35.0f);
                m_Rotation = 0.0f;
                return;
            }

            // 🎯【核心改進 3】: 更好的懸崖偵測

            // int bottomTileID = m_Map->GetTileID(m_X, bottomY);

            // 檢查前方下方是否也有支撐（防止卡在邊緣）
            int bottomTileID = m_Map->GetTileID(m_X, 0);
            if (m_Map->IsWall(bottomTileID, true)) {
                // 有地板：將 Y 精確貼齊
                float TILE_SIZE = 64.0f;
                float tileRow = std::floor(checkY / TILE_SIZE);
                float tileTopY = (tileRow + 1.0f) * TILE_SIZE;
                m_Y = tileTopY + halfH;
            } else {
                // 下方無地板 → 退出滑行，變回飛行（準備下落）
                m_IsSliding = false;
                m_VelocityY = 0.0f;
                m_Y += 5.0f; // 稍微上提，避免卡邊
            }
        }
    }

    // ====== 拋物線飛行狀態 ======
    else {
        if (m_FacingRight) m_X += m_Speed;
        else m_X -= m_Speed;

        m_Y += m_VelocityY;
        m_VelocityY -= m_Gravity;

        // 計算空中飛行的拋物線切線角度
        float currentSpeedX = m_FacingRight ? m_Speed : -m_Speed;
        m_Rotation = std::atan2(m_VelocityY, std::abs(currentSpeedX));
        if (!m_FacingRight) {
            m_Rotation = -m_Rotation;
        }

        if (m_Map) {
            float arrowHalfWidth = (m_Image->GetSize().x * 0.5f) * 0.5f;
            float arrowHalfHeight = m_Image->GetSize().y * 0.5f;
            float frontX = m_FacingRight ? (m_X + arrowHalfWidth) : (m_X - arrowHalfWidth);

            // 1. 偵測前方撞牆
            int tileID = m_Map->GetTileID(frontX, m_Y);
            if (m_Map->IsWall(tileID, true)) {
                m_IsStuck = true;
                m_VelocityY = 0.0f;
                m_X = m_FacingRight ? (m_X - 35.0f) : (m_X + 35.0f);
                m_Rotation = 0.0f;
                return;
            }

            // 2. 偵測下方著地（啟動滑行）
            if (m_VelocityY <= 0.0f) {
                float bottomY = m_Y - arrowHalfHeight;
                int bottomTileID = m_Map->GetTileID(m_X, bottomY);

                if (m_Map->IsWall(bottomTileID, true)) {
                    // 🎯 啟動滑行模式
                    m_IsSliding = true;
                    m_VelocityY = 0.0f;

                    float TILE_SIZE = 64.0f;
                    float tileRow = std::floor(bottomY / TILE_SIZE);
                    float tileTopY = (tileRow + 1.0f) * TILE_SIZE;

                    // 精準貼齊地面
                    m_Y = tileTopY + arrowHalfHeight;

                    // 🎯【核心改進 4】: 根據速度決定初始旋轉，讓轉場更自然
                    // 如果水平速度還夠，保留一點角度讓它看起來像在"滑入"
                    if (m_Speed > 3.0f) {
                        m_Rotation = m_FacingRight ? 0.2f : -0.2f;
                    } else {
                        m_Rotation = 0.0f;
                    }
                }
            }
        }
    }

    // 邊界檢查
    if (m_X > 1500.0f || m_X < -1500.0f) {
        m_IsDead = true;
    }
}

void Arrow::Draw(float cameraX, float cameraY) {
    if (m_IsDead || !m_IsVisible) return;

    Util::Transform transform;

    // 🎯【核心改進 5】: 插牆時完全水平，滑行時使用動態角度
    if (m_IsStuck) {
        transform.rotation = 0.0f;
    } else {
        transform.rotation = m_Rotation;
    }

    transform.scale = m_FacingRight ? glm::vec2{-1.8f, 1.8f} : glm::vec2{1.8f, 1.8f};

    float screenX = m_X - cameraX;
    float screenY = m_Y - cameraY;
    transform.translation = {screenX, screenY};

    glm::vec2 size = m_Image->GetSize();
    Core::Matrices matricesData = Util::ConvertToUniformBufferData(transform, size, 0.5f);
    m_Image->Draw(matricesData);
}