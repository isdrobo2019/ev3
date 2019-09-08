import math

# route_list = [[0, 50, 80, 0, 0], [0, 0, 20, 15, 0 ], [0, 0, 0, 10, 15], [0, 0, 0, 0, 30], [0, 0, 0, 0, 0]] # 初期のノード間の距離のリスト
route_list = [[1, 4],[0, 2, 5],[1, 3, 6],[2, 7],[0, 5, 8],[1, 4, 6, 9],[2, 5, 7, 10],[3, 6, 11],[4, 9, 12],[5, 8, 10, 13],[6, 9, 11, 14],[7, 10, 15],[8, 13],[9, 12, 14],[10, 13, 15],[11, 14]]


def calc(start_idx, end_idx):
    idx = 1
    for route in route_list[start_idx]:
        print("start_idx:" +start_idx + "/end_idx:" + end_idx ; "=" + str(idx) + ":" + str(route))
        calc(8, 13)
        idx += 1

calc(8, 13)
