import cv2
import numpy as np

image = cv2.imread('change.png') # ファイル読み込み

# HSVでの色抽出
# 青の抽出
hsvLower = np.array([107, 77, 35])    # 抽出する色の下限(HSV)
hsvUpper = np.array([246, 28, 62])    # 抽出する色の上限(HSV)

# 緑の抽出
# hsvLower = np.array([30, 153, 255])    # 抽出する色の下限(HSV)
# hsvUpper = np.array([30, 153, 255])    # 抽出する色の上限(HSV)

# 赤の抽出
# hsvLower = np.array([30, 153, 255])    # 抽出する色の下限(HSV)
# hsvUpper = np.array([30, 153, 255])    # 抽出する色の上限(HSV)

# 黄の抽出
# hsvLower = np.array([30, 153, 255])    # 抽出する色の下限(HSV)
# hsvUpper = np.array([30, 153, 255])    # 抽出する色の上限(HSV)

hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV) # 画像をHSVに変換
hsv_mask = cv2.inRange(hsv, hsvLower, hsvUpper)    # HSVからマスクを作成
result = cv2.bitwise_and(image, image, mask=hsv_mask) # 元画像とマスクを合成

cv2.imshow("Extraction",result)

cv2.waitKey(0)
cv2.destroyAllWindows()
