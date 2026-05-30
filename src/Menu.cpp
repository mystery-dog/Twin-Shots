//
// Created by 吳梓暟 on 2026/5/28.
//

#include "Menu.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/TransformUtils.hpp"

Menu::Menu() {
    // 載入你的選單圖片
    BasicSet(m_BackgroundImage,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/141.jpg"),
        m_BGTransform,0.0f, 0.0f,2.35f, 1.8f);
    BasicSet(m_LogoImage,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/menu/menu.png"),
        m_LGTransform,0.0f, 150.0f,1.5f,1.5f);
    BasicSet(m_PlayImage,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/menu/play0.png"),
        m_PlayTransform,0.0f, -50.0f,1.0f,1.0f);
    //遮罩
    BasicSet(m_Block,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/block.png"),
        BlockTransform,0.0f, 0.0f,1.0f, 1.0f);
    BasicSet(m_PurpleBlock,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/level/instruct_bk.png"),
        PurpleBlockTransform,0.0f,0.0f,2.0f,2.0f);
    //按鈕
    BasicSet(m_Continue,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/continue0.png"),
        ContinueTransform,0.0f, -50.0f,1.5f, 2.0f);
    BasicSet(m_Endgame,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/endgame0.png"),
        EndgameTransform,0.0f, -125.0f,1.5f, 2.0f);
    BasicSet(m_Resue,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/restart0.png"),
        ResueTransform,0.0f, 0.0f,1.5f, 2.0f);
    BasicSet(m_RestartLevel,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/restart_level0.png"),
        RestartLevelTransform,0.0f, -50.0f,1.5f, 2.0f);
    BasicSet(m_Back,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/level/back_1.png"),
        BackTransform,0.0f,-200.0f,2.0f,2.0f);
    BasicSet(m_One,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/level/level1.png"),
        OneTransform,-200.0f,0.0f,2.0f,2.0f);
    BasicSet(m_Two,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/level/level2.png"),
        TwoTransform,0.0f,0.0f,2.0f,2.0f);
    BasicSet(m_Three,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/level/level3.png"),
        ThreeTransform,200.0f,0.0f,2.0f,2.0f);
    //字
    BasicSet(m_LevelFinished,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/level_finished.png"),
        LevelfinishedTransform,0.0f, 150.0f,2.0f, 1.5f);
    BasicSet(m_Pause,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/pause_label.png"),
        PauseTransform,0.0f,125.0f,1.2f,1.2f);
    BasicSet(m_LevelFailed,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/level_failed.png"),
      LevelFailedTransform,0.0f,150.0f,2.0f, 1.5f);
    BasicSet(m_SelectLevel,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/level/select_level.png"),
        SelectLevelTransform,0.0f,200.0f,2.5f,2.5f);

    BasicSet(m_ShotCat,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/shotcat.png"),
        ShotcatTransform,-150.0f, 50.0f,2.0f, 2.0f);
    BasicSet(m_TrophyCat,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/game_finished.png"),
        TrophycatTransform,0.0f,50.0f,2.0f,2.0f);


    std::string fontPath = RESOURCE_DIR"/fonts/Cubic_11.ttf";
    auto purpleColor = Util::Color::FromName(Util::Colors::PURPLE);
    m_PressSpaceText = std::make_shared<Util::Text>(fontPath, 30, "Press SPACE to continue", purpleColor);
    m_ScoreText = std::make_shared<Util::Text>(fontPath, 40, "Level score: " + std::to_string(m_CurrentScore), purpleColor);
    m_TotalScoreText = std::make_shared<Util::Text>(fontPath, 40, "Total score: " + std::to_string(m_TotalScore), purpleColor);
    m_CurrentScoreText = std::make_shared<Util::Text>(fontPath, 30, std::to_string(m_CurrentScore), Util::Color::FromName(Util::Colors::PINK));
}

void Menu::BasicSet(std::shared_ptr<Util::Image>& m_Image, std::shared_ptr<Util::Image> Image, Util::Transform& transform, float tx, float ty, float sx, float sy) {
    m_Image = Image;
    transform.translation = {tx,ty};
    transform.scale = {sx,sy};
}


