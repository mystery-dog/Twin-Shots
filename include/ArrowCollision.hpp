#ifndef ARROW_COLLISION_HPP
#define ARROW_COLLISION_HPP

#include <memory>
#include <glm/vec2.hpp>
#include "Map.hpp"

class ArrowCollision {
public:
    static float GetGroundY(float arrowX, float arrowY, float arrowHalfHeight, std::shared_ptr<Map> map);
    static bool IsHittingWall(float frontX, float arrowY, std::shared_ptr<Map> map);
    static float CastGround(float arrowX, float startY, float endY, float halfH, std::shared_ptr<Map> map);

    // 🎯【新增】：只對箭矢設計的專用 AABB 碰撞箱檢查
    static bool IsHittingEnemy(float arrowX, float arrowY, glm::vec2 arrowSize,
                               float enemyX, float enemyY, glm::vec2 enemySize);
};

#endif