import numpy as np
import cv2
# import sys
class Number:

	def bonus_num_detect():
		# 射影変換後の数字カード
		img = cv2.imread("./number.png",0)

		# サイズ取得
		height = img.shape[0]
		width = img.shape[1]

		# 二値化
		ret,unknown = cv2.threshold(img, 0, 255, cv2.THRESH_OTSU)

		# 数字カードテンプレ読み込み
		numberList = []
		numberList.append(cv2.imread("./numberCard/one.png",0))
		numberList.append(cv2.imread("./numberCard/two.png",0))
		numberList.append(cv2.imread("./numberCard/three.png",0))
		numberList.append(cv2.imread("./numberCard/four.png",0))
		numberList.append(cv2.imread("./numberCard/five.png",0))
		numberList.append(cv2.imread("./numberCard/six.png",0))
		numberList.append(cv2.imread("./numberCard/seven.png",0))
		numberList.append(cv2.imread("./numberCard/eight.png",0))

		# マッチングの類似度格納用
		matchPercent = []

		# 1～8を比較
		for i in range(8):
			
			# 数字カードのサイズに合わせる
			numberList[i] = cv2.resize(numberList[i], dsize =(width, height))
			
			# 二値化
			ret,numberList[i] = cv2.threshold(numberList[i], 0, 255, cv2.THRESH_OTSU)
			
			# マッチングの類似度取得
			result = cv2.matchTemplate(unknown, numberList[i],cv2.TM_CCOEFF_NORMED)
			matchPercent.append(result[0][0])

		# リストから最大値取得
		# print(max(matchPercent))

		# 類似度の最大値から要素数取得
		matchNumber = matchPercent.index(max(matchPercent))

		while(True):

			cv2.imshow("test",unknown)
			
			# 一致してると思われる数字カードを出力
			cv2.imshow("test2",numberList[matchNumber])
			
			# 「Q」が押されるまで画像を表示する
			if cv2.waitKey(1) & 0xFF == ord("q"):
				break
				
		# cv2.destroyWindow("test")
		cv2.destroyAllWindows()
		print(matchNumber + 1)

		if matchNumber < 4
			return matchNumber
		else 
			return matchNumber + 1

	bonus_num_detect()

