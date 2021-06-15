import json
import jsonpath as jp
import requests
import re


def conditionCheck(condition):
    for cond in condition:
        if not cond[0]:
            return False
    return True


if __name__ == '__main__':
    # 交易信号api
    signalRecords = 'http://10.10.10.131/api/strategy-manager/private/client/quantitativeReport/tradingSignal.json' \
                    '?reportId=3988 '
    header = {'Access-token': '0123456789'}
    req = requests.get(signalRecords, headers=header)
    signalRecords = req.json()
    # 交易信号合集
    signalPoints = signalRecords['content']
    output = {'reportId': signalPoints[0]["reportId"], 'instrumentId': signalPoints[0]["instrumentId"], 'content': []}

    # 交易数据参数，time起始时间，size提取数量（循环周期），id产品id
    time = '2020-12-01 00:00:00.000'
    size = 50
    id = 3988
    prefix = 'http://10.10.10.131/api/strategy-manager/private/client/measurement/details.json?id='

    j = 0
    buy = False
    while True:
        # 交易数据，size为循环周期
        tradingRecords = prefix + str(id) + '&time=' + str(time) + '&size=' + str(size)
        req = requests.get(tradingRecords, headers=header)
        tradingRecords = req.json()
        points = tradingRecords['content']

        i = 0
        # signal to actions
        num = len(points)
        if num != size:
            n = num
        else:
            n = num-1

        while i < n:
            point = points[i]
            if point[0][:-4] == signalPoints[j]['tradingSignalTime']:
                conditions = point[2]
                if not buy:
                    for key, buyCheck in conditions.items():
                        if 'BUY_OPEN' in key:
                            b = conditionCheck(buyCheck)
                            if b:
                                buy = True
                                point.append(signalPoints[j]["avgPrice"])
                                point.append("BUY")
                                output['content'].append(point)
                                j += 1
                else:
                    for key, sellCheck in conditions.items():
                        if 'SELL_CLOSE' in key:
                            b = conditionCheck(sellCheck)
                            if b:
                                buy = False
                                point.append(signalPoints[j]["avgPrice"])
                                point.append("SELL")
                                output['content'].append(point)
                                j += 1
                                break
            i += 1
            if j == len(signalPoints):
                break
        if j == len(signalPoints):
            break
            # if i == 100:
            #     break
        if num != size:
            break
        else:
            time = points[i][0]
        print(time)
        print(len(output['content']))

    # print(output)
    with open('tradingRecords.json', 'w') as outfile:
        json.dump(output, outfile, indent=4, ensure_ascii=False)
