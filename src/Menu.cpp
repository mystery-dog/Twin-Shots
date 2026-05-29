//
// Created by 吳梓暟 on 2026/5/28.
//

#include "Menu.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/TransformUtils.hpp"

Menu::Menu() {
    // 載入你的選單圖片
    m_BackgroundImage = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/141.jpg");
    m_LogoImage = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/menu/menu.png");
    m_PlayImage_dark = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/menu/play0.png");
    m_PlayImage_bright = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/menu/play1.png");

    m_BGTransform.scale = {2.35f, 1.8f};
    m_BGTransform.translation = {0.0f, 0.0f};

    m_LGTransform.translation = {0.0f, 150.0f};
    m_LGTransform.scale = {1.5f,1.5f};

    m_PlayTransform.scale = {1.0f,1.0f};
    m_PlayTransform.translation = {0.0f, -50.0f};
}

void Menu::Update() {
    // 1. 取得滑鼠位置
    glm::vec2 mousePos = Util::Input::GetCursorPosition();

    // 2. 取得按鈕尺寸
    glm::vec2 btnSize = m_PlayImage_dark->GetSize();
    float halfW = btnSize.x / 2.0f;
    float halfH = btnSize.y / 2.0f;

    // 3. 判定滑鼠是否在按鈕矩形範圍內
    if (mousePos.x > m_PlayTransform.translation.x - halfW &&
        mousePos.x < m_PlayTransform.translation.x + halfW &&
        mousePos.y > m_PlayTransform.translation.y - halfH &&
        mousePos.y < m_PlayTransform.translation.y + halfH) {

        m_IsHovering = true;

        // 4. 如果懸停且按下左鍵
        if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {
            m_StartPressed = true;
        }
        } else {
            m_IsHovering = false;
        }
    auto currentPlayImg = m_IsHovering ? m_PlayImage_bright : m_PlayImage_dark;
    auto matrices = Util::ConvertToUniformBufferData(m_PlayTransform, currentPlayImg->GetSize(), 0.5f);
    currentPlayImg->Draw(matrices);
    Draw(0.0f,0.0f);
}

void Menu::Draw(float cameraX, float cameraY) {
    m_BackgroundImage->Draw(Util::ConvertToUniformBufferData(m_BGTransform, m_BackgroundImage->GetSize(), m_ZIndex_BG));
    m_LogoImage->Draw(Util::ConvertToUniformBufferData(m_LGTransform, m_LogoImage->GetSize(), m_ZIndex_LG));

}