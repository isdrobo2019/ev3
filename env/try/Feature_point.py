import numpy as np
import cv2
img1 = cv2.imread('seven.png',1)
img2 = cv2.imread('area7.png',1)
# #特徴抽出機の生成
# detector = cv2.Feature2D.()
# #kpは特徴的な点の位置 destは特徴を現すベクトル
# kp1, des1 = detector.detectAndCompute(img1, None)
# kp2, des2 = detector.detectAndCompute(img2, None)
# #特徴点の比較機
# bf = cv2.BFMatcher()
# matches = bf.knnMatch(des1,des2, k=2)
# #割合試験を適用
# good = []
# match_param = 0.6
# for m,n in matches:
#     if m.distance < match_param*n.distance:
#         good.append([m])
# #cv2.drawMatchesKnnは適合している点を結ぶ画像を生成する
# img3 = cv2.drawMatchesKnn(img1,kp1,img2,kp2,good, None,flags=2)
# cv2.imwrite("shift_result.png", img3)

# median1 = cv2.medianBlur(img1, 31) # ぼかし
# median2 = cv2.medianBlur(img2, 31) # ぼかし
# img1 = median1
# img2 = median2

# A-KAZE検出器の生成
akaze = cv2.AKAZE_create()                                

# 特徴量の検出と特徴量ベクトルの計算
kp1, des1 = akaze.detectAndCompute(img1, None)
kp2, des2 = akaze.detectAndCompute(img2, None)

# Brute-Force Matcher生成
bf = cv2.BFMatcher()

# 特徴量ベクトル同士をBrute-Force＆KNNでマッチング
matches = bf.knnMatch(des1, des2, k=2)

# データを間引きする
ratio = 0.7
good = []
for m, n in matches:
    if m.distance < ratio * n.distance:
        good.append([m])

# 対応する特徴点同士を描画
img3 = cv2.drawMatchesKnn(img1, kp1, img2, kp2, good, None, flags=2)

# 画像表示
# cv2.imshow('img', img3)
# cv2.waitKey(0)

# # ORB特徴点検出器を作る
# orb = cv2.ORB_create()

# # ORBでキーポイントと特徴記述子を求める
# kp1, des1 = orb.detectAndCompute(img1,None)
# kp2, des2 = orb.detectAndCompute(img2,None)

# # BFMatcherオブジェクトを作る
# bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)

# # 特徴記述子のマッチング
# matches = bf.match(des1,des2)

# # 距離でソートする
# matches = sorted(matches, key = lambda x:x.distance)

# # 最初の（上位）10個の対応点を描画
# img3 = cv2.drawMatches(img1,kp1,img2,kp2,matches, None,flags=2)



cv2.imshow('img', img3)

# キー押下で終了
cv2.waitKey(0)
cv2.destroyAllWindows()