import numpy as np
import cv2
 

filename = "aaa.jpeg"
windowname = "image"
img = cv2.imread(filename)

# 画像の２値化（グレースケール）
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

ret,th1 = cv2.threshold(gray,200,255,cv2.THRESH_BINARY)
cv2.imshow(windowname, th1)


contours, hierarchy = cv2.findContours(th1, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

areas = []
for cnt in contours:
    area = cv2.contourArea(cnt)
    if area > 10000:
        epsilon = 0.1*cv2.arcLength(cnt,True)
        approx = cv2.approxPolyDP(cnt,epsilon,True)
        areas.append(approx)

cv2.drawContours(img,areas,-1,(0,255,0),3)



dst = []

pts1 = np.float32(areas[0])
pts2 = np.float32([[600,300],[600,0],[0,0],[0,300]])

M = cv2.getPerspectiveTransform(pts1,pts2)
dst = cv2.warpPerspective(img,M,(600,300))

cv2.imshow(windowname, dst)


# cv2.imshow(windowname, img)
cv2.waitKey(0)
cv2.destroyAllWindows()
