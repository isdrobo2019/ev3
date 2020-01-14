import CVTrackbar 
import cv2

L = cv2.imread("sample1.png")
cv2.namedWindow('image')

# LowThrという名前のトラックバーの作成
low_threshold_trackbar = CVTrackbar("LowThr", 'image', 0, 200)
low_threshold_trackbar.setValue(20)

# Trackbarの値参照
edge = cv2.Canny(L, low_threshold_trackbar.value(), upper_threshold_trackbar.value())

low_threshold_trackbar = CVTrackbar("LowThr", 'image', 0, 200)
low_threshold_trackbar.setValue(20)

upper_threshold_trackbar = CVTrackbar("UpperThr", 'image', 0, 300)
upper_threshold_trackbar.setValue(40)

while True:

    # Canny Edgeのインタラクティブな描画更新
    if display_mode == display_modes[2]:
        edge = cv2.Canny(L, low_threshold_trackbar.value(), upper_threshold_trackbar.value())
        cv2.imshow('image', edge)