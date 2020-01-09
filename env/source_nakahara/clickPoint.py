# -*- coding: utf-8 -*-
import numpy as np
import cv2
import sys

#�l���̍��W
list = []

#�S�̂̍��W
list2 = []

def mouse_event(event, x, y, flags, param):

	if event == cv2.EVENT_LBUTTONUP:
		if len(list) < 4:
		
			#�~���o��
			cv2.circle(img, (x, y), 10,(0, 0, 255), -1)
			
			list.append((x,y))
			print("X:{0} Y:{1}".format(list[len(list)-1][0],list[len(list)-1][1]))
		
		#�l���擾��
		elif len(list2) == 0:
			
			#X:���オ0�@�E�ɍs���ΐ��l���オ��AY:���オ0�@���ɍs���ΐ��l���オ��
			
			#����̌�_�T�[�N���̍��W���i�[
			list2.append(list[0])
			
			#�l����4��
			for i in range(4):
			
				if i < 3:
					#�_i�Ɠ_i+1�̊�(2�_)�̍��W���擾
					Xval1 = list[i][0] + ((list[i + 1][0] - list[i][0])/3)
					Yval1 = list[i][1] + ((list[i + 1][1] - list[i][1])/3)
					Xval2 = list[i][0] + (((list[i + 1][0] - list[i][0])/3) *2)
					Yval2 = list[i][1] + (((list[i + 1][1] - list[i][1])/3) *2)
					
					#���X�g�Ɋi�[���Ă݂��B��(2�_)�Ɠ_i+1�܂�
					list2.append((round(Xval1), round(Yval1)))
					list2.append((round(Xval2), round(Yval2)))
					list2.append(list[i+1])
					
				#�ŏ��̓_�ƍŌ�̓_�̒��Ԃ��擾
				else:
					Xval1 = list[0][0] + ((list[i][0] - list[0][0])/3)
					Yval1 = list[0][1] + ((list[i][1] - list[0][1])/3)
					Xval2 = list[0][0] + (((list[i][0] - list[0][0])/3) *2)
					Yval2 = list[0][1] + (((list[i][1] - list[0][1])/3) *2)
					
					#�Ԃ�2�_�̂݊i�[
					list2.append((round(Xval2), round(Yval2)))
					list2.append((round(Xval1), round(Yval1)))
					
					#print("X:{0} Y:{1}".format(Xval1,Yval1))
					#print("X:{0} Y:{1}".format(Xval2,Yval2))
			
				#�~���o��
				cv2.circle(img, (round(Xval1), round(Yval1)), 10,(0, 0, 255), -1)
				cv2.circle(img, (round(Xval2), round(Yval2)), 10,(0, 0, 255), -1)
			
			
			#��list2�̍��W�̔z��C���[�W
			# 0 1 2 3
			#11     4
			#10     5
			# 9 8 7 6
			#print(list2)
			
			#�}��
			list2.insert(4, list2[11])
			list2.insert(6, list2[11])
			
			#�}���������폜
			del list2[12:]
			
			#��list2�̍��W�̔z��C���[�W
			# 0 1 2 3
			# 4     5
			# 6     7
			#1110 9 8
			
			#�\�[�g����ւ�
			for i in range(4):
				
				i = i + 1
				list2.insert(i+7, list2[-i])
			
			#�}���������폜
			del list2[12:]

			#��list2�̍��W�̔z��C���[�W
			# 0 1 2 3
			# 4     5
			# 6     7
			# 8 91011
			
			#�g�̒��̌�_(4�_)�擾
			for i in range(2):
				i = (i*4) + 4
				Xval1 = list2[i][0] + ((list2[i + 1][0] - list2[i][0])/3)
				Yval1 = list2[i][1] + ((list2[i + 1][1] - list2[i][1])/3)
				Xval2 = list2[i][0] + (((list2[i + 1][0] - list2[i][0])/3) *2)
				Yval2 = list2[i][1] + (((list2[i + 1][1] - list2[i][1])/3) *2)
				
				#�Ԃ�2�_�̂݊i�[
				list2.insert(i+1, (round(Xval1), round(Yval1)))
				list2.insert(i+2, (round(Xval2), round(Yval2)))
				#�~���o��
				cv2.circle(img, (round(Xval1), round(Yval1)), 10,(0, 0, 255), -1)
				cv2.circle(img, (round(Xval2), round(Yval2)), 10,(0, 0, 255), -1)
			
			print(list2)
			print(len(list2))
			
			if len(list2) > 15:
				path = 'circle_points.txt'
				with open(path, mode='w') as f:
					for i in range(len(list2)):
						f.write(str(list2[i][0]) + "," + str(list2[i][1]))
						if i != 15:
							f.write("\n")
					
	if event == cv2.EVENT_RBUTTONUP:
		list.clear()
		list2.clear()
		

img = cv2.imread("change.png",1)

#���Z�b�g�p�A���ɈӖ��͂Ȃ�
img_delete = img.copy()


#�摜�T�C�Y�����k
height = img.shape[0]
width = img.shape[1]
#img = cv2.resize(img,(int(width * 0.5),int(height*0.5)))

#hsv�ϊ�
hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

cv2.namedWindow("test", cv2.WINDOW_NORMAL)
cv2.setMouseCallback("test", mouse_event)

while(True):

	#�����摜�\��
	cv2.imshow("test",img)
	
	#a�Ń��Z�b�g����肽������������肭�����ĂȂ�
	if cv2.waitKey(1) & 0xFF == ord("a"):
		mouse_event(cv2.EVENT_RBUTTONUP, 0, 0, 0, 0)
		img = img_delete.copy()
	#�uQ�v���������܂ŉ摜��\������
	if cv2.waitKey(1) & 0xFF == ord("q"):
		break
		
cv2.destroyAllWindows()