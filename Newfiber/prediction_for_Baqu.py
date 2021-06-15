#---------------------------------------------------------------------------
#
#   Project: NewFiber SVM Module
#   Version: 1.0
#   Date:    2020/10/23  (Build 1.0.001)
#   Author:  Qianyu Yang
#

#   remark:
#-----------------------------------------------------------------------------
##############################################################################
#导入库
import pandas as pd
import numpy as np
from SVM import *
import openpyxl
import matplotlib.pyplot as plt
##############################################################################
#函数库
#-----------------------------------------------------------------------------
#训练并保存结果数据函数
def train(X, y, target_name, predict):
    # ----------------------------------------------------------------------------------------------------------------
    X_train, X_test, y_train, y_test = train_test_split(X, y.values.ravel(), test_size=0.2, random_state=0)
    y_traind = y_train * 1.0
    y_testd = y_test * 1.0
    # ----------------------------------------------------------------------------------------------------------------
    model, y_train_result = SVRCOMPIL_Train(X_train, y_traind, 'rbf', 3, 'auto', 0.0, 1e-3, 1.0, 0.1)
    y_test_result = COMPIL_Test(model, X_test)
    result = pd.concat([pd.DataFrame(y_test_result),pd.DataFrame(y_testd)], axis=1)
    # ----------------------------------------------------------------------------------------------------------------
    #保存测试结果以及真实测试集数值到指定路径
    #指定路径需创好正确名字的excel文件
    #pd.DataFrame(result).to_excel('/Users/ethan/Desktop/newfiber/八曲水库/方案管理模版/02工具/02测试结果/' +
    #                              target_name + 't+' + str(predict) + '.xlsx')
    # Save model
    savemodel('model.pickle', model)
    # Load model
    model2 = loadmodel('model.pickle')
    #用于检查模拟效果
    return y_testd, y_test_result

# -----------------------------------------------------------------------------
##############################################################################
# Main
if __name__ == '__main__':
    #读取数据，设定数据所在路径
    dat = pd.read_excel(r'/Users/ethan/Desktop/newfiber/八曲水库/方案管理模版/03原始图资数据/八曲河水位雨量数据/雷达水位站小时水位.xls',
                        sheet_name='ALL')
    dat = dat.drop(dat.index[10845:])
    dat = dat.drop(dat.index[3750:9859])
    dat = dat.drop(dat.index[0:42])
    time = dat[dat.columns[1]]
    dat = dat.drop(columns=dat.columns[0:13])
    dat = dat.drop(columns=dat.columns[8:])

    lvl_ds = dat[dat.columns[0]]
    lvl_ws = dat[dat.columns[1]]
    lvl_sc = dat[dat.columns[2]]
    drp_th = dat[dat.columns[3]]
    drp_gm = dat[dat.columns[4]]
    drp_ds = dat[dat.columns[5]]
    drp_sg = dat[dat.columns[6]]
    drp_sc = dat[dat.columns[7]]

    #预报时间
    predict = 1

    #预报目标监测站以及其他监测站设定
    target = lvl_sc
    target_name = "三岔河桥"
    other1 = lvl_ws
    other2 = lvl_ds

    #创建输入项
    X = pd.concat([target[12:-predict].reset_index(drop=True),#预报测站当前、前1小时、前3小时水位
                   target[11:-(predict+1)].reset_index(drop=True),
                   target[9:-(predict+3)].reset_index(drop=True),

                   drp_gm[12:-predict].reset_index(drop=True),#各测站当前雨量
                   drp_th[12:-predict].reset_index(drop=True),
                   drp_sg[12:-predict].reset_index(drop=True),
                   drp_sc[12:-predict].reset_index(drop=True),

                   other1[9:-(predict+3)].reset_index(drop=True),#其他测站当前、前3、6、9、12小时水位
                   other1[6:-(predict+6)].reset_index(drop=True),
                   other1[3:-(predict+9)].reset_index(drop=True),
                   other1[0:-(predict+12)].reset_index(drop=True),

                   other2[9:-(predict+3)].reset_index(drop=True),
                   other2[6:-(predict+6)].reset_index(drop=True),
                   other2[3:-(predict+9)].reset_index(drop=True),
                   other2[0:-(predict+12)].reset_index(drop=True)], axis=1)

    #创建输出项
    y = lvl_ds[predict+12:]

    #训练并倒出结果
    y_testd, y_test_result = train(X, y, target_name, predict)

    #检查相关系数
    #print(np.corrcoef(y_testd.astype(float), y_test_result.astype(float)))

    time = time[predict+12:]
    time_train, time_test = train_test_split(time,test_size=0.2, random_state=0)
    plt.plot(time_test, y_testd, 'b-')
    plt.show()
