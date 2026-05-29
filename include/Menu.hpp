//
// Created by 吳梓暟 on 2026/5/28.
//

#ifndef MENU_HPP
#define MENU_HPP

#include "GameObject.hpp"
#include "Util/Image.hpp"
#include <memory>

class Menu : public GameObject{
public:
    Menu();
    void Update() override;
    void Draw(float cameraX, float cameraY) override;
    bool IsStartPressed() const { return m_StartPressed; }

private:
    std::shared_ptr<Util::Image> m_LogoImage;
    std::shared_ptr<Util::Image> m_PlayImage_dark;
    std::shared_ptr<Util::Image> m_PlayImage_bright;
    std::shared_ptr<Util::Image> m_BackgroundImage;

    bool m_StartPressed = false;
    Util::Transform m_BGTransform;
    Util::Transform m_LGTransform;

    float m_ZIndex_BG = 0.0f;
    float m_ZIndex_LG = 0.5f;
    float m_ZIndex_Play = 0.5f;

    Util::Transform m_PlayTransform; // 專門給按鈕用的位置
    bool m_IsHovering = false;       // 目前滑鼠是否在按鈕上
};

#endif