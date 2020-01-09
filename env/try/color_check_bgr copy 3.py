import cv2
import numpy as np

image = cv2.imread('change.png') # ファイル読み込み
# image = cv2.imread('transform.png') # ファイル読み込み

# #トラックバーのコールバック関数の設定
# cv2.createTrackbar("R", "img", 0, 255, img_mask)
# cv2.createTrackbar("G", "img", 0, 255, img_mask)
# cv2.createTrackbar("B", "img", 0, 255, img_mask)

# BGRでの色抽出
# 青の抽出
# bgrLower = np.array([90, 50, 20])   # 抽出する色の下限(BGR)
# bgrUpper = np.array([160, 115, 120])   # 抽出する色の上限(BGR)

# 緑の抽出
# bgrLower = np.array([0, 60, 20])   # 抽出する色の下限(BGR)
# bgrUpper = np.array([80, 130, 90])   # 抽出する色の上限(BGR)

# 赤の抽出
bgrLower = np.array([0, 0, 70])   # 抽出する色の下限(BGR)
bgrUpper = np.array([60, 80, 160])   # 抽出する色の上限(BGR)

# 黄の抽出
# bgrLower = np.array([0, 70, 100])   # 抽出する色の下限(BGR)
# bgrUpper = np.array([60, 140, 170])   # 抽出する色の上限(BGR)

img_mask = cv2.inRange(image, bgrLower, bgrUpper) # BGRからマスクを作成
result = cv2.bitwise_and(image, image, mask=img_mask) # 元画像とマスクを合成

bounding_img = np.copy(result)
pt_img = np.copy(result)

# オブジェクト検出用の前処理
median = cv2.medianBlur(bounding_img, 51) # ぼかし
gray = cv2.cvtColor(median, cv2.COLOR_BGR2GRAY) # グレースケール
ret,thresh = cv2.threshold(gray,50,255,0) # 二値化

contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE) # 輪郭抽出

# 外接矩形によるオブジェクト検出
for contour in contours:
    x, y, w, h = cv2.boundingRect(contour)
    # 輪郭の面積を計算する。
    area = cv2.contourArea(contour)
    print(area)
    if area > 500:
        cv2.rectangle(bounding_img, (x, y), (x + w, y + h), (0, 255, 0), 3) # 外接矩形描画
        a, b = (x, y + h)
        cv2.drawMarker(pt_img, (a, b), color=(255, 0, 0),markerType=cv2.MARKER_CROSS, thickness=3) # 左下の座標描画
        
cv2.imshow("Extraction", pt_img) # 外接矩形適用画像出力

# キャニー法（ハフ関数による円検出の前処理）
# find binary image with edges
# circles_edges = cv2.Canny(result, threshold1=90, threshold2=110)

# find hough circles ハフ関数による円検出
# circles = cv2.HoughCircles(circles_edges, cv2.HOUGH_GRADIENT, dp=1.5, minDist=40, param2=60, minRadius=10, maxRadius=150)
# circles_copy = np.copy(result)

# circles = circles[0]
# for (x, y, r) in circles:
#     x, y, r = int(x), int(y), int(r)
#     cv2.circle(circles_copy, (x, y), r, (255, 255, 0), 4)

# cv2.imshow("Extraction", circles_copy)  # 円適用画像出力




# rect = cv2.minAreaRect(cnt)
# box = cv2.boxPoints(rect)
# box = np.int0(box)
# im = cv2.drawContours(result,[box],0,(0,0,255),2)




# cv2.imshow("Extraction", 出力イメージ)

cv2.waitKey(0) # 待ち
cv2.destroyAllWindows() # 解放
