#include "App.hpp"

#include "Emeny.hpp"
#include "Player.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Transform.hpp"


void App::Start() {

    LOG_TRACE("Start");

    if (!m_Menu) {
        m_Menu = std::make_shared<Menu>();
    }
    m_Menu->Update();
    m_Menu->SetCurrentScore();
    m_Menu->SetTotalScore();
    Stop = false;

    // 取得玩家按下的按鈕代號
    int btn = m_Menu->GetClickWhichButton();

    // 【核心修正】：只有在按下 1、2、3 關卡按鈕時 (代號 11, 12, 13)，才允許進入遊戲！
    if (btn == 11 || btn == 12 || btn == 13) {

        if (btn == 11) m_CurrentLevel = 1;
        else if (btn == 12) m_CurrentLevel = 2;
        else if (btn == 13) m_CurrentLevel = 3;

        if (!m_Map) {
            m_Map = std::make_shared<Map>();
        }

        Level();

        // 【新增】設定相機的臨界值與邊界
        // 1. 設定緩衝區 (Deadzone)：
        // X 設為 200.0f 代表玩家在螢幕正中心左右 200 像素內移動時，相機都不會動。
        // 你可以自己微調這個數字，找回原版遊戲的手感。
        m_Camera.SetDeadzone(200.0f, 50.0f);

        // 切換狀態前，把 Menu 的按鈕與點擊狀態「洗掉」！
        m_Menu->SetPressed();
        m_Menu->SetClickWhichButton(); // 【新增】把按鈕狀態重置回 -1，避免帶入遊戲中

        // 正式切換狀態進入遊戲循環
        m_CurrentState = State::UPDATE;
    }
    // 原本引擎內建的偵測：當按下 ESC 鍵或點擊視窗關閉鈕時，準備結束遊戲
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

}

