//
// Created by 吳梓暟 on 2026/5/22.
//

#include "Map.hpp"
#include "Util/Logger.hpp"    // 用於在終端機印出除錯訊息
#include "Util/Transform.hpp" // 用於設定圖片的繪製座標、縮放與旋轉
#include "Util/TransformUtils.hpp" // 【新增】引入助教寫的矩陣轉換工具
#include <fstream>            // File Stream，用於開啟與讀取外部的 txt 檔案
#include <sstream>            // String Stream，用於把讀取到的文字行切割成獨立的數字
#include <cmath> // 加入這行來使用 std::floor

#include "Player.hpp"
#include "Util/Input.hpp"

// 【建構子實作】
// 當我們在 App.cpp 中寫出 new Map() 或 make_shared<Map>() 時，這段程式碼會第一個被執行。
// 我們在這裡將傳入的 tileSize 存起來，並呼叫 InitTileImages 來準備所有圖片。
Map::Map(float tileSize) : m_TileSize(tileSize) {
    InitTileImages();
}

// 【圖片快取系統實作】
// 為了避免每畫一個方塊就去硬碟讀一次圖片（會讓遊戲超級卡），
// 我們在遊戲一開始，就把 ID.hpp 裡面的 TileID 對應到實際的圖片檔案，並暫存在記憶體中。
void Map::InitTileImages() {
    // 這裡的檔案路徑 (例如 "assets/141.jpg" 或 "assets/tile0.png")
    // 必須與你專案中實際存放圖片的路徑與副檔名完全一致。

    // 背景圖片 (對應 ID 1)
    m_TileImages[1] = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/141.jpg"); // 改用 ID 當 Key 比較快
    m_TileImages[8] = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/background/score.png");
    for (int i = 0; i < 4; i++) {
        if (i<2) {
            auto path = std::string(RESOURCE_DIR) + "/Image/background/pause" + std::to_string(i) + ".png";
            m_TileImages[i + 6] = std::make_shared<Util::Image>(path);
        }
        auto path = std::string(RESOURCE_DIR) + "/Image/background/life_" + std::to_string(i) + ".png";
        m_TileImages[i + 2] = std::make_shared<Util::Image>(path);
    }
    // 地板圖片 (對應 ID 10 ~ 17)
    for (int i = 0; i < 8; i++) {
        auto path = std::string(RESOURCE_DIR) + "/Image/background/tile" + std::to_string(i) + ".png";
        m_TileImages[i + 10] = std::make_shared<Util::Image>(path); // 改用 ID 當 Key 比較快
        m_TileImages[i + 910] = std::make_shared<Util::Image>(path);
        m_TileImages[i + 70] = std::make_shared<Util::Image>(path);
    }
    // 草地圖片 (對應 ID 30 ~ 36)
    for (int i = 0; i < 7; i++) {
        auto path = std::string(RESOURCE_DIR) + "/Image/background/grass" + std::to_string(i) + ".png";
        m_TileImages[i + 30] = std::make_shared<Util::Image>(path); // 改用 ID 當 Key 比較快
    }
    // 花圖片 (對應 ID 40 ~ 48)
    for (int i = 0; i < 9; i++) {
        auto path = std::string(RESOURCE_DIR) + "/Image/background/flower" + std::to_string(i) + ".png";
        m_TileImages[i + 40] = std::make_shared<Util::Image>(path); // 改用 ID 當 Key 比較快
    }
    // 牆壁或隱形牆 (對應 ID 910+ 與 90)
    for (int i = 0; i < 5; i++) {
        auto path = std::string(RESOURCE_DIR) + "/Image/background/pillar0_" + std::to_string(i) + ".png";
        m_TileImages[i + 200] = std::make_shared<Util::Image>(path); // 改用 ID 當 Key 比較快
        m_TileImages[i + 9200] = std::make_shared<Util::Image>(path);
        m_TileImages[i + 80] = std::make_shared<Util::Image>(path);
    }
    for (int i = 0; i < 4; i++) {
        auto path = std::string(RESOURCE_DIR) + "/Image/background/pillar1_" + std::to_string(i) + ".png";
        m_TileImages[i + 210] = std::make_shared<Util::Image>(path); // 改用 ID 當 Key 比較快
        m_TileImages[i + 9210] = std::make_shared<Util::Image>(path);
        m_TileImages[i + 85] = std::make_shared<Util::Image>(path);
    }
}

