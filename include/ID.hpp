#ifndef ID_HPP
#define ID_HPP

namespace TileID {
    // 使用 enum class 可以避免名稱衝突，且強制類型安全
    enum Type {
        EMPTY = 0,//空

        //背景 開頭0
        BACKGROUND = 1,     //對應141.jpg
        LIFE_0  =   2,      //對應life_0
        LIFE_1  =   3,      //對應life_1
        LIFE_2  =   4,      //對應life_2
        LIFE_3  =   5,      //對應life_3

        //地板 開頭1
        GROUND_0 = 10,      // 對應 tile0
        GROUND_1 = 11,      // 對應 tile1
        GROUND_2 = 12,      // 對應 tile2
        GROUND_3 = 13,      // 對應 tile3
        GROUND_4 = 14,      // 對應 tile4
        GROUND_5 = 15,      // 對應 tile5
        GROUND_6 = 16,      // 對應 tile6
        GROUND_7 = 17,      // 對應 tile7

        //柱子 開頭2
        PILLAR0_0 = 200,    // 對應 pillar0_0
        PILLAR0_1 = 201,    // 對應 pillar0_1
        PILLAR0_2 = 202,    // 對應 pillar0_2
        PILLAR0_3 = 203,    // 對應 pillar0_3
        PILLAR0_4 = 204,    // 對應 pillar0_4
        PILLAR1_0 = 210,    // 對應 pillar1_0
        PILLAR1_1 = 211,    // 對應 pillar1_1
        PILLAR1_2 = 212,    // 對應 pillar1_2
        PILLAR1_3 = 213,    // 對應 pillar1_3

        //牆壁 開頭9
        AIRWALL = 90,
        //地板
        WALLGROUND_0 = 910,      // 對應 tile0
        WALLGROUND_1 = 911,      // 對應 tile1
        WALLGROUND_2 = 912,      // 對應 tile2
        WALLGROUND_3 = 913,      // 對應 tile3
        WALLGROUND_4 = 914,      // 對應 tile4
        WALLGROUND_5 = 915,      // 對應 tile5
        WALLGROUND_6 = 916,      // 對應 tile6
        WALLGROUND_7 = 917,      // 對應 tile7
        //柱子
        WALLGPILLAR0_0 = 9200,    // 對應 pillar0_0
        WALLGPILLAR0_1 = 9201,    // 對應 pillar0_1
        WALLGPILLAR0_2 = 9202,    // 對應 pillar0_2
        WALLGPILLAR0_3 = 9203,    // 對應 pillar0_3
        WALLGPILLAR0_4 = 9204,    // 對應 pillar0_4
        WALLGPILLAR1_0 = 9210,    // 對應 pillar1_0
        WALLGPILLAR1_1 = 9211,    // 對應 pillar1_1
        WALLGPILLAR1_2 = 9212,    // 對應 pillar1_2
        WALLGPILLAR1_3 = 9213,    // 對應 pillar1_3

        //草 開頭3
        GRASS_0 = 30, // 對應 grass0
        GRASS_1 = 31, // 對應 grass1
        GRASS_2 = 32, // 對應 grass2
        GRASS_3 = 33, // 對應 grass3
        GRASS_4 = 34, // 對應 grass4
        GRASS_5 = 35, // 對應 grass5
        GRASS_6 = 36, // 對應 grass6

        //同時是牆也是地板 開頭7~8
        BOTHGROUND_0 = 70,      // 對應 tile0
        BOTHGROUND_1 = 71,      // 對應 tile1
        BOTHGROUND_2 = 72,      // 對應 tile2
        BOTHGROUND_3 = 73,      // 對應 tile3
        BOTHGROUND_4 = 74,      // 對應 tile4
        BOTHGROUND_5 = 75,      // 對應 tile5
        BOTHGROUND_6 = 76,      // 對應 tile6
        BOTHGROUND_7 = 77,      // 對應 tile7
        BOTHPILLAR0_0 = 80,    // 對應 pillar0_0
        BOTHPILLAR0_1 = 81,    // 對應 pillar0_1
        BOTHPILLAR0_2 = 82,    // 對應 pillar0_2
        BOTHPILLAR0_3 = 83,    // 對應 pillar0_3
        BOTHPILLAR0_4 = 84,    // 對應 pillar0_4
        BOTHPILLAR1_0 = 85,    // 對應 pillar1_0
        BOTHPILLAR1_1 = 86,    // 對應 pillar1_1
        BOTHPILLAR1_2 = 87,    // 對應 pillar1_2
        BOTHPILLAR1_3 = 88,    // 對應 pillar1_3

    };
}

#endif