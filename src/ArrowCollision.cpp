#include "ArrowCollision.hpp"
#include <cmath>
#include <algorithm>

float ArrowCollision::GetGroundY(float arrowX, float arrowY, float arrowHalfHeight, std::shared_ptr<Map> map) {
    float checkY = arrowY - arrowHalfHeight;
    int tileID = map->GetTileID(arrowX, checkY);

    if (map->IsWall(tileID, false)) {
        float TILE_SIZE = map->GetTileSize();
        float tileRow = std::floor(checkY / TILE_SIZE);
        return (tileRow + 1.0f) * TILE_SIZE + arrowHalfHeight;
    }
    return -999.0f;
}

bool ArrowCollision::IsHittingWall(float frontX, float arrowY, std::shared_ptr<Map> map) {
    int tileID = map->GetTileID(frontX, arrowY);
    return map->IsWall(tileID, false);
}

// 🎯【核心升級】：步進式縱向掃描，專治高速箭穿地
float ArrowCollision::CastGround(float arrowX, float startY, float endY, float halfH, std::shared_ptr<Map> map) {
    if (!map) return -999.0f;
    if (endY >= startY) return -999.0f;

    float TILE_SIZE = map->GetTileSize();
    float startBottomY = startY - halfH;
    float endBottomY = endY - halfH;

    // 🔧 步長改為 0.15 倍地磚高，杜絕高速穿透
    float step = TILE_SIZE * 0.15f;
    float currentBottomY = startBottomY;

    while (currentBottomY >= endBottomY) {
        int tileID = map->GetTileID(arrowX, currentBottomY);

        if (map->IsWall(tileID, true)) {   // 需確保地板 tile 的 IsWall(..., true) 回傳 true
            float tileRow = std::floor(currentBottomY / TILE_SIZE);
            float tileTopY = (tileRow + 1.0f) * TILE_SIZE;
            return tileTopY + halfH;
        }

        if (currentBottomY == endBottomY) break;
        currentBottomY = std::max(endBottomY, currentBottomY - step);
    }
    return -999.0f;
}

bool ArrowCollision::IsHittingEnemy(float arrowX, float arrowY, glm::vec2 arrowSize,
                                   float enemyX, float enemyY, glm::vec2 enemySize) {
    float deltaX = std::abs(arrowX - enemyX);
    float deltaY = std::abs(arrowY - enemyY);

    float arrowHalfW = arrowSize.x * 0.5f;
    float arrowHalfH = arrowSize.y * 0.5f;
    float enemyHalfW = enemySize.x * 0.5f;
    float enemyHalfH = enemySize.y * 0.5f;

    return (deltaX < (arrowHalfW + enemyHalfW) && deltaY < (arrowHalfH + enemyHalfH));
}