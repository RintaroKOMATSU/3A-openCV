# 3A-openCV

視線検出を用いたインタラクティブゲーム。MediaPipe による目のランドマーク検出と、OpenGL/OpenCV によるゲーム描画を共有メモリで連携させている。

## 構成

- **Python** (MediaPipe): カメラ映像からリアルタイムに目のランドマークを検出し、共有メモリへ書き込む
- **C++** (OpenGL / OpenCV / SDL2): 共有メモリからランドマークを読み取り、ゲームを描画・制御する

## ビルド・実行

```bash
make             # C++ ビルド
python3 main.py  # 実行（eye_detection + ゲーム本体を起動）
```

## 依存

- OpenCV 4
- OpenGL / GLUT
- SDL2 / SDL2_mixer
- MediaPipe (Python)
