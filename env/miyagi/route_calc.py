import math
import copy

# ルート定義リスト
route_list = [[1, 4],[0, 2, 5],[1, 3, 6],[2, 7],[0, 5, 8],[1, 4, 6, 9],[2, 5, 7, 10],[3, 6, 11],[4, 9, 12],[5, 8, 10, 13],[6, 9, 11, 14],[7, 10, 15],[8, 13],[9, 12, 14],[10, 13, 15],[11, 14]]

# ブロックサークル色リスト(0:左コース, 1:右コース)
block_circle_list = [
    [4, 2, 1, 3, 0, 4, 2, 1, 3],
    [1, 2, 4, 4, 0, 3, 3, 1, 2]
]

# 全てのブロックを移動する際の最適なルート探索を行う(cource=0：右、1：左)
def get_all_route(start_idx, direction, block_setting_list_org, cource, bonus_num):
    result = []
    loop_cnt = 0
    # 本体の向き(参照渡し用に配列として定義)
    direction_ref = [direction]
    # 
    block_setting_list_in = copy.copy(block_setting_list_org)
    block_circle_list_in = copy.copy(block_circle_list[cource])
    # ブロック単位でループ
    while 1:
        # 現状の最適ルート検索
        best_route = get_best_route(start_idx, block_setting_list_in, block_circle_list_in, bonus_num)
        # print("best_route:" + str(best_route))
        # 最適ルートが見つからない場合(詰んでる場合)は終了
        if len(best_route) == 0:
            break
        # ルート変換
        conv = convert_root(best_route, direction_ref)
        # 戻り値に追加
        result.extend(conv)
        # 移動したブロックをブロック配置リストから消す
        block_setting_list_in[best_route[1][0]] = 0
        # print("block_setting_list_in:" + str(block_setting_list_in))
        # 開始位置を更新
        start_idx = best_route[1][1]
        # 残りのブロック数を数える
        block_cnt = 0
        for block_setting in block_setting_list_in:
            block_cnt += block_setting
        # ブロック数が0になったら終了
        if block_cnt == 0:
            break
        # ループ回数を追加
        loop_cnt += 1
        # ループ回数が10回を超えたら終了
        if loop_cnt > 10:
            break
    # 結果を返却
    return result

# 1つのブロックを移動する際の最適ルートを探索する
def get_best_route(start_idx, block_setting_list_in, block_circle_list_in, bonus_num):
    best_route = []
    # ブロック-移動先の全パターンを取得してループ
    set_move_list = get_set_move_pattern(block_setting_list_in, block_circle_list_in, bonus_num)
    for set_move in set_move_list:
        result = []
        # print("set_move:" + str(set_move))
        # 開始位置とブロック位置が一緒なら開始位置-ブロック位置の経路探索はしない(あるとしたらエリア開始直後くらい？)
        if start_idx != set_move[0]:
            # 開始位置-ブロック位置の経路探索
            now_block = calc_start(start_idx, set_move[0], block_setting_list_in, True)
            # print("now_block:" + str(now_block))
            # 結果なしの場合処理しない
            if len(now_block) == 0:
                continue
            else:
                # 結果ありの場合は最適ルートに追加
                result.extend(now_block)
        # ブロック位置と設置先が一緒ならブロック位置-設置先の経路探索はしない
        if set_move[0] != set_move[1]:
            # ブロック位置-設置先の経路探索
            block_end = calc_start(set_move[0], set_move[1], block_setting_list_in, False)
            # print("block_end:" + str(block_end))
            # 結果なしの場合処理しない
            if len(block_end) == 0:
                continue
            else:
                # 開始位置-ブロック位置の移動がある場合は先頭を除去
                if len(result) > 0:
                    del block_end[0]
                # 最適ルートに追加
                result.extend(block_end)
        # これまでの最適ルートより距離が短い場合は最適ルートを更新
        if len(result) != 0 and (len(best_route) == 0 or len(best_route[0]) > len(result)):
            # print("set_move:" + str(set_move))
            # print("now_block:" + str(now_block))
            # print("block_end:" + str(block_end))
            # print("result:" + str(result))
            best_route = [result, set_move]
    # 結果を返す
    return best_route

