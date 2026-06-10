#include "DxLib.h"

// 画面サイズ定義
#define SCREEN_WIDTH  600
#define SCREEN_HEIGHT 600

// オブジェクトの構造体定義
typedef struct {
    double posX;
    double posY;
    double velocityX;
    double velocityY;
    double accelY;
    int size;
} Ball;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // ---- 1. DxLib 初期化設定 ----
    ChangeWindowMode(TRUE);
    SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);
    SetMainWindowText("Velocity Simulation (DxLib)");

    if (DxLib_Init() == -1) return -1;

    SetDrawScreen(DX_SCREEN_BACK); // ダブルバッファリング有効化

    // ---- 2. 変数・物理パラメータの初期化 ----
    // 60fps環境に合わせて1フレームあたりの移動・加速度を調整
    Ball ball;
    ball.posX = 0.0;
    ball.posY = 500.0; // 画面内に収まる初期位置（JavaScript版の床は600）
    ball.velocityX = 2.0;   // X軸の移動速度
    ball.velocityY = -10.0; // ジャンプ初速（上方向への力）
    ball.accelY = 0.3;   // 重力加速度
    ball.size = 10;    // 四角形のサイズ

    // 外枠（床）の閾値
    double floorY = (double)(SCREEN_HEIGHT - ball.size);

    // ---- 3. メインループ ----
    while (ScreenFlip() == 0 && ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {

        // 画面クリア（JavaScript版の ctx.fillRect(0, 0, 600, 600) に相当）
        ClearDrawScreen();

        // ---- 4. 物理演算・移動処理 (tick) ----
        ball.posX += ball.velocityX;
        ball.velocityY += ball.accelY;
        ball.posY += ball.velocityY;

        // X軸の画面外判定（右端にいったら左端へ戻る）
        if (ball.posX > SCREEN_WIDTH) {
            ball.posX = 0.0;
        }

        // Y軸の床判定（バウンド処理）
        if (ball.posY > floorY) {
            ball.posY = floorY;     // 床にめり込まないように位置補正
            ball.velocityY = -10.0; // 反発初速を再設定
        }

        // ---- 5. 描画処理 (paint) ----
        // 白い四角形を描画
        DrawBox(
            (int)ball.posX,
            (int)ball.posY,
            (int)ball.posX + ball.size,
            (int)ball.posY + ball.size,
            GetColor(255, 255, 255),
            TRUE
        );

    }

    // ---- 6. 終了処理 ----
    DxLib_End();
    return 0;
}