void Menu::Update() {
    // 1. 不管在哪個畫面，背景圖永遠都要畫在最底層！
    m_BackgroundImage->Draw(Util::ConvertToUniformBufferData(m_BGTransform, m_BackgroundImage->GetSize(), m_ZIndex_BG));

    // 2. 狀態分流：根據是否按下 Play 鍵，決定要畫什麼 UI
    if (!m_StartPressed) {
        // 【狀態 A：還沒按 Play (主畫面)】
        // 畫出 Logo
        m_LogoImage->Draw(Util::ConvertToUniformBufferData(m_LGTransform, m_LogoImage->GetSize(), m_ZIndex_LG));
        // 處理並畫出 Play 按鈕
        Click(m_PlayImage, m_PlayTransform);
    } else {
        // 【狀態 B：已經按下 Play (選關卡畫面)】
        // 直接在這裡呼叫選關卡函式，不再畫 Logo 跟 Play！
        LevelSelect();
    }
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
            }else if (Image == m_Endgame || Image == m_Back) {
                m_ClickWhichButton = 1;
            }else if (Image == m_Resue) {
                m_ClickWhichButton = 2;
            }else if (Image == m_RestartLevel) {
                m_ClickWhichButton = 3;
            }else if (Image == m_One) {
                m_ClickWhichButton = 11;
            }else if (Image == m_Two) {
                m_ClickWhichButton = 12;
            }else if (Image == m_Three) {
                m_ClickWhichButton = 13;
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
    }else if (Image == m_Back) {
        auto currentPlayImg = m_IsHovering ? std::make_shared<Util::Image>(RESOURCE_DIR"/Image/level/back_2.png") : std::make_shared<Util::Image>(RESOURCE_DIR"/Image/level/back_1.png");
        auto matrices = Util::ConvertToUniformBufferData(BackTransform, currentPlayImg->GetSize(), 1.6f);
        currentPlayImg->Draw(matrices);
    }else if (Image == m_One) {
        auto currentPlayImg = m_IsHovering ? std::make_shared<Util::Image>(RESOURCE_DIR"/Image/level/level1_1.png") : std::make_shared<Util::Image>(RESOURCE_DIR"/Image/level/level1.png");
        auto matrices = Util::ConvertToUniformBufferData(OneTransform, currentPlayImg->GetSize(), 1.6f);
        currentPlayImg->Draw(matrices);
    }else if (Image == m_Two) {
        auto currentPlayImg = m_IsHovering ? std::make_shared<Util::Image>(RESOURCE_DIR"/Image/level/level2_2.png") : std::make_shared<Util::Image>(RESOURCE_DIR"/Image/level/level2.png");
        auto matrices = Util::ConvertToUniformBufferData(TwoTransform, currentPlayImg->GetSize(), 1.6f);
        currentPlayImg->Draw(matrices);
    }else if (Image == m_Three) {
        auto currentPlayImg = m_IsHovering ? std::make_shared<Util::Image>(RESOURCE_DIR"/Image/level/level3_3.png") : std::make_shared<Util::Image>(RESOURCE_DIR"/Image/level/level3.png");
        auto matrices = Util::ConvertToUniformBufferData(ThreeTransform, currentPlayImg->GetSize(), 1.6f);
        currentPlayImg->Draw(matrices);
    }
}



void Menu::Draw(float cameraX, float cameraY) {
    m_BackgroundImage->Draw(Util::ConvertToUniformBufferData(m_BGTransform, m_BackgroundImage->GetSize(), m_ZIndex_BG));
}

void Menu::LevelSelect() {
    auto matrices = Util::ConvertToUniformBufferData(PurpleBlockTransform,m_PurpleBlock->GetSize(),1.5f);
    m_PurpleBlock->Draw(matrices);
    matrices = Util::ConvertToUniformBufferData(SelectLevelTransform,m_SelectLevel->GetSize(),1.6f);
    m_SelectLevel->Draw(matrices);
    Click(m_One, OneTransform);
    Click(m_Two, TwoTransform);
    Click(m_Three, ThreeTransform);
    Click(m_Back, BackTransform);

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

void Menu::Win() {
    auto matrices = Util::ConvertToUniformBufferData(BlockTransform,m_Block->GetSize(),5.0f);

    m_Block->Draw(matrices);

    matrices = Util::ConvertToUniformBufferData(TrophycatTransform,m_TrophyCat->GetSize(),5.5f);
    m_TrophyCat->Draw(matrices);

    EndgameTransform.translation = {0.0f, -125.0f};
    Click(m_Endgame,EndgameTransform);
}

void Menu::ShowScore(float x, float y) {
    // 在畫圖前，把最新的分數「組合」成字串，然後塞給文字物件！
    m_CurrentScoreText->SetText(std::to_string(m_CurrentScore));
    m_CurrentScoreText->SetColor(Util::Color::FromName(Util::Colors::PINK));

    // 2. 取得這串文字現在「真實的寬度和高度」
    glm::vec2 size = m_CurrentScoreText->GetSize();

    // 3. 【核心魔法：靠右對齊】
    // 我們讓傳進來的 x 當作「最右邊界」。
    // 既然引擎看的是中心點，那我們就把中心點往左推 (減去寬度的一半)！
    uiTransform.translation = { x - (size.x / 2.0f), y };
    uiTransform.scale = {1.0f,1.0f};

    m_CurrentScoreText->Draw(Util::ConvertToUniformBufferData(uiTransform, m_CurrentScoreText->GetSize(), 1.6f));
}
