# -*- coding: utf-8 -*-
import numpy as np
import cv2
import sys

img = cv2.imread("sample.png",1)
blankValue = 100

#サイズが大きかったので小さくした（省いてよし
height = img.shape[0]
width = img.shape[1]
img = cv2.resize(img,(int(width * 0.5),int(height*0.5)))

#サイズ再取得
height = img.shape[0]
width = img.shape[1]

#白画像作成(やらかして白と黒しかないので、そのまま合成できない
blank = np.zeros((height, width+(blankValue * 2), 3))
blank += 255 #←全ゼロデータに255を足してホワイトにする

#下記の処理では合成不可、bkankが白だと何も起きず、黒だと白色が追加される
#blank[0:height, 50:width+50] = img

#あきらめてファイル出力(最初っから用意しとけば全部省ける
cv2.imwrite('blank.png',blank)

#上記のファイル、即読み込み(読み込んだことでファイル形式的なのが変わることで合成可能に
img_blank = cv2.imread("blank.png",1)

#中央にブロックビンゴを配置
img_blank[0 : height, blankValue : width + blankValue] = img

while(True):

	#合成画像表示
	cv2.imshow("test",img_blank)
	
	#「Q」が押されるまで画像を表示する
	if cv2.waitKey(1) & 0xFF == ord("q"):
		break
		
cv2.destroyAllWindows()