void App::Update() {

    // 【新增】判斷現在是不是 Game Over 狀態
    bool isGameOver = (m_Player->GetLife() == 0);

    // ==================== 【大改動：多型架構的自動化更新與繪製】 ====================
    // 1. 【新增】更新相機的位置 (目標是追蹤玩家)
    // 玩家走到哪，相機的 X, Y 就跟到哪
    m_Camera.Update(m_Player->GetX(), m_Player->GetY());

    // 這裡使用了範圍 for 迴圈 (Range-based for loop)，它會自動遍歷清單中的每一個物件。
    // 對 App 而言，它只知道裡面的東西叫作 "GameObject"，根本不關心它是地圖還是角色。
    // 主程式只需要無腦呼叫 obj->Update() 和 obj->Draw()，
    // C++ 執行期就會透過虛擬函式表 (Vtable) 自動導向去執行 Map::Update() 與 Map::Draw()！
    for (auto& obj : m_GameObjects) {
        // ==========================================
        // 【核心魔法：暫停邏輯】
        // 只有活著的時候，才呼叫 Update() 讓他們移動、算物理、扣血
        // ==========================================
        if (!isGameOver && !Stop) {
            obj->Update(); // 執行該物件的邏輯更新 (目前地圖內部留空，未來角色會用來偵測按鍵)
        }

        // ==========================================
        // 畫面渲染 (Draw) 不管怎樣都要執行！
        // 這樣就算時間暫停了，玩家還是看得到死掉的小惡魔和定格的怪物
        // ==========================================
        obj->Draw(m_Camera.GetX(), m_Camera.GetY());   // 執行該物件的畫面渲染 (會去調用助教的矩陣工具並把圖畫出來)
        m_Menu->ShowScore(620.0f, -333.0f);
    }





    //<Ying>
    // ==================== 【從舊版合併：自動清除不活躍的物件】 ====================
    // 例如死掉的怪物或消失的箭矢，會透過這個機制徹底移出記憶體，避免記憶體洩漏
    m_GameObjects.erase(
        std::remove_if(m_GameObjects.begin(), m_GameObjects.end(),
            [](const std::shared_ptr<GameObject>& obj) {
                // 檢查是否是不活躍的角色（如播完死亡動畫的怪物）
                auto character = std::dynamic_pointer_cast<Character>(obj);
                if (character != nullptr && !character->IsActive()) return true;

                // 檢查是否是已經死掉（出界或射中目標）的箭矢
                auto arrow = std::dynamic_pointer_cast<Arrow>(obj);
                if (arrow != nullptr && arrow->IsDead()) return true;

                return false;
            }),
        m_GameObjects.end()
    );
    // ==================== 【核心新增：箭矢與怪物的碰撞偵測】 ===================
    if (m_Player) {
        // 透過剛寫的 Getter 拿到玩家身上所有的箭
        for (auto& arrow : m_Player->GetArrows()) {

            // 只有「還在飛」且「沒插在牆上」的箭才能傷怪
            if (!arrow->IsDead() && !arrow->IsStuck()) {

                for (auto& obj : m_GameObjects) {
                    auto enemy = std::dynamic_pointer_cast<Emeny>(obj);

                    // 找到場上還活著的怪物
                    if (enemy != nullptr && !enemy->IsDead()) {

                        // 1. 計算箭與怪物的中心點距離
                        float deltaX = std::abs(arrow->GetX() - enemy->GetX());
                        float deltaY = std::abs(arrow->GetY() - enemy->GetY());

                        // 2. 取得兩者的碰撞範圍 (圖片大小的一半)
                        float arrowHalfW = arrow->GetSize().x * 0.5f;
                        float arrowHalfH = arrow->GetSize().y * 0.5f;
                        float enemyHalfW = enemy->GetImageSize().x * 0.5f;
                        float enemyHalfH = enemy->GetImageSize().y * 0.5f;

                        // 3. AABB 矩形碰撞公式判定
                        if (deltaX < (arrowHalfW + enemyHalfW) && deltaY < (arrowHalfH + enemyHalfH)) {

                            // 【🎯 擊中成功！】
                            enemy->SetLifeToZero(); // 讓怪物血量歸零（觸發牠在自己的 Update 播死亡動畫）
                            enemy->Die();           // 標記已死（讓下面的過關清點數量立刻扣除）
                            arrow->Die();           // 讓箭矢死掉

                            m_Menu->SetScore(100);  // 射中得分！
                            break; // 這支箭已經爆了，跳出怪物迴圈，換下一支箭
                        }
                    }
                }
            }
        }
    }

    // ====== 【核心新增：玩家與怪物的受傷碰撞偵測】 ======
    // 只有在遊戲沒結束、沒暫停，且玩家活著時才偵測
    if (m_Player && m_Player->GetLife() > 0 && !isGameOver && !Stop) {

        // 如果玩家目前已經是無敵狀態，我們就不用浪費效能去算碰撞了
        if (!m_Player->IsInvincible()) {

            // 取得玩家的碰撞範圍 (半寬與半高)
            // 這裡假設 m_Player 也有 GetImageSize() 或類似拿到寬高的方法
            // 如果是用 GetSize() 則依據你 Player 的設計修改
            float playerX = m_Player->GetX();
            float playerY = m_Player->GetY();
            float playerHalfW = m_Player->GetSize().x * 0.5f;
            float playerHalfH = m_Player->GetSize().y * 0.5f;

            // 遍歷所有遊戲物件，尋找活著的怪物
            for (auto& obj : m_GameObjects) {
                auto enemy = std::dynamic_pointer_cast<Emeny>(obj);

                // 找到場上還活著的怪物
                if (enemy != nullptr && !enemy->IsDead()) {

                    // 1. 計算玩家與怪物的中心點距離
                    float deltaX = std::abs(playerX - enemy->GetX());
                    float deltaY = std::abs(playerY - enemy->GetY());

                    // 2. 取得怪物的碰撞範圍
                    float enemyHalfW = enemy->GetImageSize().x * 0.5f;
                    float enemyHalfH = enemy->GetImageSize().y * 0.5f;

                    // 3. AABB 矩形碰撞公式判定
                    if (deltaX < (playerHalfW + enemyHalfW) && deltaY < (playerHalfH + enemyHalfH)) {

                        // 【💥 玩家撞到怪！】
                        m_Player->Hurt(); // 呼叫玩家受傷功能（會自動扣血、變更受傷圖片、開啟無敵閃爍）

                        break; // 已經撞到一隻怪並受傷了，立刻跳出迴圈，避免同一幀內被多隻怪重複判定
                    }
                }
            }
        }
    }
    // ===================================================
    //</Ying>




    // ==========================================
    // 2. 【新增】過關判定：清點活著的怪物數量
    // ==========================================
    int aliveEnemyCount = 0;

    for (auto& obj : m_GameObjects) {
        // 【核心魔法：動態轉型 (Dynamic Pointer Cast)】
        // 嘗試把這個「未知物件(GameObject)」當成「怪物(Emeny)」來看
        auto enemy = std::dynamic_pointer_cast<Emeny>(obj);

        // 如果 enemy 不是空的，代表它真的是怪物！(如果是地圖或玩家，轉型失敗會變成 nullptr)
        if (enemy != nullptr) {
            // 檢查這隻怪物死了沒？如果還沒死，活著的數量就 +1
            if (!enemy->IsDead()) {
                aliveEnemyCount++;
            }
        }
    }
    // 3. 如果場上活著的怪物數量歸零，自動進入下一關！
    if (aliveEnemyCount == 0 ) {
        Stop = true;
        if (m_CurrentLevel < 3) {
            m_Menu->LevelComplete();
        }else {
            if (!m_GameObjects.empty()) m_GameObjects.clear();
            m_Menu->Draw(0.0f,0.0f);
            m_Menu->Win();
        }
    }

    if (m_Menu->GetClickWhichButton() == 0) {
        m_CurrentLevel++;

        Level(); // 載入新關卡
        m_Player->SetLife(); // 幫玩家補滿血

        m_Menu->SetCurrentScore();//重置關卡分數

        m_Menu->SetClickWhichButton();//重置按鈕狀態
        Stop = false;
    }else if (m_Menu->GetClickWhichButton() == 1) {
        if (!m_GameObjects.empty()) m_GameObjects.clear();
        m_Map->SetPausePressed();
        m_Menu->SetClickWhichButton();//重置按鈕狀態
        m_Menu->SetPressed();//重置play按鈕狀態
        m_CurrentState = State::START;
        Start();
    }else if (m_Menu->GetClickWhichButton() == 2) {
        Stop = false;
        m_Map->SetPausePressed();
        m_Menu->SetClickWhichButton();//重置按鈕狀態
    }else if (m_Menu->GetClickWhichButton() == 3) {
        Stop = false;
        if (!m_GameObjects.empty()) m_GameObjects.clear();
        Level();
        m_Menu->SetClickWhichButton();//重置按鈕狀態
    }
    if (m_Map->GetPauseState()) {
        Stop = true;
        m_Menu->Pause();
    }
    if (m_Player->GetLife() == 0) {
        m_Menu->Gameover();
    }

    // 原本引擎內建的偵測：當按下 ESC 鍵或點擊視窗關閉鈕時，準備結束遊戲
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }


    Cheating();
}

