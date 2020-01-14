# OpenCVライブラリのインポート
import cv2
import datetime

def main():
    # Raspberry Pi3（Webサーバ）のURL
    # http://(Raspberry Pi3のIPアドレス)/?action=stream
    URL = "http://192.168.11.100/?action=stream"
    
    # VideoCaptureのインスタンスを作成する。
    # 引数でカメラを選べれる。
    cap = cv2.VideoCapture(URL)

    # カメラFPSを30FPSに設定
    cap.set(cv2.CAP_PROP_FPS, 30)

    # カメラ画像の横幅を1280に設定
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)

    # カメラ画像の縦幅を720に設定
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)

    # 表示するWindow名を設定
    WINDOW_NAME = "camera"
    cv2.namedWindow(WINDOW_NAME)
    
    if cap.isOpened():
        # 画像をキャプチャ
        ret, img = cap.read()
            
        # 画像をPNGファイルへ保存(座標の調整用途)
        cv2.imwrite('snapshot_{0:%Y%m%d_%H%M%S}.png'.format(datetime.datetime.now()), img)
    
    while cap.isOpened():
        ret, frame = cap.read()
        
        # フレームを表示する
        cv2.imshow(WINDOW_NAME, frame)
        
        key = cv2.waitKey(1)&0xff
        
        if key == ord('q'):
            break
        
    cap.release()
    cv2.destroyAllWindows()
    # del mouseData

main()