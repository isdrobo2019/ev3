import cv2
import numpy as np

filename = "image.png"
windowname = "image"

# 画像の読み込み
img = cv2.imread(filename,1)
# cv2.imshow(windowname, img)

# 白黒画像で画像を読み込み
# gray = cv2.imread(filename_before,0)
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# cv2.imshow(windowname, gray)

ret,th1 = cv2.threshold(gray,130,255,cv2.THRESH_BINARY)
cv2.imshow(windowname, th1)

contours, hierarchy = cv2.findContours(th1, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

dst = []

# pts1 = np.float32([[1396,305],[718,170],[164,402],[1264,773]])
# pts1 = np.float32([[1421,299],[698,159],[109,408],[1296,832]])
path = 'point_list.txt'
points = []
with open(path) as f:
    lines = f.read().split("\n")
    for line in lines:
        sp = line.split(",")
        points.append([int(sp[0]), int(sp[1])])
pts1 = np.float32([points[0],points[1],points[2],points[3]])

pts2 = np.float32([[600,300],[600,0],[0,0],[0,300]])

M = cv2.getPerspectiveTransform(pts1,pts2)
dst = cv2.warpPerspective(img,M,(600,300))

# 読み込んだ画像の高さと幅を取得
height = dst.shape[0]
width = dst.shape[1]

# 画像のサイズを変更
 # 第一引数：サイズを変更する画像
 # 第二引数：変更後の幅
 # 第三引数：変更後の高さ
resized_img = cv2.resize(dst,(width*1, height*2))

cv2.imshow(windowname, resized_img)

cv2.waitKey(0)
cv2.destroyAllWindows()


