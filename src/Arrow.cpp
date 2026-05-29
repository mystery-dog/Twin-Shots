//
// Created by 吳梓暟 on 2026/5/22.
//

#include "Arrow.hpp"
#include "Util/Transform.hpp"
#include "Util/TransformUtils.hpp"

Arrow::Arrow(float x, float y) : m_X(x), m_Y(y) {
    // 為了方便測試，我們先拿小惡魔的圖片來當箭，或是你可以換成其他的圖片
    m_Image = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/character/stand/player_l0.png");
}

void Arrow::Update() {
    // 測試用：什麼都不做，讓這支箭永遠釘在 m_X, m_Y 的位置
}

void Arrow::Draw(float cameraX, float cameraY) {
    Util::Transform transform;
    transform.rotation = 0.0f;
    // 稍微縮小一點當作區別
    transform.scale = {1.0f, 1.0f};

    // 【核心驗證：世界轉螢幕】
    float screenX = m_X - cameraX;
    float screenY = m_Y - cameraY;

    transform.translation = {screenX, screenY};

    glm::vec2 size = m_Image->GetSize();
    Core::Matrices matricesData = Util::ConvertToUniformBufferData(transform, size, 0.5f);
    m_Image->Draw(matricesData);
}
