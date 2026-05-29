//
// Created by 吳梓暟 on 2026/5/22.
//

#ifndef MAP_HPP
#define MAP_HPP

#include "pch.hpp"
#include "Util/Image.hpp"
#include "ID.hpp"
#include "GameObject.hpp"    // 【新增】引入介面
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

class Player;

// 【繼承技巧】使用 : public GameObject 來繼承介面
class Map : public GameObject {
public:
    Map(float tileSize = 55.5f);

    void LoadFromFile(const std::string& filepath);

    // 【多型技巧】使用 override 關鍵字
    // 告訴編譯器：「我正在覆寫 (Override) 父類別 (GameObject) 的虛擬函式」。
    // 這樣如果父類別的函式名稱改了，或是你打錯字，編譯器會馬上報錯提醒你。
    void Update() override;
    void Draw(float cameraX, float cameraY) override;

    int GetTileID(float worldX, float worldY); // 輸入世界座標，回傳該位置的方塊 ID
    bool IsSolid(int tileID, bool isPlayer);                  // 判斷這個 ID 是不是「可以踩的實體」
    float GetTileSize() const { return m_TileSize; } // 取得方塊大小

    // 【新增】判斷這個 ID 是不是「會阻擋左右移動的牆壁」
    bool IsWall(int tileID, bool isPlayer = false);

    void SetPlayer(std::shared_ptr<Player> player) { m_Player = player; }

    bool GetPauseState()
    ;// 【新增】提供一個方法讓外部強制洗掉暫停狀態
    void SetPausePressed() { m_PausePressed = false; }

private:
    void InitTileImages();

    std::vector<std::vector<int>> m_Grid;
    float m_TileSize;
    std::unordered_map<int, std::shared_ptr<Util::Image>> m_TileImages;
    std::shared_ptr<Player> m_Player; // 記錄玩家是誰

    // 【新增】地圖的除錯模式開關，預設為 false
    bool m_ShowDebug = false;

    Util::Transform m_Pause;
    bool m_PausePressed = false;
    bool m_IsPauseHovering = false;
};

#endif