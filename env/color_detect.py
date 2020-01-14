from functools import reduce
import struct
import numpy as np
import cv2
import datetime

from sample_types import CommandCode
from sample_types import ResponseCode
from sample_types import Color
from capture import Capture
import route_calc


# # 座標IDとピクセル座標の連想リスト(ピクセル座標はカメラ位置に応じて調整して下さい)
circle_no = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16]
path = 'circle_points.txt'
circle_point = []
with open(path) as f:
    lines = f.read().split("\n")
    for line in lines:
        sp = line.split(",")
        circle_point.append([int(sp[0]), int(sp[1])])

dots = dict(zip(circle_no, circle_point))

colorlist = []
result = [] # 判定結果のリスト
img = cv2.imread("change.png")

for (x, y) in circle_point:
    # RGBからHSVへ変換
    pixelValue = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)[y, x]
    
    # 色判定
    col = Color.getColor(pixelValue) # HSVから色コードへ変換
    result.append((x, y, col)) # (x座標,y座標,色)タプルをリストへ追加
    colorlist.append(col)

    # 色情報を画像へ付加
    dot_num = [k for k, v in dots.items() if (x == v[0] and y == v[1])][0]
    H = str(pixelValue[0])
    S = str(pixelValue[1])
    V = str(pixelValue[2])
    cv2.circle(img,(x, y), 3, (255,255,255), -1)
    cv2.putText(img, ("[%d]:" % dot_num) + Color.toColorName(col), (x+10, y+5), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255), 1, cv2.LINE_AA)
    cv2.putText(img, H+","+S+","+V, (x+10, y+25), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255), 1, cv2.LINE_AA)

all_route = route_calc.get_all_route(11, 3, colorlist, 1, 1)
print(all_route)
print(colorlist)
# 画像をJPEGファイルへ保存(座標の調整用途)
cv2.imwrite('snapshot_{0:%Y%m%d_%H%M%S}.jpg'.format(datetime.datetime.now()), img)