// 【讀取地圖 txt 檔案實作】
// 傳入的 filepath 會是類似 "assets/level1.txt" 的字串
void Map::LoadFromFile(const std::string& filepath) {
    // 先把舊的地圖資料清空，以防我們在遊戲中途重新載入關卡時，新舊地圖疊加在一起
    m_Grid.clear();

    // 建立一個檔案讀取流 (ifstream) 嘗試開啟檔案
    std::ifstream file(filepath);

    // 檢查檔案是否成功開啟 (如果是 false，通常代表路徑打錯，或是 txt 檔沒放在對的資料夾)
    if (!file.is_open()) {
        LOG_ERROR("無法讀取地圖檔案: " + filepath);
        return; // 開檔失敗就直接離開函式，避免後續程式崩潰
    }

    std::string line;
    // 使用 std::getline 一行一行地把 txt 檔案的內容讀取出來，存進 line 變數中
    // 迴圈會一直執行，直到檔案被讀完為止
    while (std::getline(file, line)) {

        std::vector<int> row;       // 建立一個一維陣列，用來存放「這一橫行」所有的方塊 ID
        std::stringstream ss(line); // 將剛才讀到的那一行文字(例如 "9212 0 0...") 放進字串資料流中
        int tileID;

        // ss >> tileID 的意思是：從資料流中遇到空白就切一刀，把切出來的文字轉成整數存入 tileID。
        // 只要還能切出數字，迴圈就會繼續。
        while (ss >> tileID) {
            row.push_back(tileID);  // 把這個數字塞進這一橫行的陣列尾端
        }

        // 這一橫行處理完畢後，把它塞進我們的地圖二維陣列 (m_Grid) 裡面
        m_Grid.push_back(row);
    }

    // 地圖讀取完畢後，在終端機印出一條綠色(或預設顏色)的追蹤訊息，方便確認
    LOG_TRACE("地圖載入成功！路徑: " + filepath);
}

// 【GameObject 介面要求的 Update 實作】
// 由於我們繼承了 GameObject 介面，介面規定一定要有 Update() 函式。
// 目前地圖是靜態的(不會自己亂動)，所以這裡留空即可。
void Map::Update() {
    // 留空。未來如果你的地圖會有「隨時間崩塌的地板」之類的動態機制，才寫在這裡。




    // 【新增】當按下 P 鍵時，同步切換地圖的除錯模式
    if (Util::Input::IsKeyDown(Util::Keycode::P)) {
        m_ShowDebug = !m_ShowDebug;
    }
}

// 輸入世界座標，換算出那是第幾行第幾列，並回傳 txt 裡的數字
int Map::GetTileID(float worldX, float worldY) {
    // 依據我們畫地圖的公式反推，算出二維陣列的 X 索引與 Y 索引
    int x = std::floor((worldX + 610.0f + m_TileSize / 2.0f) / m_TileSize);
    int y = std::floor((-(worldY - 400.0f) + m_TileSize / 2.0f) / m_TileSize);

    // 檢查有沒有超出陣列範圍，避免程式崩潰
    if (y >= 0 && y < m_Grid.size()) {
        int cols = m_Grid[y].size();
        if (cols > 0) {
            // ==========================================
            // 【核心修正：X 軸無縫輪迴】
            // ==========================================
            // 利用餘數運算，讓地圖的左右兩端在物理判斷上接合在一起！
            // 寫法 (x % cols + cols) % cols 是為了防止 C++ 處理負數時出錯。
            // 這樣一來：
            // 當 x 變成 24 (超出右邊) -> 會自動去查 x = 0 (左邊牆壁)
            // 當 x 變成 -1 (超出左邊) -> 會自動去查 x = 23 (右邊牆壁)
            x = (x % cols + cols) % cols;

            return m_Grid[y][x];
        }
    }
    return TileID::EMPTY; // 超出地圖範圍就當作是空氣
}