void App::Level() {
    // ==========================================
    // 0. 【關鍵修復】清空上一關所有的物件！
    // ==========================================
    // 這樣舊的地圖、舊的玩家、舊的怪物才會徹底從記憶體中消失
    if (!m_GameObjects.empty()) m_GameObjects.clear();

    // ==================== 【大改動：動態建立並載入第一關地圖】 ====================

    // 2. 讓地圖去讀取你已經建立好的第一關地圖檔案
    //    請務必確認 "level1.txt" 有被妥善放在專案的 assets 資料夾中
    m_Map->LoadFromFile(RESOURCE_DIR"/map/level" +  std::to_string(m_CurrentLevel) + ".txt");

    // 3. 將這個地圖物件塞進我們在 App.hpp 宣告好的物件清單 (m_GameObjects) 尾端
    //    此時 Map* 指標會自動向上轉型 (Upcasting) 儲存為 GameObject* 指標
    m_GameObjects.push_back(m_Map);

    if (m_CurrentLevel == 1) {
        // ==========================================
        // 2. 【新增】建立角色
        // ==========================================
        // 呼叫角色的建構子，並給予初始位置 (例如 X=600, Y=-300)
        // 這個位置可以讓角色一開始出現在柱子附近的半空中
        m_Player = std::make_shared<Player>(-500.0f, -85.0f, m_Map);
        m_Map->SetPlayer(m_Player);

        // 直接把角色塞進物件清單中！
        // App::Update 裡面的迴圈會自動幫我們呼叫角色的 Update() 和 Draw()！
        m_GameObjects.push_back(m_Player);

        m_Player->SetBorder(400.0f,-640.0f);
        m_Player->SetMapmax(400.0f,700.0f);
        // ==========================================
        // 3. 【新增】建立敵人 (史萊姆)
        // ==========================================
        // 在地圖上產生一隻怪物，給它一個初始座標 (例如 X=100, Y=-50)
        auto enemy1 = std::make_shared<Emeny>(ENEMY_ID::Slime,75.0f, -75.0f, m_Map);
        auto enemy2 = std::make_shared<Emeny>(ENEMY_ID::Slime,200.0f, -75.0f, m_Map);
        auto enemy3 = std::make_shared<Emeny>(ENEMY_ID::Slime,-50.0f, -75.0f, m_Map);

        // 直接塞進多型清單裡！App::Update 會自動幫它呼叫 Update() 和 Draw()！
        m_GameObjects.push_back(enemy1);
        m_GameObjects.push_back(enemy2);
        m_GameObjects.push_back(enemy3);

        // 2. 設定相機邊界 (Bounds)：
        // 這 4 個數字決定了相機能拍到的極限位置。
        // 如果你發現畫面左邊露出了黑色的虛空，就把 minX 加大 (例如從 -200 改成 0)。
        // 如果右邊露出了虛空，就把 maxX 減小。
        // ★ 這裡我先填入估計值，你需要根據實際跑出來的畫面做微調 ★
        m_Camera.SetBounds(0.0f, 55.5f, -165.0f, 55.0f);//地圖一
    }
    if (m_CurrentLevel == 2) {
        m_Player = std::make_shared<Player>(-500.0f, -360.0f, m_Map);
        m_Map->SetPlayer(m_Player);
        m_GameObjects.push_back(m_Player);
        m_Player->SetBorder(500.0f,-640.0f);
        m_Player->SetMapmax(450.0f,700.0f);
        auto enemy1 = std::make_shared<Emeny>(ENEMY_ID::Slime,-200.0f, 85.0f, m_Map);
        auto enemy2 = std::make_shared<Emeny>(ENEMY_ID::Slime,400.0f, -300.0f, m_Map);
        auto enemy3 = std::make_shared<Emeny>(ENEMY_ID::Flymonster,400.0f, -25.0f, m_Map);
        m_GameObjects.push_back(enemy1);
        m_GameObjects.push_back(enemy2);
        m_GameObjects.push_back(enemy3);
        m_Camera.SetBounds(0.0f, 55.5f, -220.0f, 110.0f);//地圖二
    }
    if (m_CurrentLevel == 3) {
        m_Player = std::make_shared<Player>(0.0f, 0.0f, m_Map);
        m_Map->SetPlayer(m_Player);
        m_GameObjects.push_back(m_Player);
        m_Player->SetBorder(500.0f,-640.0f);
        m_Player->SetMapmax(450.0f,640.0f);
        auto enemy1 = std::make_shared<Emeny>(ENEMY_ID::Slime,400.0f, 150.0f, m_Map);
        auto enemy2 = std::make_shared<Emeny>(ENEMY_ID::DarkDevil,330.0f, -100.0f, m_Map);
        auto enemy3 = std::make_shared<Emeny>(ENEMY_ID::DarkDevil,-440.0f, 150.0f, m_Map);
        m_GameObjects.push_back(enemy1);
        m_GameObjects.push_back(enemy2);
        m_GameObjects.push_back(enemy3);
        m_Camera.SetBounds(0.0f, 0.0f, -220.0f, 110.0f);//地圖二
    }
}

void App::Cheating() {
    if (Util::Input::IsKeyDown(Util::Keycode::M)) {
        m_GameObjects.pop_back();
        m_Menu->SetScore(100);//要寫在怪物判定那裡
    }
    if (Util::Input::IsKeyDown(Util::Keycode::N)) {
        m_Menu->SetScore(100);//要寫在怪物判定那裡
    }
}


void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}

