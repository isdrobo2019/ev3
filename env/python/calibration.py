import time
import cv2
import numpy as np

class mouseParam:
    def __init__(self, input_img_name):
        #マウス入力用のパラメータ
        self.mouseEvent = {"x":None, "y":None, "event":None, "flags":None}
        #マウス入力の設定
        cv2.setMouseCallback(input_img_name, self.__CallBackFunc, None)
    
    #コールバック関数
    def __CallBackFunc(self, eventType, x, y, flags, userdata):
        
        self.mouseEvent["x"] = x
        self.mouseEvent["y"] = y
        self.mouseEvent["event"] = eventType    
        self.mouseEvent["flags"] = flags    

    #マウス入力用のパラメータを返すための関数
    def getData(self):
        return self.mouseEvent
    
    #マウスイベントを返す関数
    def getEvent(self):
        return self.mouseEvent["event"]                

    #マウスフラグを返す関数
    def getFlags(self):
        return self.mouseEvent["flags"]                

    #xの座標を返す関数
    def getX(self):
        return self.mouseEvent["x"]  

    #yの座標を返す関数
    def getY(self):
        return self.mouseEvent["y"]  

    #xとyの座標を返す関数
    def getPos(self):
        return (self.mouseEvent["x"], self.mouseEvent["y"])
        

filename = "image.png"
if __name__ == "__main__":
    
    while 1:
        #入力画像
        org = cv2.imread(filename)
        read = org.copy()
        
        #表示するWindow名
        window_name = "input window"
        
        #画像の表示
        cv2.imshow(window_name, read)
        
        #コールバックの設定
        mouseData = mouseParam(window_name)

        points = []
        imgs = []

        while 1:
            key = cv2.waitKey(20)
            if key == 27:
                break
            #左クリックがあったら表示
            if mouseData.getEvent() == cv2.EVENT_LBUTTONDOWN:
                if len(points) > 3:
                    break
                cv2.circle(read, (mouseData.getPos()[0], mouseData.getPos()[1]), 10, color=(0, 255, 255), thickness=-1)
                if len(points) > 0:
                    cv2.line(read, (points[len(points)-1][0], points[len(points)-1][1]), (mouseData.getPos()[0], mouseData.getPos()[1]), (0, 255, 255), thickness=2, lineType=cv2.LINE_8)
                    if len(points) == 3:
                        cv2.line(read, (points[0][0], points[0][1]), (mouseData.getPos()[0], mouseData.getPos()[1]), (0, 255, 255), thickness=2, lineType=cv2.LINE_8)
                cv2.imshow(window_name, read)
                points.append(mouseData.getPos())
                imgs.append(read.copy())
                time.sleep(0.2)
            #右クリックがあったら終了
            elif mouseData.getEvent() == cv2.EVENT_RBUTTONDOWN:
                if len(points) == 1:
                    del points[0]
                    del imgs[0]
                    read = org.copy()
                elif len(points) > 1:
                    del points[len(points)-1]
                    del imgs[len(imgs)-1]
                    read = imgs[len(points)-1]
                cv2.imshow(window_name, read)
                time.sleep(0.2)

        if len(points) > 3:
            sorts = []
            for point in points:
                sum = point[0] + point[1]
                rank = 0
                for sort in sorts:
                    if sum < sort[0] + sort[1]:
                        rank += 1
                sorts.insert(rank, point)
            path = 'point_list.txt'
            with open(path, mode='w') as f:
                f.write(str(sorts[1][0]) + "," + str(sorts[1][1]))
                f.write("\n")
                f.write(str(sorts[2][0]) + "," + str(sorts[2][1]))
                f.write("\n")
                f.write(str(sorts[3][0]) + "," + str(sorts[3][1]))
                f.write("\n")
                f.write(str(sorts[0][0]) + "," + str(sorts[0][1]))

            # lf = ""
            # with open(path, mode='w') as f:
            #     for point in points:
            #         f.write(lf)
            #         f.write(str(point[0]) + "," + str(point[1]))
            #         lf = "\n"

            cv2.destroyAllWindows()            
            print("Complete")
            
            # 画像の読み込み
            img = org.copy()
            # cv2.imshow(windowname, img)

            # 白黒画像で画像を読み込み
            # gray = cv2.imread(filename_before,0)
            gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)


            ret,th1 = cv2.threshold(gray,130,255,cv2.THRESH_BINARY)

            contours, hierarchy = cv2.findContours(th1, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

            dst = []

            path = 'point_list.txt'
            ptss = []
            with open(path) as f:
                lines = f.read().split("\n")
                for line in lines:
                    sp = line.split(",")
                    ptss.append([int(sp[0]), int(sp[1])])
            pts1 = np.float32([ptss[0],ptss[1],ptss[2],ptss[3]])

            pts2 = np.float32([[600,300],[600,0],[0,0],[0,300]])

            M = cv2.getPerspectiveTransform(pts1,pts2)
            dst = cv2.warpPerspective(img,M,(600,300))

            # 読み込んだ画像の高さと幅を取得
            height = dst.shape[0]
            width = dst.shape[1]

            # 画像のサイズを変更
            # 第一引数：サイズを変更する画像
            # 第二引数：変更後の幅
            # 第三引数：変更後の高さ
            resized_img = cv2.resize(dst,(width*1, height*2))

            cv2.imshow("change", resized_img)
            key = cv2.waitKey()
            if key != 27:
                cv2.destroyAllWindows()
                break
        else:
            break
            cv2.destroyAllWindows()            
            print("NotComplete")
