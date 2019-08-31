import cv2
import numpy as np

image = cv2.imread('change.png') # ファイル読み込み

# BGRでの色抽出
# 青の抽出
# bgrLower = np.array([90, 50, 20])   # 抽出する色の下限(BGR)
# bgrUpper = np.array([160, 115, 120])   # 抽出する色の上限(BGR)

# 緑の抽出
# bgrLower = np.array([0, 60, 20])   # 抽出する色の下限(BGR)
# bgrUpper = np.array([90, 130, 90])   # 抽出する色の上限(BGR)

# 赤の抽出
# bgrLower = np.array([0, 0, 70])   # 抽出する色の下限(BGR)
# bgrUpper = np.array([50, 50, 160])   # 抽出する色の上限(BGR)

# 黄の抽出
bgrLower = np.array([0,70, 100])   # 抽出する色の下限(BGR)
bgrUpper = np.array([60, 140, 170])   # 抽出する色の上限(BGR)

img_mask = cv2.inRange(image, bgrLower, bgrUpper) # BGRからマスクを作成
result = cv2.bitwise_and(image, image, mask=img_mask) # 元画像とマスクを合成

cv2.imshow("Extraction",result)  # ファイル出力

cv2.waitKey(0) # 待ち
cv2.destroyAllWindows() # 解放
