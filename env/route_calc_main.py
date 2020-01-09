import route_calc

block_setting_list = [4, 0, 2, 0, 0, 1, 0, 1, 3, 2, 4, 0, 0, 5, 0, 0]
# 左用
# all_route = route_calc.get_all_route(8, 1, block_setting_list, 0, 1)
# 右用
all_route = route_calc.get_all_route(11, 3, block_setting_list, 1, 1)
print("all_route:" + str(all_route))
