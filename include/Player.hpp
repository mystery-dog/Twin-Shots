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
#include "Util/Transform.hpp"     // 負責大小與位置
#include "Util/TransformUtils.hpp"// 助教提供的矩陣轉換工具

class Player : public Character {
public:
    // 建構子：在產生角色時，設定它的初始位置
    Player(float startX, float startY, std::shared_ptr<Map> map);

    // 實作 GameObject 介面規定的兩個函式
    void Update() override;

    int GetLife() { return  m_life; }

    void SetLife() { m_life = 3; }

    void SetBorder(float Height, float Width);
private:
    int m_life = 3;

    float m_BorderWidth = -640.0f;
    float m_BorderHeight = 400.0f;
};

#endif