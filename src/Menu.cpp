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
    m_PlayImage = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/menu/play0.png");

    m_Block = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/block.png");

    m_Continue = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/continue0.png");
    m_Endgame = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/endgame0.png");
    m_Resue = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/restart0.png");
    m_RestartLevel = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/restart_level0.png");

    m_LevelFinished = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/level_finished.png");
    m_Pause = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/pause_label.png");
    m_LevelFailed = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/level_failed.png");

    m_ShotCat = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/shotcat.png");

    m_BGTransform.scale = {2.35f, 1.8f};
    m_BGTransform.translation = {0.0f, 0.0f};

    m_LGTransform.translation = {0.0f, 150.0f};
    m_LGTransform.scale = {1.5f,1.5f};

    m_PlayTransform.scale = {1.0f,1.0f};
    m_PlayTransform.translation = {0.0f, -50.0f};

    // 1. 宣告 Transform
    BlockTransform.translation = {0.0f, 0.0f}; // 貼死在螢幕正中心
    BlockTransform.scale = {1.0f, 1.0f};       // 圖片原始比例

    ContinueTransform.translation = {0.0f, -50.0f}; //
    ContinueTransform.scale = {1.5f, 2.0f};       // 圖片原始比例

    ResueTransform.translation = {0.0f, 0.0f};
    ResueTransform.scale = {1.5f, 2.0f};

    RestartLevelTransform = ContinueTransform;

    EndgameTransform.translation = {0.0f, -125.0f}; //
    EndgameTransform.scale = {1.5f, 2.0f};       // 圖片原始比例

    LevelfinishedTransform.translation = {0.0f, 150.0f}; //
    LevelfinishedTransform.scale = {2.0f, 1.5f};       // 圖片原始比例

    PauseTransform.translation = {0.0f,125.0f};
    PauseTransform.scale = {1.2f,1.2f};

    LevelFailedTransform.translation = {0.0f,150.0f};
    LevelFailedTransform.scale = {2.0f, 1.5f};

    ShotcatTransform.translation = {-150.0f, 50.0f}; //
    ShotcatTransform.scale = {2.0f, 2.0f};       // 圖片原始比例

    std::string fontPath = RESOURCE_DIR"/fonts/Cubic_11.ttf";
    auto purpleColor = Util::   Color::FromName(Util::Colors::PURPLE);
    m_PressSpaceText = std::make_shared<Util::Text>(fontPath, 30, "Press SPACE to continue", purpleColor);
    m_ScoreText = std::make_shared<Util::Text>(fontPath, 40, "Level score: " + std::to_string(m_CurrentScore), purpleColor);
    m_TotalScoreText = std::make_shared<Util::Text>(fontPath, 40, "Total score: " + std::to_string(m_TotalScore), purpleColor);
}

void Menu::Update() {
    Click(m_PlayImage,m_PlayTransform);
    Draw(0.0f,0.0f);
}

void Menu::Click(std::shared_ptr<Util::Image> Image, Util::Transform transform) {
    // 1. 取得滑鼠位置
    glm::vec2 mousePos = Util::Input::GetCursorPosition();

    // 2. 取得按鈕尺寸
    glm::vec2 btnSize = Image->GetSize() * transform.scale;
    float halfW = btnSize.x / 2.0f;
    float halfH = btnSize.y / 2.0f;

    // 3. 判定滑鼠是否在按鈕矩形範圍內
    if (mousePos.x > transform.translation.x - halfW &&
        mousePos.x < transform.translation.x + halfW &&
        mousePos.y > transform.translation.y - halfH &&
        mousePos.y < transform.translation.y + halfH) {

        m_IsHovering = true;

        // 4. 如果懸停且按下左鍵
        if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
            if (Image == m_PlayImage) {
                m_StartPressed = true;
            }else if (Image == m_Continue) {
                m_ClickWhichButton = 0;
            }else if (Image == m_Endgame) {
                m_ClickWhichButton = 1;
            }else if (Image == m_Resue) {
                m_ClickWhichButton = 2;
            }else if (Image == m_RestartLevel) {
                m_ClickWhichButton = 3;
            }
        }

    } else{
            m_IsHovering = false;
    }
    if (Image == m_PlayImage) {
        auto currentPlayImg = m_IsHovering ? std::make_shared<Util::Image>(RESOURCE_DIR"/Image/menu/play1.png") : std::make_shared<Util::Image>(RESOURCE_DIR"/Image/menu/play0.png");
        auto matrices = Util::ConvertToUniformBufferData(m_PlayTransform, currentPlayImg->GetSize(), 0.5f);
        currentPlayImg->Draw(matrices);
    }else if (Image == m_Continue) {
        auto currentPlayImg = m_IsHovering ? std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/continue1.png") : std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/continue0.png");
        auto matrices = Util::ConvertToUniformBufferData(ContinueTransform, currentPlayImg->GetSize(), 5.5f);
        currentPlayImg->Draw(matrices);
    }else if (Image == m_Endgame) {
        auto currentPlayImg = m_IsHovering ? std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/endgame1.png") : std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/endgame0.png");
        auto matrices = Util::ConvertToUniformBufferData(EndgameTransform, currentPlayImg->GetSize(), 5.5f);
        currentPlayImg->Draw(matrices);
    }else if (Image == m_Resue) {
        auto currentPlayImg = m_IsHovering ? std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/restart1.png") : std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/restart0.png");
        auto matrices = Util::ConvertToUniformBufferData(ResueTransform, currentPlayImg->GetSize(), 5.5f);
        currentPlayImg->Draw(matrices);
    }else if (Image == m_RestartLevel) {
        auto currentPlayImg = m_IsHovering ? std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/restart_level1.png") : std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/restart_level0.png");
        auto matrices = Util::ConvertToUniformBufferData(RestartLevelTransform, currentPlayImg->GetSize(), 5.5f);
        currentPlayImg->Draw(matrices);
    }
}



