//
// Created by 吳梓暟 on 2026/5/28.
//

#ifndef MENU_HPP
#define MENU_HPP

#include "GameObject.hpp"
#include "Util/Image.hpp"
#include <memory>
// 1. 【新增】在最上方引入 Text 與 Color 的標頭檔
#include "Util/Text.hpp"
#include "Util/Color.hpp"

class Menu : public GameObject{
public:
    Menu();
    void Update() override;
    void Draw(float cameraX, float cameraY) override;

    bool IsPressed() const { return m_StartPressed; }
    void SetPressed() { m_StartPressed = false; }

    int GetClickWhichButton() { return m_ClickWhichButton; }
    void SetClickWhichButton() { m_ClickWhichButton = -1; }

    void Click(std::shared_ptr<Util::Image> Image, Util::Transform transform);

    void LevelComplete();

    void Pause();

    void Gameover();

    void SetScore(int score);
    void SetCurrentScore() { m_CurrentScore = 0; }
    void SetTotalScore() { m_TotalScore = 0; }


private:
    std::shared_ptr<Util::Image> m_LogoImage;
    std::shared_ptr<Util::Image> m_PlayImage;
    std::shared_ptr<Util::Image> m_BackgroundImage;


    // 【新增】用來裝載全螢幕半透明遮罩的圖片
    std::shared_ptr<Util::Image> m_Block;       //半透明遮罩

    std::shared_ptr<Util::Image> m_Continue;    //continue按鈕
    std::shared_ptr<Util::Image> m_Endgame;     //endgame按鈕
    std::shared_ptr<Util::Image> m_Resue;     //resue按鈕
    std::shared_ptr<Util::Image> m_RestartLevel;     //resue按鈕

    std::shared_ptr<Util::Image> m_LevelFinished;//字
    std::shared_ptr<Util::Image> m_Pause;//字
    std::shared_ptr<Util::Image> m_LevelFailed;//字

    std::shared_ptr<Util::Image> m_ShotCat;     //射箭貓小圖

    std::shared_ptr<Util::Text> m_PressSpaceText;     // 提示文字
    std::shared_ptr<Util::Text> m_ScoreText;          // 【新增】用來顯示分數
    std::shared_ptr<Util::Text> m_TotalScoreText;          // 【新增】用來顯示分數

    Util::Transform uiTransform;

    Util::Transform BlockTransform;

    Util::Transform ContinueTransform;
    Util::Transform EndgameTransform;
    Util::Transform ResueTransform;
    Util::Transform RestartLevelTransform;

    Util::Transform LevelfinishedTransform;
    Util::Transform PauseTransform;
    Util::Transform LevelFailedTransform;

    Util::Transform ShotcatTransform;

    bool m_StartPressed = false;
    Util::Transform m_BGTransform;
    Util::Transform m_LGTransform;

    float m_ZIndex_BG = 0.0f;
    float m_ZIndex_LG = 0.5f;
    float m_ZIndex_Play = 0.5f;

    Util::Transform m_PlayTransform; // 專門給按鈕用的位置
    bool m_IsHovering = false;       // 目前滑鼠是否在按鈕上

    //
    int m_ClickWhichButton = -1;
    // 【新增】測試用的分數變數
    int m_CurrentScore = 0;
    int m_TotalScore = 0;
};

#endif