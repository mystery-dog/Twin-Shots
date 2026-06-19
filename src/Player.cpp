//
// Created by 吳梓暟 on 2026/5/22.
//

#include "Player.hpp"
#include "Util/Input.hpp"         // 負責偵測鍵盤按鍵
#include "Util/Keycode.hpp"       // 定義了 UP, DOWN, LEFT, RIGHT 等按鍵代碼

// 【建構子】
// 當角色被創造出來時，設定它的初始座標，並載入圖片
Player::Player(float startX, float startY, std::shared_ptr<Map> map)
    : Character(startX, startY, map) {
    // 【新增】設定自己的身分是玩家！
    m_Speed = 9.0f;
    m_IsPlayer = true;
    m_JumpForce = 13.5f;
    for (int i = 0; i < 8; i++) {
        std::string pathL = RESOURCE_DIR"/Image/character/run/run_l" + std::to_string(i) + ".png";
        m_WalkLeftImages.push_back(std::make_shared<Util::Image>(pathL));

        std::string pathR = RESOURCE_DIR"/Image/character/run/run_r" + std::to_string(i) + ".png";
        m_WalkRightImages.push_back(std::make_shared<Util::Image>(pathR));

        std::string standL = RESOURCE_DIR"/Image/character/stand/player_l" + std::to_string(i) + ".png";
        m_StandLeftImage.push_back(std::make_shared<Util::Image>(standL));
        std::string standR = RESOURCE_DIR"/Image/character/stand/player_r" + std::to_string(i) + ".png";
        m_StandRightImage.push_back(std::make_shared<Util::Image>(standR));
    }
    // 🎯【新增】載入射擊動畫圖片 (假設檔名是 shoot_l0.png ~ shoot_l3.png)
    int totalShootFrames = 8; // 💡 依你實際的圖片張數調整
    for (int i = 0; i < totalShootFrames; i++) {
        std::string shootL = RESOURCE_DIR"/Image/character/shoot/shot_l" + std::to_string(i) + ".png";
        m_ShootLeftImages.push_back(std::make_shared<Util::Image>(shootL));

        std::string shootR = RESOURCE_DIR"/Image/character/shoot/shot_r" + std::to_string(i) + ".png";
        m_ShootRightImages.push_back(std::make_shared<Util::Image>(shootR));
    }
    int totalJumpFrames = 2; // 依你實際的圖片張數調整，只有 1 張也可以
    for (int i = 0; i < totalJumpFrames; i++) {
        m_JumpLeftImages.push_back(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/character/jump/up_l" + std::to_string(i) + ".png"));
        m_JumpRightImages.push_back(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/character/jump/up_r" + std::to_string(i) + ".png"));
    }

    int totalFallFrames = 2;
    for (int i = 0; i < totalFallFrames; i++) {
        m_FallLeftImages.push_back(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/character/down/down_l" + std::to_string(i) + ".png"));
        m_FallRightImages.push_back(std::make_shared<Util::Image>(RESOURCE_DIR"/Image/character/down/down_r" + std::to_string(i) + ".png"));
    }
    m_HurtLeftImage = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/character/hit/player_dead_l.png");
    m_HurtRightImage = std::make_shared<Util::Image>(RESOURCE_DIR"/Image/character/hit/player_dead_r.png");
    m_Image = m_StandLeftImage[0];// 我們只負責幫自己載入這張專屬的「小惡魔圖片」
}

void Player::SetBorder(float Height, float Width) {
    m_BorderHeight = Height;
    m_BorderWidth = Width;
}


// 【邏輯更新】每幀都會被執行一次
void Player::Update() {

    //<Ying>======================================
    // 【新增】先記錄上一幀有沒有在動，用來判斷「剛開始起步」的瞬間
    bool wasMoving = m_IsMoving;
    // m_IsMoving = false; // 先假設這幀沒動

    bool leftPressed = Util::Input::IsKeyPressed(Util::Keycode::LEFT);
    bool rightPressed = Util::Input::IsKeyPressed(Util::Keycode::RIGHT);

    // 💡【進階體驗優化】：如果你希望「射箭時角色必須停在原地」，可以解開下面這兩行
    // if (m_IsShooting) { leftPressed = false; rightPressed = false; }

    // 處理向左加速
    if (leftPressed && !rightPressed) {
        m_VelocityX -= m_Acceleration;     // 速度往負方向累加
        if (m_VelocityX < -m_Speed) {      // 限制不能超過最高速
            m_VelocityX = -m_Speed;
        }
        m_FacingRight = false;
    }
    // 處理向右加速
    else if (rightPressed && !leftPressed) {
        m_VelocityX += m_Acceleration;     // 速度往正方向累加
        if (m_VelocityX > m_Speed) {       // 限制不能超過最高速
            m_VelocityX = m_Speed;
        }
        m_FacingRight = true;
    }
    // 🎯【核心修改 3】：如果左右都沒按（或是同時按），就套用摩擦力讓角色慢慢滑行停下來
    else {
        m_VelocityX *= m_Friction;         // 摩擦力減速
        if (std::abs(m_VelocityX) < 0.1f) { // 速度太慢時直接歸零，防止無限微動
            m_VelocityX = 0.0f;
        }
    }
    m_IsMoving = (std::abs(m_VelocityX) > 0.2f);
    //</Ying>======================================

    // if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
    //     m_VelocityX -= m_Acceleration;     // 速度往負方向累加
    //     if (m_VelocityX < -m_Speed) {      // 限制不能超過最高速
    //         m_VelocityX = -m_Speed;
    //     }
    //     m_FacingRight = false;
    //     m_FacingRight = false;
    //     // m_IsMoving = true;
    // }
    // if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
    //     m_VelocityX = m_Speed;
    //     m_FacingRight = true;
    //     m_IsMoving = true;
    // }
    if (Util::Input::IsKeyDown(Util::Keycode::UP) && (m_IsGrounded || m_IsOnArrow)) {
        m_VelocityY = m_JumpForce;
        m_IsGrounded = false; // 跳起來的瞬間，就不是踩在地面上了
        m_IsOnArrow = false;  // 跳起來的瞬間，也離開了箭
    }



    // Arrow【新增】按空白鍵射箭
    if (Util::Input::IsKeyDown(Util::Keycode::SPACE)&& !m_IsShooting) {
        // 在玩家目前的座標產生一支新箭，並傳入玩家目前的朝向 m_FacingRight
        m_IsShooting = true;
        m_HasSpawnedArrow = false; // 重置發射標記
        m_AnimTimer = 0.0f;        // 動畫計時歸零
        m_CurrentFrame = 0;        // 強制從第 0 格動畫開始播

    }



    // 2. 處理上下左右無縫聯通 (Screen Wrapping)
    // 概念：一旦座標超出了地圖邊界，就強制把座標修改到對面的邊界。

    // 超出左邊界 -> 從右邊出來
    if (m_X < m_BorderWidth) {
        m_X = m_MapWidth;
    }
    // 超出右邊界 -> 從左邊出來
    else if (m_X > m_MapWidth) {
        m_X = m_BorderWidth;
    }

    // 超出上邊界 -> 從下面出來 (我們設定 Y 往下是負數，所以最下面是 -m_MapHeight)
    if (m_Y > m_BorderHeight) {//地圖一
        //地圖二 m_Y > 550.0f
        m_Y = -m_MapHeight;
    }
    // 超出下邊界 -> 從上面出來
    else if (m_Y < -m_MapHeight) {
        m_Y = m_BorderHeight;//地圖一
        //地圖二 m_Y = 550.0f
    }
    // <Ying>====== 【新增：無敵時間與受傷閃爍邏輯】 ======
    if (m_IsInvincible) {
        // 1. 減少無敵時間 (60FPS 下固定用 0.016f，或改用 DeltaTime)
        m_InvincibleTimer -= 0.016f;

        if (m_InvincibleTimer <= 0.0f) {
            m_IsInvincible = false;  // 無敵時間結束
            m_IsVisible = true;      // 確保恢復可見
        }
        else {
            // 2. 無敵期間內的閃爍邏輯
            m_FlashTimer += 0.016f;
            if (m_FlashTimer >= m_FlashInterval) {
                m_FlashTimer = 0.0f;
                m_IsVisible = !m_IsVisible; // 反轉可見狀態 (true 變 false，false 變 true)
            }
        }
    } else {
        m_IsVisible = true; // 沒有無敵時，一定要看得到
    }
    //<--動畫-->
    m_AnimTimer += 0.016f;
    float currentDuration = m_AnimFrameDuration;

    bool isInAir = (!m_IsGrounded && !m_IsOnArrow);

    // 🎯【修改】優先採用射擊動畫的時間長度
    if (m_IsShooting) {
        currentDuration = m_ShootFrameDuration;
    } else if (isInAir) {
        currentDuration = m_JumpFrameDuration;
    } else {
        currentDuration = m_IsMoving ? m_WalkFrameDuration : m_StandFrameDuration;
    }

    // 如果這幀「剛離開地面（跳起）」或「移動/靜止狀態改變」，就重置動畫影格
    wasGroundedOrOnArrow = (m_IsGrounded || m_IsOnArrow);
    if ((wasGroundedOrOnArrow && isInAir) || (m_IsMoving != wasMoving)) {
        // 🎯【修改】如果正在射擊，就不打斷射擊影格的計算
        if (!m_IsShooting) {
            m_AnimTimer = 0.0f;
            m_CurrentFrame = 0;
        }
    }
    // 🎯【補上漏掉的關鍵核心 1】：當計時器超過當前狀態所需的時間，就切換到下一張圖！
    if (m_AnimTimer >= currentDuration) {
        m_AnimTimer = 0.0f;
        m_CurrentFrame++; // 影格遞增，動畫才會真正跑起來！
        // 🎯【新增】如果正在射擊，且影格播完了射擊陣列的長度，就關閉射擊狀態
        if (m_IsShooting && !m_ShootLeftImages.empty() && m_CurrentFrame >= m_ShootLeftImages.size()) {
            auto newArrow = std::make_shared<Arrow>(m_X, m_Y, m_FacingRight, m_Map);
            m_PlayerArrows.push_back(newArrow);
            m_IsShooting = false;
            m_CurrentFrame = 0; // 回歸正常影格
        }
    }
    // 🎯【新增】如果正在射擊，且影格播完了射擊陣列的長度，就關閉射擊狀態
    if (m_IsShooting && !m_ShootLeftImages.empty() && m_CurrentFrame >= m_ShootLeftImages.size()) {
        m_IsShooting = false;
        m_CurrentFrame = 0; // 回歸正常影格
    }
    // 🎯【補上漏掉的關鍵核心 2】：在判定結束後，把這一幀的地面狀態存進去，供下一幀做 was 比較
    if (m_IsInvincible) {
        // 【最高優先級：受傷狀態】
        // 只要還在無敵時間內，不管是在走路還是跳躍，都強制顯示受傷圖片
        m_Image = m_FacingRight ? m_HurtRightImage : m_HurtLeftImage;
    }else if (m_IsShooting) {
        // 🎯【新增優先級：射擊狀態圖片切換】
        if (!m_ShootLeftImages.empty()) {
            m_CurrentFrame = m_CurrentFrame % m_ShootLeftImages.size();
            m_Image = m_FacingRight ? m_ShootRightImages[m_CurrentFrame] : m_ShootLeftImages[m_CurrentFrame];
        }
    }else if (isInAir) {
        // 【狀態 A：在空中】
        if (m_VelocityY > 0.0f) {
            // 1. 上升中 -> 播跳躍動畫
            if (!m_JumpLeftImages.empty()) {
                m_CurrentFrame = m_CurrentFrame % m_JumpLeftImages.size();
                m_Image = m_FacingRight ? m_JumpRightImages[m_CurrentFrame] : m_JumpLeftImages[m_CurrentFrame];
            }
        } else {
            // 2. 下降中 -> 播下落動畫
            if (!m_FallLeftImages.empty()) {
                m_CurrentFrame = m_CurrentFrame % m_FallLeftImages.size();
                m_Image = m_FacingRight ? m_FallRightImages[m_CurrentFrame] : m_FallLeftImages[m_CurrentFrame];
            }
        }
    }
    else {
        // 【狀態 B：在地面或踩在箭上】
        if (m_IsMoving) {
            // 3. 走路中
            if (!m_WalkLeftImages.empty()) {
                m_CurrentFrame = m_CurrentFrame % m_WalkLeftImages.size();
                m_Image = m_FacingRight ? m_WalkRightImages[m_CurrentFrame] : m_WalkLeftImages[m_CurrentFrame];
            }
        } else {
            // 4. 站立中
            if (!m_StandLeftImage.empty()) {
                m_CurrentFrame = m_CurrentFrame % m_StandLeftImage.size();
                m_Image = m_FacingRight ? m_StandRightImage[m_CurrentFrame] : m_StandLeftImage[m_CurrentFrame];
            }
        }
    }
    //</Ying> ==============================================

    Character::Update();


    //<Ying>===========================================
    // 5. 【新增】更新箭矢位置 & 踩箭的物理判定
    m_IsOnArrow = false; // 每幀開始前重設

    // 使用傳統迭代器迴圈，方便在箭飛太遠時將它從陣列中刪除
    for (auto it = m_PlayerArrows.begin(); it != m_PlayerArrows.end(); ) {
        (*it)->Update(); // 讓箭往前飛

        // 如果箭已經飛太遠死掉了，就從清單中移除
        if ((*it)->IsDead()) {
            it = m_PlayerArrows.erase(it);
            continue;
        }
        // 只有「還在飛」的箭才能傷怪，插在牆上的不行（原版 Twin-Shots 設定）
        if (!(*it)->IsStuck() && m_Map) {
            bool arrowHitSomething = false;

            float arrowX = (*it)->GetX();
            float arrowY = (*it)->GetY();
            float arrowHalfWidth = (*it)->GetSize().x * 0.5f;  // 寬度半徑
            float arrowHalfHeight = (*it)->GetSize().y * 0.5f; // 高度半徑

            // 遍歷地圖上所有的怪物 (這裡沿用之前假設的 m_Map->GetEnemies() 函數)
            for (auto& enemy : m_Map->GetEnemies()) {
                // 如果怪已經死了，就跳過
                if (enemy->GetLife() <= 0) continue;

                float enemyX = enemy->GetX();
                float enemyY = enemy->GetY();
                float enemyHalfWidth = enemy->GetImageSize().x * 0.5f;
                float enemyHalfHeight = enemy->GetImageSize().y * 0.5f;

                // AABB 矩形碰撞公式
                bool isOverlappingX = (arrowX + arrowHalfWidth >= enemyX - enemyHalfWidth) &&
                                      (arrowX - arrowHalfWidth <= enemyX + enemyHalfWidth);

                bool isOverlappingY = (arrowY + arrowHalfHeight >= enemyY - enemyHalfHeight) &&
                                      (arrowY - arrowHalfHeight <= enemyY + enemyHalfHeight);

                // 如果撞到了！
                if (isOverlappingX && isOverlappingY) {
                    // 1. 讓怪物死掉（這裡直接扣除牠所有的血，或是呼叫你怪物的扣血函數）
                    // 假設你的怪物有 m_life，可以提供一個 Hurt() 或直接扣血
                    enemy->SetLifeToZero(); // ⚠️ 或者是 enemy->Hurt(1);

                    // 2. 標記這支箭已經打到東西，該消失了
                    arrowHitSomething = true;
                    break;
                }
            }

            // 如果箭打到怪了，就把這支箭從清單中刪除
            if (arrowHitSomething) {
                it = m_PlayerArrows.erase(it);
                continue; // 繼續處理下一支箭
            }
        }
        // 踩箭碰撞判定：如果玩家在空中、且正在往下掉，就檢查有沒有踩到這支箭
        if (!m_IsGrounded && m_VelocityY <= 0.0f) {

            // 【修改】多加上 && (*it)->IsStuck() 條件
            // 只有當箭矢插在牆上靜止時，玩家才能踩它！
            if ((*it)->IsStuck()) {
                float playerHalfWidth = m_Image->GetSize().x * 0.5f;
                float playerHalfHeight = m_Image->GetSize().y;
                float playerFeetY = m_Y - playerHalfHeight;

                float arrowX = (*it)->GetX();
                float arrowY = (*it)->GetY();
                float arrowHalfWidth = (*it)->GetSize().x * 0.5f; // 如果 scale 改成 2.0，半寬就是 size * 0.5

                if (playerFeetY >= arrowY - 5.0f && playerFeetY <= arrowY + 15.0f) {
                    if (m_X + playerHalfWidth >= arrowX - arrowHalfWidth &&
                        m_X - playerHalfWidth <= arrowX + arrowHalfWidth) {

                        m_Y = arrowY + playerHalfHeight;
                        m_VelocityY = 0.0f;
                        m_IsOnArrow = true;
                        }
                }
            }
        }
        it++;
    }
    //</Ying>========================================


    //扣血測試
    if (Util::Input::IsKeyDown(Util::Keycode::O)) {
        m_life--;
    }

    // 【新增】除錯開關切換
    // 注意：這裡必須用 IsKeyDown (按下的瞬間觸發一次)，
    // 不能用 IsKeyPressed (按住會一直觸發，導致開關瘋狂閃爍)
    if (Util::Input::IsKeyDown(Util::Keycode::P)) {
        m_ShowDebug = !m_ShowDebug; // 把開關的狀態反轉 (true 變 false, false 變 true)
    }
}

void Player::Draw(float cameraX, float cameraY) {
    // ====== 【新增：如果這幀閃爍到隱形狀態，就不畫角色本體與分身】 ======
    if (!m_IsVisible) {
        // 記得保留箭矢的繪製
        for (auto& arrow : m_PlayerArrows) {
            arrow->Draw(cameraX, cameraY);
        }
        return;
    }
    // ==============================================================
    // 1. 呼叫老爸 (Character) 的 Draw，讓他幫我們畫出「本體」和「除錯紅點探針」！
    Character::Draw(cameraX, cameraY);

    // 2. 玩家專屬的「無縫穿牆視覺分身」邏輯
    if (!m_Image) return;

    Util::Transform transform;
    transform.rotation = 0.0f;
    transform.scale = {2.0f, 2.0f};
    glm::vec2 size = m_Image->GetSize();

    float screenX = m_X - cameraX;
    float screenY = m_Y - cameraY;

    // 計算地圖的總寬度 (右邊界 - 左邊界)
    // 這裡我們用你之前設定的 m_BorderWidth 和 m_MapWidth
    float leftEdge = m_BorderWidth;
    float rightEdge = m_MapWidth;
    float wrapDistance = rightEdge - leftEdge;

    // 如果角色的座標靠近左邊界
    if (m_X < leftEdge + size.x) {
        transform.translation = {screenX + wrapDistance, screenY};
        m_Image->Draw(Util::ConvertToUniformBufferData(transform, size, 1.0f));
    }
    // 如果角色的座標靠近右邊界
    else if (m_X > rightEdge - size.x) {
        transform.translation = {screenX - wrapDistance, screenY};
        m_Image->Draw(Util::ConvertToUniformBufferData(transform, size, 1.0f));
    }

    //<Ying>
    // 2. 接著，把玩家射出去的所有箭通通畫出來！
    for (auto& arrow : m_PlayerArrows) {
        arrow->Draw(cameraX, cameraY);
    }
}

// <Ying>====== 【新增：受傷邏輯】 =======================
void Player::Hurt() {
    // 如果已經在無敵時間，直接跳過，不重複扣血
    if (m_IsInvincible) return;

    // 扣一顆心
    m_life--;

    // 進入無敵狀態，並設定計時器
    m_IsInvincible = true;
    m_InvincibleTimer = m_InvincibleDuration;

    // 讓角色受傷的瞬間立刻隱形（啟動閃爍）
    m_IsVisible = false;
    m_FlashTimer = 0.0f;

    // 【可選】如果你希望受傷時角色被彈開（Knockback效果），可以在這裡給予一個反向的速度
    m_VelocityY = m_JumpForce * 0.7f; // 往上小跳一下
    m_VelocityX = m_FacingRight ? -m_Speed*2.0: m_Speed*2.0 ;// 往後彈
}
//</Ying>===============================================