//
// Created by 吳梓暟 on 2026/5/27.
//

#ifndef ENEMY_ID_HPP
#define ENEMY_ID_HPP
namespace ENEMY_ID {
    // 使用 enum class 可以避免名稱衝突，且強制類型安全
    enum Type {
        Slime = 0,
        Flymonster = 1,
        DarkDevil = 2,
    };
}

#endif //PTSD_PROJECT_ENEMY_ID_HPP