//
// Created by 吳梓暟 on 2026/5/22.
//

#ifndef ARROW_HPP
#define ARROW_HPP

#include "pch.hpp"
#include "GameObject.hpp"
#include "Util/Image.hpp"
#include <memory>

class Arrow : public GameObject {
public:
    // 建構子只需要傳入固定的 X 和 Y
    Arrow(float x, float y);

    void Update() override;
    void Draw(float cameraX, float cameraY) override;

private:
    float m_X;
    float m_Y;
    std::shared_ptr<Util::Image> m_Image;
};

#endif