// 判斷這個數字是不是實體（能踩的）
bool Map::IsSolid(int id, bool isPlayer) {
    if (id == TileID::EMPTY) return false;      // 0 是空氣
    if (id == TileID::BACKGROUND) return false; // 1 是背景
    if (id >= TileID::LIFE_0 && id <= TileID::LIFE_3) return false; // 愛心 UI 不能踩
    if (id >= TileID::WALLGPILLAR0_0 && id <= TileID::WALLGPILLAR1_3) return false;
    if (id >= TileID::WALLGROUND_0 && id <= TileID::WALLGROUND_7) return false;
    if (id >= TileID::GRASS_0 && id <= TileID::FLOWER_8) return false;//草+花
    if (id <= TileID::LAFTJUMP) return false;
    if (id == TileID::AIRWALL) {
        // 如果是空氣牆 (90)：玩家可以穿過 (回傳 false)，怪物會被擋住 (回傳 true)
        return !isPlayer;
    }

    // 其餘的 (包含 10~17 地板, 200+ 柱子) 全都是可以踩的實體！
    return true;
}

bool Map::IsWall(int id, bool isPlayer) {
    // 根據你在 ID.hpp 裡定義的「牆壁 開頭9」來判斷
    if (id == TileID::AIRWALL) {
        // 如果是空氣牆 (90)：玩家可以穿過 (回傳 false)，怪物會被擋住 (回傳 true)
        return !isPlayer;
    }

    // 910 ~ 917 的牆壁版地板
    if (id >= TileID::WALLGROUND_0 && id <= TileID::WALLGROUND_7) return true;

    // 9200 ~ 9213 的牆壁版柱子
    if (id >= TileID::WALLGPILLAR0_0 && id <= TileID::WALLGPILLAR1_3) return true;

    if (id >= TileID::BOTHGROUND_0 && id <= TileID::BOTHPILLAR1_3) return true;

    // 其他的 (包含一般的 10~17 和 200+) 一律不阻擋水平移動！
    return false;
}

