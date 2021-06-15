import requests
import json
import re

tradingRecords = 'http://10.10.10.131/api/strategy-manager/private/client/quantitativeReport/tradingSignal.json' \
                    '?reportId=3988 '

fileName = 'getSignal'
header = {'Access-token': '0123456789'}
print(tradingRecords)
req = requests.get(tradingRecords, headers=header)

dat = req.json()

# print(dat)

with open(fileName+'.json', 'w') as outfile:
    json.dump(dat, outfile, indent=4, ensure_ascii=False)

