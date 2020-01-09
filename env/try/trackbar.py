# -*- coding: utf-8 -*-
import numpy as np
import cv2
import sys

#�Ȃ񂩃T�C�Y���ς����ǂ�����ւ�͂ǂ��ɂ�����
#�ςȂ̂𒼂����Ɖ摜�T�C�Y�����k�Ȃǂ����Ă݂��Ӗ��Ȃ�����

#����͂悭�킩���A�Ƃ肠�����쐬
def changeColor(val):
	pass

img = cv2.imread("change.png")
print(img)
# cv2.imshow('a',img)

#�摜�T�C�Y�����k
height = img.shape[0]
width = img.shape[1]
#img = cv2.resize(img,(int(width * 0.5),int(height*0.5)))

#hsv�ϊ�
hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

#�Ƃ肠�����g�̖��O�w��
cv2.namedWindow("test",cv2.WINDOW_KEEPRATIO | cv2.WINDOW_NORMAL)
#cv2.namedWindow("test")

#�g���b�N�o�[����
cv2.createTrackbar('L_H',"test",0,180,changeColor)
cv2.createTrackbar('L_S',"test",0,255,changeColor)
cv2.createTrackbar('L_V',"test",0,255,changeColor)

cv2.createTrackbar('U_H',"test",180,180,changeColor)
cv2.createTrackbar('U_S',"test",255,255,changeColor)
cv2.createTrackbar('U_V',"test",255,255,changeColor)


while(True):

	#�g���b�N�o�[�̒l�擾
	l_h = cv2.getTrackbarPos("L_H", "test")
	l_s = cv2.getTrackbarPos("L_S", "test")
	l_v = cv2.getTrackbarPos("L_V", "test")

	u_h = cv2.getTrackbarPos("U_H", "test")
	u_s = cv2.getTrackbarPos("U_S", "test")
	u_v = cv2.getTrackbarPos("U_V", "test")

	#臒l
	lower_color = np.array([l_h,l_s,l_v])
	upper_color = np.array([u_h,u_s,u_v])
	
	#�}�X�L���O�p�摜����
	img_mask = cv2.inRange(hsv, lower_color, upper_color)
	
	#����
	img_color = cv2.bitwise_and(img, img, mask=img_mask)

	#�����摜�\��
	cv2.imshow("test",img_color)
	
	#�uQ�v���������܂ŉ摜��\������
	if cv2.waitKey(1) & 0xFF == ord("q"):
		break
		
cv2.destroyWindow("test")