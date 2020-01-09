#!/usr/bin/env python
# coding: utf-8
from functools import reduce
import struct
import numpy
import cv2
import datetime

from sample_types import CommandCode
from sample_types import ResponseCode
from sample_types import Color
from SerialInterface import SerialInterface
from transform import Transform
# from number import Number
import route_calc


port = "COM4" # 環境に合わせて変更して下さい
baud = 115200
circle_no = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16]
circle_point = []

path = 'circle_points.txt'
with open(path) as f:
    lines = f.read().split("\n")
    for line in lines:
        sp = line.split(",")
        circle_point.append([int(sp[0]), int(sp[1])])

dots = dict(zip(circle_no, circle_point))

Transform.capture_img_for_detect()


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

    if matchNumber < 4:
        return matchNumber
    else :
        return matchNumber + 1



####################################################
# 座標IDとピクセル座標の連想リスト(辞書)を作成する関数 #
####################################################
def createDict():
    circle_no = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16]
    circle_point = []

    path = 'circle_points.txt'
    with open(path) as f:
        lines = f.read().split("\n")
        for line in lines:
            sp = line.split(",")
            circle_point.append([int(sp[0]), int(sp[1])])

    dots = dict(zip(circle_no, circle_point))

    return dots


##############################
# 要求パケットを解読する関数 #
##############################
def decodePacket(command, parameter):

    # 引数チェック
    if command != CommandCode.All and command != CommandCode.Specific: # 未定義のコマンドコード
        errmsg = "unsupported command"
        return None, errmsg

    points   = [] # 色判定対象ピクセル座標のリスト
    if command == CommandCode.All: # 全座標色取得コマンド
        points = dots.values() # 全サークル分の座標リストを作る
    elif command == CommandCode.Specific: # 指定座標色取得コマンド
        num_of_dots = int(parameter[0]) # 座標数
        for i in range(num_of_dots):
            # ネットワークバイトオーダー(!)で座標IDを読み出す
            dot_id = struct.unpack("!H", parameter[2*i+1:2*i+3])[0]
            # 座標IDに対応するピクセル座標をリストへ追加
            points.append( dots[dot_id] )
    return points, None


############################
# 応答パケットを生成する関数 #
############################
def encodePacket(rescode, result, errmsg=None):
    # 引数チェック：問題がある場合はエラー応答用のメッセージを設定
    if rescode != ResponseCode.Color and rescode != ResponseCode.Error: # 未定義のレスポンスコード
        lrescode = ResponseCode.Error
        lerrmsg = "server internal error"
    elif rescode == ResponseCode.Color and (len(result) > 84): # 応答座標数がプロトコルの規定上限を超えている
        lrescode = ResponseCode.Error
        lerrmsg = "response size is too big"
    else: # 引数が正常のケース
        lrescode = rescode
        lerrmsg = errmsg
    
    # if lrescode == ResponseCode.Color: # 座標色応答のケース
    #     # ネットワークバイトオーダー(!)で座標ID,色コードをバイナリ化
    #     data = struct.pack("B", len(result)) # 座標数
    #     for item in result:
    #         data += struct.pack(
    #             "!HB", 
    #             [k for k, (x, y) in dots.items() if (x == item[0] and y == item[1])][0], # ピクセル座標から座標IDへの変換
    #             item[2] # ピクセル座標の色コード
    #         )
    # else: # エラー応答のケース
    #     # エラーメッセージをUTF8エンコードでバイナリ化
    #     data = struct.pack(str(len(lerrmsg)) + "s", bytes(lerrmsg, 'utf-8'))

    return lrescode, struct.pack("B"*len(data), *data)


#######################################################
# カメラ画像上の指定座標のブロック有無・色を判定する関数  #
# [(x座標,y座標,色),...]を返す                        #
######################################################
def detectColor(points):
    
    result = [] # 判定結果のリスト
    colorlist = []

    img = cv2.imread("transform.png")

    for (x, y) in points:
        # RGBからHSVへ変換
        pixelValue = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)[y, x]
        
        # ブロック色・有無判定
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

    # 画像をPNGファイルへ保存(座標の調整用途)
    cv2.imwrite('snapshot_{0:%Y%m%d_%H%M%S}.png'.format(datetime.datetime.now()), img)

    # ボーナスナンバー判定
    bonus_num = bonus_num_detect()

    # 左用_経路計算
    # all_route = route_calc.get_all_route(8, 1, colorlist, 0, bonus_num)
    # 右用_経路計算
    all_route = route_calc.get_all_route(11, 3, colorlist, 1, bonus_num)

    print(all_route)
    print(colorlist)

    return all_route


##############
# メイン関数 #
##############
# if __name__ == '__main__':
#     # bluetooth通信用COMポートを開通する
#     si = SerialInterface()
#     si.open(port, baud, None)

#     # 連想リスト(辞書)を作成する
#     dots = createDict()

#     # 本番画像キャプチャ
#     transform.capture_img_for_detect()

#     # 要求待ち受けのループ
#     while True:    
#         try:
#             # クライアントから要求パケットを受信する（受信するまでブロックすることに注意）
#             command, parameter = si.read()
#             print("(command, parameter) = ", (command, parameter))

#             # parameterを解読し色判定対象のピクセル座標のリストを作る
#             points, errmsg = decodePacket(command, parameter)

#             # 色判定結果から応答パケットを作る
#             if points is not None:
#                 # 座標色応答
#                 response, parameter = encodePacket(ResponseCode.Color, detectColor(points))
#             else:
#                 # エラー応答：座標IDからピクセル座標への特定ができなかったケース
#                 response, parameter = encodePacket(ResponseCode.Error, None, errmsg)
#             print("(response, parameter) = ", (response, parameter))

#             # # 応答パケットを送信する
#             # si.write(response, parameter)
#             # param = [配列の長さ, 経路の配列…]
#             # param = [5, 3, 2, 4, 1, 2]
#             param_org = all_route
#             param = [len(param_org)]
#             param.extend(param_org)
#             si.write(ResponseCode.Color, struct.pack('B'*len(param), *param))
#         except KeyboardInterrupt: # キーボード割り込み(Ctrl + C)
#             # COMポートを閉じる
#             si.close()
points, errmsg = decodePacket(CommandCode.All, [])
response, parameter = encodePacket(ResponseCode.Color, detectColor(points))

