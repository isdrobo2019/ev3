import cv2

image = cv2.imread('snapshot_20190908_171431.png')

# # 読み込んだ画像の高さと幅を取得
# height = image.shape[0]
# width = image.shape[1]

# # アスペクト比を固定して、幅が指定した値になるようリサイズする。
# scale = 1280 / image.shape[1]
# resized_img = cv2.resize(image, dsize=None, fx=scale, fy=scale)

# cv2.imshow('a', resized_img)
cv2.imshow('a', image)

cv2.waitKey(0) # 待ち
cv2.destroyAllWindows() # 解放
