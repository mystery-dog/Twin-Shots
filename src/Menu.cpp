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
    BasicSet(m_HelpImage,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/menu/help1.png"),
        m_HelpTransform,-5.0f, -125.0f,2.0f,1.8f);
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
    //其他
    BasicSet(m_ShotCat,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/shotcat.png"),
        ShotcatTransform,-150.0f, 50.0f,2.0f, 2.0f);
    BasicSet(m_TrophyCat,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/game_finished.png"),
        TrophycatTransform,0.0f,50.0f,2.0f,2.0f);
    BasicSet(m_Left,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/menu/left.png"),
        LeftTransform,-250.0f,-25.0f,3.5f,3.5f);
    BasicSet(m_Right,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/menu/right.png"),
        RightTransform,-50.0f,-25.0f,3.5f,3.5f);
    BasicSet(m_Up,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/menu/up.png"),
        UpTransform,-150.0f,75.0f,3.5f,3.5f);
    BasicSet(m_Space,std::make_shared<Util::Image>(RESOURCE_DIR"/Image/menu/space.png"),
        SpaceTransform,150.0f,25.0f,8.0f,3.5f);

    std::string fontPath = RESOURCE_DIR"/fonts/Cubic_11.ttf";
    auto purpleColor = Util::Color::FromName(Util::Colors::PURPLE);
    m_PressSpaceText = std::make_shared<Util::Text>(fontPath, 30, "Press SPACE to continue", purpleColor);
    m_ScoreText = std::make_shared<Util::Text>(fontPath, 40, "Level score: " + std::to_string(m_CurrentScore), purpleColor);
    m_TotalScoreText = std::make_shared<Util::Text>(fontPath, 40, "Total score: " + std::to_string(m_TotalScore), purpleColor);
    m_CurrentScoreText = std::make_shared<Util::Text>(fontPath, 30, std::to_string(m_CurrentScore), Util::Color::FromName(Util::Colors::PINK));
    // 【新增】設定 Help 畫面的文字內容 (字型、大小、文字、顏色)
    m_HelpText1 = std::make_shared<Util::Text>(fontPath, 35, "跳躍", Util::Color::FromName(Util::Colors::WHITE));
    m_HelpText2 = std::make_shared<Util::Text>(fontPath, 35, "射箭(攻擊)", Util::Color::FromName(Util::Colors::WHITE));
    m_HelpText3 = std::make_shared<Util::Text>(fontPath, 35, "向左移動     向右移動", Util::Color::FromName(Util::Colors::WHITE));
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
        // ==========================================
        // 【核心修正】：判斷現在是否開啟了 Help 面板
        // ==========================================
        if (m_HelpPressed) {
            // 狀態 A-1：開啟 Help 說明時
            // 直接「畫出」沒有 Hover 效果的原始圖片，不呼叫 Click()！
            // 這樣按鈕看得到，但完全點不到！
            auto playMatrices = Util::ConvertToUniformBufferData(m_PlayTransform, m_PlayImage->GetSize(), 0.5f);
            m_PlayImage->Draw(playMatrices);

            auto helpMatrices = Util::ConvertToUniformBufferData(m_HelpTransform, m_HelpImage->GetSize(), 0.5f);
            m_HelpImage->Draw(helpMatrices);

            // 畫完死板的背景按鈕後，才把 Help 面板疊加在最上層
            Help();

        } else {
            // 狀態 A-2：正常主畫面
            // 呼叫 Click 讓按鈕正常偵測滑鼠發光並接收點擊
            Click(m_PlayImage, m_PlayTransform);
            Click(m_HelpImage, m_HelpTransform);
        }
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
            }else if (Image == m_HelpImage) {
                m_HelpPressed = true;
            }else if (Image == m_Continue) {
                m_ClickWhichButton = 0;
            }else if (Image == m_Endgame) {
                m_ClickWhichButton = 1;
            }else if (Image == m_Back) {
                if (m_HelpPressed) {
                    // 如果現在是在 Help 畫面，按 Back 只要關閉 Help 就好！
                    m_HelpPressed = false;
                } else {
                    // 如果是在選關卡畫面，按 Back 就把 Play 的狀態取消，退回主 Logo 畫面！
                    m_StartPressed = false;
                }
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
    }else if (Image == m_HelpImage) {
        auto currentPlayImg = m_IsHovering ? std::make_shared<Util::Image>(RESOURCE_DIR"/Image/menu/help2.png") : std::make_shared<Util::Image>(RESOURCE_DIR"/Image/menu/help1.png");
        auto matrices = Util::ConvertToUniformBufferData(m_HelpTransform, currentPlayImg->GetSize(), 0.5f);
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

void Menu::Help() {
    auto matrices = Util::ConvertToUniformBufferData(PurpleBlockTransform,m_PurpleBlock->GetSize(),1.5f);
    m_PurpleBlock->Draw(matrices);
    matrices = Util::ConvertToUniformBufferData(LeftTransform,m_Left->GetSize(),1.6f);
    m_Left->Draw(matrices);
    matrices = Util::ConvertToUniformBufferData(RightTransform,m_Left->GetSize(),1.6f);
    m_Right->Draw(matrices);
    matrices = Util::ConvertToUniformBufferData(UpTransform,m_Left->GetSize(),1.6f);
    m_Up->Draw(matrices);
    matrices = Util::ConvertToUniformBufferData(SpaceTransform,m_Left->GetSize(),1.6f);
    m_Space->Draw(matrices);

    // 2. 【新增】畫出說明文字 (Z-Index = 1.6f，要比紫板高才不會被蓋住！)
    uiTransform.scale = {1.0f, 1.0f}; // 養成好習慣，畫圖前先把比例歸零

    if (m_HelpText1) {
        uiTransform.translation = {-150.0f, 20.0f}; // 放在畫面偏上方
        m_HelpText1->Draw(Util::ConvertToUniformBufferData(uiTransform, m_HelpText1->GetSize(), 1.6f));
    }

    if (m_HelpText2) {
        uiTransform.translation = {150.0f, -30.0f}; // 放在第一行的下方
        m_HelpText2->Draw(Util::ConvertToUniformBufferData(uiTransform, m_HelpText2->GetSize(), 1.6f));
    }

    if (m_HelpText3) {
        uiTransform.translation = {-150.0f, -75.0f}; // 放在第一行的下方
        m_HelpText3->Draw(Util::ConvertToUniformBufferData(uiTransform, m_HelpText3->GetSize(), 1.6f));
    }

    Click(m_Back, BackTransform);
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