# ブロック配置地点-ブロック移動地点のパターンを取得
def get_set_move_pattern(block_setting_list_in, block_circle_list_in, bonus_num):
    set_move_list = []
    # ブロックを全てループ
    for b_idx, block in enumerate(block_setting_list_in):
        # 黒ブロックの場合
        if block == 5:
            # ボーナスナンバーの周りの交点サークルを取得
            endpoints = get_around_circle(bonus_num)
            # ブロックと目的地の交点サークルのパターンを登録
            set_move_list.append([b_idx, endpoints[0], 2])
            set_move_list.append([b_idx, endpoints[1], 3])
            set_move_list.append([b_idx, endpoints[2], 1])
            set_move_list.append([b_idx, endpoints[3], 0])
        # 黒以外のブロックの場合
        elif block >= 1 and block <= 4:
            # ブロックサークルを全てループ
            for c_idx, circle in enumerate(block_circle_list_in):
                # ブロックの色とブロックサークルの色が一致している場合
                if block == circle:
                    # ブロックサークルの周りの交点サークルを取得
                    endpoints = get_around_circle(c_idx)
                    # ブロックと目的地の交点サークルのパターンを登録
                    set_move_list.append([b_idx, endpoints[0], 2])
                    set_move_list.append([b_idx, endpoints[1], 3])
                    set_move_list.append([b_idx, endpoints[2], 1])
                    set_move_list.append([b_idx, endpoints[3], 0])
    # 結果を返す
    return set_move_list

# ブロックサークルの周りの交点サークルを取得
def get_around_circle(index):
    add_val = math.floor(index / 3)
    return [add_val + index, add_val + index + 1, add_val + index + 4, add_val + index + 5]

# ルート計算(開始用)
def calc_start(start_idx, end_idx, block_setting_list_in, is_block):
    return calc(start_idx, end_idx, block_setting_list_in, [start_idx], [[]], is_block)[0]

# ルート計算(再帰用)
def calc(start_idx, end_idx, block_setting_list_in, now_root, ans_root, is_block):
    # 今いるサークルから移動できるルートを全て探索
    for route in route_list[start_idx]:
        # 別ルートの探索で参照しないようにオブジェクトコピー
        cp_now_root = copy.copy(now_root)
        # これまでの経路に今いる交点サークルが含まれている場合は処理しない
        if route in now_root:
            continue
        # ブロックが置かれている場合は処理しない(目的地がブロックで目的地である場合を除く)
        elif block_setting_list_in[route] != 0 and not (is_block and route == end_idx):
            continue
        else:
            # ルートに追加
            cp_now_root.append(route)
            # 今いる交点サークルが目標地点の場合は終了
            if route == end_idx:
                # 今回のルートが最短の場合は正解ルートとして設定
                if len(ans_root[0]) == 0 or len(ans_root[0]) > len(cp_now_root):
                    ans_root[0] = cp_now_root
            else:
                # 今いる交点サークルから再度ルート計算処理を呼び出す
                calc(route, end_idx, block_setting_list_in, copy.copy(cp_now_root), ans_root, is_block)
    # 全てのルートの探索が完了したら正解ルートを返す
    return ans_root

# ルートをロボットが認識できる形に変換
def convert_root(best_route, direction_in):
    result = []
    direction = direction_in[0]
    # ルートをループ
    for idx, point in enumerate(best_route[0]):
        # 最初と最後は処理しない
        if idx > 0 and idx < len(best_route[0]):
            # 前のサークルからの位置を判定
            sub = point - best_route[0][idx - 1]
            angle = 0
            # 上
            if sub == -4:
                angle = 0
            # 右
            elif sub == 1:
                angle = 1
            # 下
            elif sub == 4:
                angle = 2
            # 左
            elif sub == -1:
                angle = 3
            # 今の方向から次のサークルに移動するための移動方法を判定して結果に追加
            diff = direction - angle
            # まっすぐ
            if diff == 0:
                result.append(0)
            # 右
            elif diff == -1 or diff == 3:
                result.append(1)
            # 反転
            elif abs(diff) % 2 == 0:
                result.append(2)
            # 左
            elif diff == 1 or diff == -3:
                result.append(3)
            direction = angle
    # ブロックサークルに対してブロックを置きに行く方向を設定
    put_angle = best_route[1][2] - direction + 4
    if put_angle < 4:
        put_angle += 4
    result.append(put_angle)

    # 呼び出し元の方向を変える
    direction_in[0] = direction

    # 結果を返却
    return result


# デバッグコード
# ブロック配置         0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
# block_setting_list = [4, 0, 2, 0, 0, 1, 0, 1, 3, 2, 4, 0, 0, 5, 0, 0]
# all_route = get_all_route(11, 3, block_setting_list, block_circle_list[1], 1)
# print("all_route:" + str(all_route))

# best_route = get_best_route(14, [4, 0, 2, 0, 0, 1, 0, 0, 3, 2, 4, 0, 0, 5, 0, 0], block_circle_list_cp, 1)
# print("best_route:" + str(best_route))


# result = calc_start(11, 13)
# print("result:" + str(result))
# デバッグコード
