import cv2
import numpy as np

class Capture:

    def image_capture(filename):

        # Raspberry Pi3（カメラシステム）のURL
        url = "http://192.168.11.100/?action=stream"

        # VideoCaptureのインスタンスを作成する。
        cap = cv2.VideoCapture(url) # カメラシステムを使う場合

        # カメラFPSを30FPSに設定
        cap.set(cv2.CAP_PROP_FPS, 30)

        # カメラ画像の横幅を1280に設定
        cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)

        # カメラ画像の縦幅を720に設定
        cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)

        # ピクセル配列をゼロ初期化
        img = np.zeros((720, 1280, 3), dtype=np.uint8)

        if cap.isOpened():
            # 画像をキャプチャ
            ret, img = cap.read()
                
            # 画像をpngファイルへ保存(座標の調整用途)
            cv2.imwrite(filename, img)

        # キャプチャ終了
        cap.release()

        # return result