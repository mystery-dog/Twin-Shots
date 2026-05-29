//
// Created by 吳梓暟 on 2026/5/22.
//

#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

// GameObject 是一個「抽象類別 (Abstract Class)」，作為所有遊戲物件的「介面」
class GameObject {
public:
    // 【繼承技巧】虛擬解構子 (Virtual Destructor)
    // 只要你的類別有虛擬函式，就一定要加上虛擬解構子。
    // 這樣未來如果用 GameObject 的指標去 delete 子類別 (如 Map) 時，才能正確釋放子類別的記憶體，避免 Memory Leak。
    virtual ~GameObject() = default;

    // 【介面技巧】純虛擬函式 (Pure Virtual Function)
    // 後面的 "= 0" 代表這是一個「沒有實作內容」的介面。
    // 任何繼承 GameObject 的子類別 (例如 Map, Character)，都「強制規定」必須實作這兩個函式。

    // 負責處理邏輯 (例如移動、計時)
    virtual void Update() = 0;

    // 負責處理畫面渲染
    // 【修改這裡】加上 cameraX 和 cameraY
    // 這樣所有的遊戲物件在被畫出來的時候，都能自動扣掉相機的偏移量
    virtual void Draw(float cameraX, float cameraY) = 0;
};

#endif