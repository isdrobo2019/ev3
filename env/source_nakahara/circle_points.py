# -*- coding: utf-8 -*-
import numpy as np
import cv2
import sys

#四隅の座標
list = []

#全体の座標
list2 = []

def mouse_event(event, x, y, flags, param):

	if event == cv2.EVENT_LBUTTONUP:
		if len(list) < 4:
		
			#円を出力
			cv2.circle(img, (x, y), 3,(0, 0, 255), -1)
			
			list.append((x,y))
			print("X:{0} Y:{1}".format(list[len(list)-1][0],list[len(list)-1][1]))
		
		#四隅取得後
		elif len(list2) == 0:
			
			#X:左上が0　右に行けば数値が上がる、Y:左上が0　下に行けば数値が上がる
			
			#左上の交点サークルの座標を格納
			list2.append(list[0])
			
			#四隅の4回分
			for i in range(4):
			
				if i < 3:
					#点iと点i+1の間(2点)の座標を取得
					Xval1 = list[i][0] + ((list[i + 1][0] - list[i][0])/3)
					Yval1 = list[i][1] + ((list[i + 1][1] - list[i][1])/3)
					Xval2 = list[i][0] + (((list[i + 1][0] - list[i][0])/3) *2)
					Yval2 = list[i][1] + (((list[i + 1][1] - list[i][1])/3) *2)
					
					#リストに格納してみた。間(2点)と点i+1まで
					list2.append((round(Xval1), round(Yval1)))
					list2.append((round(Xval2), round(Yval2)))
					list2.append(list[i+1])
					
				#最初の点と最後の点の中間を取得
				else:
					Xval1 = list[0][0] + ((list[i][0] - list[0][0])/3)
					Yval1 = list[0][1] + ((list[i][1] - list[0][1])/3)
					Xval2 = list[0][0] + (((list[i][0] - list[0][0])/3) *2)
					Yval2 = list[0][1] + (((list[i][1] - list[0][1])/3) *2)
					
					#間の2点のみ格納
					list2.append((round(Xval2), round(Yval2)))
					list2.append((round(Xval1), round(Yval1)))
					
					#print("X:{0} Y:{1}".format(Xval1,Yval1))
					#print("X:{0} Y:{1}".format(Xval2,Yval2))
			
				#円を出力
				cv2.circle(img, (round(Xval1), round(Yval1)), 3,(0, 0, 255), -1)
				cv2.circle(img, (round(Xval2), round(Yval2)), 3,(0, 0, 255), -1)
			
			
			#今list2の座標の配列イメージ
			# 0 1 2 3
			#11     4
			#10     5
			# 9 8 7 6
			#print(list2)
			
			#挿入
			list2.insert(4, list2[11])
			list2.insert(6, list2[11])
			
			#挿入した分削除
			del list2[12:]
			
			#今list2の座標の配列イメージ
			# 0 1 2 3
			# 4     5
			# 6     7
			#1110 9 8
			
			#ソート入れ替え
			for i in range(4):
				
				i = i + 1
				list2.insert(i+7, list2[-i])
			
			#挿入した分削除
			del list2[12:]

			#今list2の座標の配列イメージ
			# 0 1 2 3
			# 4     5
			# 6     7
			# 8 91011
			
			#枠の中の交点(4点)取得
			for i in range(2):
				i = (i*4) + 4
				Xval1 = list2[i][0] + ((list2[i + 1][0] - list2[i][0])/3)
				Yval1 = list2[i][1] + ((list2[i + 1][1] - list2[i][1])/3)
				Xval2 = list2[i][0] + (((list2[i + 1][0] - list2[i][0])/3) *2)
				Yval2 = list2[i][1] + (((list2[i + 1][1] - list2[i][1])/3) *2)
				
				#間の2点のみ格納
				list2.insert(i+1, (round(Xval1), round(Yval1)))
				list2.insert(i+2, (round(Xval2), round(Yval2)))
				#円を出力
				cv2.circle(img, (round(Xval1), round(Yval1)), 3,(0, 0, 255), -1)
				cv2.circle(img, (round(Xval2), round(Yval2)), 3,(0, 0, 255), -1)
			
			print(list2)
			print(len(list2))
	
	if event == cv2.EVENT_MBUTTONDOWN and len(list2) == 16:
		
		for i in range(len(list2)):
		
			if (list2[i][0] - 40) < x and (list2[i][0] + 40) > x:
				if (list2[i][1] - 40) < y and (list2[i][1] + 40) > y:
					list2[i] = (x,y)
					print(list2)
					cv2.circle(img, (list2[i][0], list2[i][1]), 3,(0, 255, 0), -1)

					break

img = cv2.imread("change.png",1)

#リセット用、特に意味はない
img2 = cv2.imread("change.png",1)


#画像サイズを圧縮
height = img.shape[0]
width = img.shape[1]
#img = cv2.resize(img,(int(width * 0.5),int(height*0.5)))

#hsv変換
hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

cv2.namedWindow("test",cv2.WINDOW_NORMAL)
cv2.setMouseCallback("test", mouse_event)

while(True):

	#合成画像表示
	cv2.imshow("test",img)
	
	#aでリセットを作りたかっただけ上手くいってない
	if cv2.waitKey(1) & 0xFF == ord("a"):
		list.clear()
		list2.clear()
		img = img2.copy()
	#「Q」が押されるまで画像を表示する
	if cv2.waitKey(1) & 0xFF == ord("q"):
		break
		
cv2.destroyAllWindows()