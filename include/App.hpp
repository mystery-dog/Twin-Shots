#ifndef APP_HPP
#define APP_HPP


#include "pch.hpp" // IWYU pragma: export
#include "Util/AssetStore.hpp"
#include "Util/Image.hpp"
#include "Util/TransformUtils.hpp"
#include "Core/Context.hpp"
#include "map"
#include <optional>
// ==================== 【大改動：引入具體的地圖類別】 ====================
#include "Map.hpp" // 引入地圖的具體功能，這樣我們才能在 Start() 裡面實例化(new)它
#include "Player.hpp"// 引入角色
#include "Camera.hpp"    // 引入相機
#include "Arrow.hpp" // 【新增】引入剛剛寫的測試箭

// ==================== 【大改動：引入物件導向介面與容器】 ====================
#include "GameObject.hpp" // 引入遊戲物件的抽象基底類別(介面)
#include <vector>         // 引入 C++ 動態陣列，用來儲存多個遊戲物件
#include <memory>         // 引入智慧型指標工具 (std::shared_ptr)

#include "Menu.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Level();

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    State m_CurrentState = State::START;

    // ==================== 【大改動：宣告全域物件管理清單】 ====================
    // 建立一個裝有 GameObject 智慧型指標的動態陣列 (vector)。
    // 透過物件導向「多型 (Polymorphism)」的特性，這個清單未來可以同時裝入：
    // 地圖、角色、甚至是你之後要寫的弓箭子彈與怪物。
    // App 主程式只要管理這個清單，就能一網打盡更新與渲染，不需要幫每個物件單獨寫變數！
    std::vector<std::shared_ptr<GameObject>> m_GameObjects;

    // 【新增】單獨記錄相機與玩家
    // 我們需要 m_Player 指標，才能從它身上取得 GetX() 和 GetY() 給相機
    std::shared_ptr<Player> m_Player;
    Camera m_Camera;

    std::shared_ptr<Menu> m_Menu;

    int m_CurrentLevel = 1;
};

#endif
