# -*- coding: utf-8 -*-
import numpy as np
import cv2
import sys

img = cv2.imread("sample.png",1)
blankValue = 100

#�T�C�Y���傫�������̂ŏ����������i�Ȃ��Ă悵
height = img.shape[0]
width = img.shape[1]
img = cv2.resize(img,(int(width * 0.5),int(height*0.5)))

#�T�C�Y�Ď擾
height = img.shape[0]
width = img.shape[1]

#���摜�쐬(��炩���Ĕ��ƍ������Ȃ��̂ŁA���̂܂܍����ł��Ȃ�
blank = np.zeros((height, width+(blankValue * 2), 3))
blank += 255 #���S�[���f�[�^��255�𑫂��ăz���C�g�ɂ���

#���L�̏����ł͍����s�Abkank�������Ɖ����N�����A�����Ɣ��F���ǉ������
#blank[0:height, 50:width+50] = img

#������߂ăt�@�C���o��(�ŏ�������p�ӂ��Ƃ��ΑS���Ȃ���
cv2.imwrite('blank.png',blank)

#��L�̃t�@�C���A���ǂݍ���(�ǂݍ��񂾂��ƂŃt�@�C���`���I�Ȃ̂��ς�邱�Ƃō����\��
img_blank = cv2.imread("blank.png",1)

#�����Ƀu���b�N�r���S��z�u
img_blank[0 : height, blankValue : width + blankValue] = img

while(True):

	#�����摜�\��
	cv2.imshow("test",img_blank)
	
	#�uQ�v���������܂ŉ摜��\������
	if cv2.waitKey(1) & 0xFF == ord("q"):
		break
		
cv2.destroyAllWindows()