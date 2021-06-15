import json
import jsonpath as jp
import requests
import datetime
import re

def getApi(path):
    header = {'Access-token': '0123456789'}
    req = requests.get(path, headers=header)
    data = req.json()
    return data

def compareTime(t1, t2):
    t1 = datetime.datetime.strptime(t1, '%Y-%m-%d %H:%M:%S.%f')
    t2 = datetime.datetime.strptime(t2, '%Y-%m-%d %H:%M:%S.%f')
    return t1 > t2

def cal(b, s):
    return s["avgPrice"] - b["avgPrice"]

# 计算一组交易信号总收益
def rate(records):
    total = 0
    start = 1
    for i in range(len(records)-1):
        if records[i]["openClose"] == "OPEN":
            if start == 1:
                start = records[i]['avgPrice']
            if records[i]["buySell"] == "BUY":
                total += cal(records[i], records[i+1])
            else:
                total += cal(records[i+1], records[i])
    return 100 * total/start

def sort(results):
    p = 0
    while(p<len(results)-1):
        if(results[p]['interest']<results[p+1]['interest']):
            results[p]['interest'],results[p+1]['interest'] = results[p+1]['interest'],results[p]['interest']
            if(not(p==0)):
                p = p-1
        else:
            p += 1
    return results


def merge(r1, r2):
    def switch(act):
        if act == "BUY":
            return "SELL"
        else:
            return "BUY"
    result = []
    i1 = i2 = 0
    action = "BUY"  # buy
    while i1 < len(r1) or i2 < len(r2):
        if i1 == len(r1) and i2 < len(r2):
            if r2[i2]["buySell"] == action:
                result += r2[i2:]
            else:
                result += r2[i2+1:]
            break
        elif i2 == len(r2) and i1 < len(r1):
            if r1[i1]["buySell"] == action:
                result += r1[i1:]
            else:
                result += r1[i1+1:]
            break
        else:
            if r1[i1]["buySell"] == action and r2[i2]["buySell"] == action:
                if compareTime(r1[i1]["signalTime"], r2[i2]["signalTime"]):
                    result.append(r2[i2])
                    i2 += 1
                else:
                    result.append(r1[i1])
                    i1 += 1
            elif r1[i1]["buySell"] == action:
                result.append(r1[i1])
                if compareTime(r1[i1]["signalTime"], r2[i2]["signalTime"]):
                    i2 += 1
                i1 += 1
            else:
                result.append(r2[i2])
                if not compareTime(r1[i1]["signalTime"], r2[i2]["signalTime"]):
                    i1 += 1
                i2 += 1
            action = switch(action)
    return result


if __name__ == '__main__':
    #   filePath = "/Users/ethan/Desktop/pv detection/jsonAnalysis/"
    #   with open(filePath+"testOutput.json") as f:
    #      test = json.load(f)
    # # # filePath = "/Users/ethan/Desktop/pv detection/jsonAnalysis/"
    # # # with open(filePath+"sampleTest2.json") as f:
    # # #     sampleTrading2 = json.load(f)
    # # # result = merge(sampleTrading2, sampleTrading3)
    #   sort(test)
    #   with open(filePath + 'testOutputSorted.json', 'w') as outfile:
    #       json.dump(test, outfile, indent=4, ensure_ascii=False)
    # #

    allTests1 = 'http://10.10.10.131/api/strategy-manager/private/strategyOptimization/findAllByMember.json?pageNum=1&pageSize=1&id=152&isReturnSnapshot=true'
    allTests2 = 'http://10.10.10.131/api/strategy-manager/private/strategyOptimization/findAllByMember.json?pageNum=1&pageSize=1&id=153&isReturnSnapshot=true'
    # allTests3 = 'http://10.10.11.11/api/strategy-manager/private/strategyOptimization/findAllByMember.json?pageNum=1&pageSize=1&id=106&isReturnSnapshot=true'
    allTests1 = getApi(allTests1)
    allTests2 = getApi(allTests2)
    # allTests3 = getApi(allTests3)

    allTestsList1 = allTests1["content"]["list"][0]["snapshotList"]
    allTestsList2 = allTests2["content"]["list"][0]["snapshotList"]
    # allTestsList3 = allTests3["content"]["list"][0]["snapshotList"]
    prefix = "http://10.10.10.131/api/strategy-manager/private/strategyExecutionOrdersSignal/list.json?snapshotId="

    results = []

    # r1 = getApi(prefix + str(6508))
    # print(rate(r1))


    for t1 in allTestsList1:
        for t2 in allTestsList2:
            # for t3 in allTestsList3:
            r1 = getApi(prefix+str(t1['id']))
            r2 = getApi(prefix+str(t2['id']))
            # r3 = getApi(prefix + str(t3['id']))
            mergeTest = merge(r1, r2)
            # mergeTest = mergeTest(rtmp, r3)
            interest = rate(mergeTest)
            # result = {'interest': interest, 'id1': t1['id'], 'id2': t2['id']}
            result = {'interest': interest, 'id1': t1['id'], 'id2': t2['id']}
            print(t1['id'], t2['id'])
            results.append(result)

    filePath = "/Users/ethan/Desktop/pv detection/jsonAnalysis/"
    sort(results)
    with open(filePath + 'testOutputSorted.json', 'w') as outfile:
        json.dump(results, outfile, indent=4, ensure_ascii=False)
