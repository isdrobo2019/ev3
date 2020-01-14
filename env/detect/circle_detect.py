import cv2
import numpy as np

image = cv2.imread('sample2.png') # ファイル読み込み

# 検出のための前処理
# median = cv2.medianBlur(image, 3) # ぼかし
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY) # グレースケール
# ret,thresh = cv2.threshold(gray,130,255,0) # 二値化
circles_edges = cv2.Canny(gray, threshold1=30, threshold2=70) # キャニー法

# find hough circles ハフ関数による円検出
circles = cv2.HoughCircles(circles_edges, cv2.HOUGH_GRADIENT, dp=1, minDist=100, param2=15, minRadius=10, maxRadius=100)
circles_copy = np.copy(image)

# i = 0
circles = circles[0]
for (x, y, r) in circles:
    # if i == 7 :
        x, y, r = int(x), int(y), int(r)
        cv2.circle(circles_copy, (x, y), r, (255, 255, 0), 4)
    # i+=1
    

cv2.imshow("Extraction", circles_copy)  # 円適用画像出力
# cv2.imshow("Extraction", thresh) 
cv2.imwrite("canny_circle.png", circles_copy)

cv2.waitKey(0) # 待ち
cv2.destroyAllWindows() # 解放