// 【GameObject 介面要求的 Draw 實作】
// 這個函式每一幀 (Frame) 都會被主程式呼叫一次，負責把整張地圖畫到螢幕上。
void Map::Draw(float cameraX, float cameraY) {

    // =================================================================
    // 【核心修正：優先繪製全景背景】
    // =================================================================
    // 為什麼要寫在最前面？因為遊戲繪製有「前後順序」（塗鴉者演算法）。
    // 先畫的會被後畫的蓋過去。背景必須在雙層迴圈「之前」先畫，才不會把地板蓋住！
    if (m_TileImages.count(TileID::BACKGROUND) > 0) {

        Util::Transform bgTransform;
        bgTransform.rotation = 0.0f;   // 背景不旋轉
        bgTransform.scale = {2.35f, 1.8f}; // 背景維持 1:1 原始比例

        // 設定背景圖片的中心點座標。
        // 【注意】如果執行後發現背景位置歪掉，可以微調這裡的 translation 座標。
        // 假設助教的引擎是以畫面正中心為 (0,0)，那設定成 {0.0f, 0.0f} 就能讓背景置中。
        bgTransform.translation = {0.0f, 0.0f};

        // 取得背景圖片的原始寬高
        glm::vec2 bgSize = m_TileImages[TileID::BACKGROUND]->GetSize();

        // 呼叫助教的工具轉換成底層矩陣
        // 這裡的 Z-Index 我們設定為 -1.0f（比地圖的 0.0f 還要低），強迫它留在最底層
        auto bgMatrices = Util::ConvertToUniformBufferData(bgTransform, bgSize, -1.0f);

        // 正式繪製背景圖
        m_TileImages[TileID::BACKGROUND]->Draw(bgMatrices);

        //生命顯示
        Util::Transform m_Life;
        m_Life.translation = {530.0f, -275.0f};
        m_Life.scale = {13.5f, 8.0f};

        auto Life = Util::ConvertToUniformBufferData(m_Life, m_Life.scale, 1.0f);

        if (m_Player->GetLife() == 3) m_TileImages[TileID::LIFE_3]->Draw(Life);
        else if (m_Player->GetLife() == 2) m_TileImages[TileID::LIFE_2]->Draw(Life);
        else if (m_Player->GetLife() == 1) m_TileImages[TileID::LIFE_1]->Draw(Life);
        else m_TileImages[TileID::LIFE_0]->Draw(Life);

        //分數顯示
        Util::Transform m_Score;
        m_Score.translation = {512.5f, -332.5f};
        m_Score.scale = {15.0f, 7.0f};
        auto Score = Util::ConvertToUniformBufferData(m_Score, m_Score.scale, 1.0f);

        m_TileImages[TileID::SCORE]->Draw(Score);

        //暫停顯示
        m_Pause.translation = {600.0f, 335.0f};
        m_Pause.scale = {6.0f, 6.0f};
    }


    // =================================================================
    // 【原本的地圖方塊雙層迴圈】
    // =================================================================
    // 背景畫完後，接著在上面重疊畫出 txt 地圖裡的磚塊、草地
    Util::Transform transform;
    transform.rotation = 0.0f;
    transform.scale = {1.0f, 1.0f};

    // 外層迴圈控制 Y 軸 (行數)
    for (size_t y = 0; y < m_Grid.size(); ++y) {
        // 內層迴圈控制 X 軸 (列數)
        for (size_t x = 0; x < m_Grid[y].size(); ++x) {

            // 拿出目前這一格的方塊 ID
            int id = m_Grid[y][x];

            // 如果是空地 (0) 或隱形空氣牆 (90)，就跳過不畫，直接處理下一格
            if (id == TileID::EMPTY ) {
                continue;
            }

            // 檢查這個方塊 ID 是否有對應的圖片被載入
            if (m_TileImages.count(id) > 0) {
                float worldX = (x * m_TileSize) - 610.0f;
                float worldY = -(y * m_TileSize) + 400.0f;

                float screenX = worldX - cameraX;
                float screenY = worldY - cameraY;

                transform.translation = {screenX, screenY};

                // 1. 取得目前這張方塊圖片的原始尺寸 (例如 32x32)
                glm::vec2 size = m_TileImages[id]->GetSize();

                // 2. 【新增這裡】動態計算縮放比例！
                // 邏輯：目標大小 (64) 除以 原始大小 (32) = 放大 2 倍
                // 這樣就算你的圖片是 16x16，它也會自動算出要放大 4 倍
                transform.scale = { m_TileSize / size.x, m_TileSize / size.y };

                // 3. 把算好 scale 的 transform 傳進去轉換
                Core::Matrices matricesData = Util::ConvertToUniformBufferData(transform, size, 0.0f);
                m_TileImages[id]->Draw(matricesData);
            }
        }
    }
}

bool Map::GetPauseState() {
    if (Util::Input::IsKeyDown(Util::Keycode::P)) {
        m_PausePressed = true;
    }
    // 1. 取得滑鼠位置
    glm::vec2 mousePos = Util::Input::GetCursorPosition();

    // 2. 取得按鈕尺寸
    glm::vec2 btnSize = m_TileImages[TileID::PAUSE_0]->GetSize();
    float halfW = btnSize.x / 2.0f;
    float halfH = btnSize.y / 2.0f;

    // 3. 判定滑鼠是否在按鈕矩形範圍內
    if (mousePos.x > m_Pause.translation.x - halfW &&
        mousePos.x < m_Pause.translation.x + halfW &&
        mousePos.y > m_Pause.translation.y - halfH &&
        mousePos.y < m_Pause.translation.y + halfH) {

        m_IsPauseHovering = true;

        // 4. 如果懸停且按下左鍵
        if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {
            m_PausePressed = true;
        }

        } else{
            m_IsPauseHovering = false;
        }

    auto Pause = Util::ConvertToUniformBufferData(m_Pause, m_Pause.scale, 1.0f);
    if (m_IsPauseHovering) m_TileImages[TileID::PAUSE_1]->Draw(Pause);
    else m_TileImages[TileID::PAUSE_0]->Draw(Pause);

    return m_PausePressed;
}