void Menu::Draw(float cameraX, float cameraY) {
    m_BackgroundImage->Draw(Util::ConvertToUniformBufferData(m_BGTransform, m_BackgroundImage->GetSize(), m_ZIndex_BG));
    m_LogoImage->Draw(Util::ConvertToUniformBufferData(m_LGTransform, m_LogoImage->GetSize(), m_ZIndex_LG));
}

void Menu::LevelComplete() {
    // 2. 利用助教的工具轉換出標準矩陣
    // 注意這裡的 Z-Index 我給了 0.9f (一個很高的數字)，確保它一定會蓋在場景的最上層！
    auto matrices = Util::ConvertToUniformBufferData(BlockTransform,m_Block->GetSize(),5.0f);

    // 3. 正式畫出朦朧布
    m_Block->Draw(matrices);

    matrices = Util::ConvertToUniformBufferData(LevelfinishedTransform,m_LevelFinished->GetSize(),5.5f);
    m_LevelFinished->Draw(matrices);
    matrices = Util::ConvertToUniformBufferData(ShotcatTransform,m_ShotCat->GetSize(),5.5f);
    m_ShotCat->Draw(matrices);

    uiTransform.scale = {0.7f, 0.7f};

    // 3. 【核心魔法：動態更新分數】(放在正中間 0.0f)
    if (m_ScoreText) {
        // 在畫圖前，把最新的分數「組合」成字串，然後塞給文字物件！
        m_ScoreText->SetText("Level score:  " + std::to_string(m_CurrentScore));

        uiTransform.translation = {25.0f, 75.5f};
        m_ScoreText->Draw(Util::ConvertToUniformBufferData(uiTransform, m_ScoreText->GetSize(), 5.5f));
    }
    if (m_TotalScoreText) {
        // 在畫圖前，把最新的分數「組合」成字串，然後塞給文字物件！
        m_TotalScoreText->SetText("Total score:  " + std::to_string(m_TotalScore));

        uiTransform.translation = {25.0f, 25.5f};
        m_TotalScoreText->Draw(Util::ConvertToUniformBufferData(uiTransform, m_TotalScoreText->GetSize(), 5.5f));
    }
    // 4. 畫上提示文字 (往下移動 200)
    if (m_PressSpaceText) {
        uiTransform.translation = {0.0f, -200.0f};
        uiTransform.scale = {1.2f, 1.2f};
        m_PressSpaceText->Draw(Util::ConvertToUniformBufferData(uiTransform, m_PressSpaceText->GetSize(), 5.5f));
    }
    // 5. 等待玩家按下空白鍵切換下一關
    if (Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
        m_ClickWhichButton = 0;
    }

    Click(m_Continue, ContinueTransform);
    EndgameTransform.translation = {0.0f, -125.0f};
    Click(m_Endgame, EndgameTransform);

}

void Menu::SetScore(int score) {
    m_CurrentScore += score;
    m_TotalScore += score;
}

void Menu::Pause() {
    auto matrices = Util::ConvertToUniformBufferData(BlockTransform,m_Block->GetSize(),5.0f);

    m_Block->Draw(matrices);

    matrices = Util::ConvertToUniformBufferData(PauseTransform,m_Pause->GetSize(),5.5f);
    m_Pause->Draw(matrices);

    if (m_PressSpaceText) {
        uiTransform.translation = {0.0f, -200.0f};
        uiTransform.scale = {1.2f, 1.2f};
        m_PressSpaceText->Draw(Util::ConvertToUniformBufferData(uiTransform, m_PressSpaceText->GetSize(), 5.5f));
    }
    Click(m_Resue, ResueTransform);
    EndgameTransform.translation = {0.0f, -75.0f};
    Click(m_Endgame, EndgameTransform);
    // 5. 等待玩家按下空白鍵切換下一關
    if (Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
        m_ClickWhichButton = 2;
    }
}

void Menu::Gameover() {
    auto matrices = Util::ConvertToUniformBufferData(BlockTransform,m_Block->GetSize(),5.0f);

    m_Block->Draw(matrices);

    matrices = Util::ConvertToUniformBufferData(LevelFailedTransform,m_LevelFailed->GetSize(),5.5f);
    m_LevelFailed->Draw(matrices);

    if (m_TotalScoreText) {
        // 在畫圖前，把最新的分數「組合」成字串，然後塞給文字物件！
        m_TotalScoreText->SetText("Final score:  " + std::to_string(m_TotalScore));

        uiTransform.translation = {0.0f, 50.f};
        uiTransform.scale = {0.7f, 0.7f};
        m_TotalScoreText->Draw(Util::ConvertToUniformBufferData(uiTransform, m_TotalScoreText->GetSize(), 5.5f));
    }

    if (m_PressSpaceText) {
        uiTransform.translation = {0.0f, -200.0f};
        uiTransform.scale = {1.2f, 1.2f};
        m_PressSpaceText->Draw(Util::ConvertToUniformBufferData(uiTransform, m_PressSpaceText->GetSize(), 5.5f));
    }
    Click(m_RestartLevel, RestartLevelTransform);
    EndgameTransform.translation = {0.0f, -125.0f};
    Click(m_Endgame, EndgameTransform);
    // 5. 等待玩家按下空白鍵切換下一關
    if (Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
        m_ClickWhichButton = 3;
    }
}


