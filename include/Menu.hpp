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

    void BasicSet(std::shared_ptr<Util::Image>& m_Image, std::shared_ptr<Util::Image> Image, Util::Transform& transform, float tx, float ty, float sx, float sy);

    bool IsPressed() const { return m_StartPressed; }
    void SetPressed() { m_StartPressed = false; }

    int GetClickWhichButton() { return m_ClickWhichButton; }
    void SetClickWhichButton() { m_ClickWhichButton = -1; }

    void Click(std::shared_ptr<Util::Image> Image, Util::Transform transform);

    void LevelSelect();

    void LevelComplete();

    void Pause();

    void Gameover();

    void Win();

    void SetScore(int score);
    void SetCurrentScore() { m_CurrentScore = 0; }
    void SetTotalScore() { m_TotalScore = 0; }


private:
    std::shared_ptr<Util::Image> m_LogoImage;
    std::shared_ptr<Util::Image> m_PlayImage;
    std::shared_ptr<Util::Image> m_BackgroundImage;


    // 【新增】用來裝載全螢幕半透明遮罩的圖片
    std::shared_ptr<Util::Image> m_Block;       //半透明遮罩
    std::shared_ptr<Util::Image> m_PurpleBlock;       //紫色遮罩（選關卡）

    std::shared_ptr<Util::Image> m_Continue;    //continue按鈕
    std::shared_ptr<Util::Image> m_Endgame;     //endgame按鈕
    std::shared_ptr<Util::Image> m_Resue;     //resue按鈕
    std::shared_ptr<Util::Image> m_RestartLevel;     //restartLevel按鈕
    std::shared_ptr<Util::Image> m_Back;     //back按鈕
    std::shared_ptr<Util::Image> m_One;     //1按鈕
    std::shared_ptr<Util::Image> m_Two;     //2按鈕
    std::shared_ptr<Util::Image> m_Three;     //3按鈕

    std::shared_ptr<Util::Image> m_LevelFinished;//字
    std::shared_ptr<Util::Image> m_Pause;//字
    std::shared_ptr<Util::Image> m_LevelFailed;//字
    std::shared_ptr<Util::Image> m_SelectLevel;//字

    std::shared_ptr<Util::Image> m_ShotCat;     //射箭貓小圖
    std::shared_ptr<Util::Image> m_TrophyCat;     //獎盃貓圖

    std::shared_ptr<Util::Text> m_PressSpaceText;     // 提示文字
    std::shared_ptr<Util::Text> m_ScoreText;          // 【新增】用來顯示分數
    std::shared_ptr<Util::Text> m_TotalScoreText;          // 【新增】用來顯示分數

    Util::Transform uiTransform;

    Util::Transform BlockTransform;
    Util::Transform PurpleBlockTransform;

    Util::Transform ContinueTransform;
    Util::Transform EndgameTransform;
    Util::Transform ResueTransform;
    Util::Transform RestartLevelTransform;
    Util::Transform BackTransform;
    Util::Transform OneTransform;
    Util::Transform TwoTransform;
    Util::Transform ThreeTransform;

    Util::Transform LevelfinishedTransform;
    Util::Transform PauseTransform;
    Util::Transform LevelFailedTransform;
    Util::Transform SelectLevelTransform;

    Util::Transform ShotcatTransform;
    Util::Transform TrophycatTransform;

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