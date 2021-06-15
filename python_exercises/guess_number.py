#-----------------------------------------------------------------------------
#
#   Project: Number Guessing
#   Version: 1.0
#   Date:    2020/09/02  (Build 1.0.001)
#   Author:  Qianyu Yang
#

#   remark:
#-----------------------------------------------------------------------------
##############################################################################
#导入库
import random
##############################################################################
#
#函数库
#-----------------------------------------------------------------------------
#产生随机数
def random_nums():  # 生成随机四位数
    num_list = range(0, 9)
    ran_nums = random.sample(num_list, 4)
    sys_nums = ''
    for i in range(4):
        sys_nums = sys_nums + str(ran_nums[i])
    return sys_nums
#-----------------------------------------------------------------------------
#判断一个四位数是否是所给数
def guess(secret):
    while True:
        guess = input("please type in your guess:")
        a = 0 #数位均正确
        b = 0 #数字正确但位置不正确
        for i in range(len(guess)):
            if secret[i] == guess[i]:
                a = a + 1
            elif guess[i] in secret:
               b = b + 1
        if a == len(secret):
            print("Correct!")
            break
        else:
            print(str(a)+"A"+str(b)+"B")
#-----------------------------------------------------------------------------
##############################################################################
#Main and example
if __name__ == '__main__':
    secret = random_nums()
    guess(secret)