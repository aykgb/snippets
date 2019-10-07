import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import baostock as bs
import tushare as ts
from datetime import datetime, timedelta
from analysis_stock_calculate import calculate
from analysis_stock_transaction import Transaction


def getHistData(stock: str, name: str):
    pass


date_focus = '2018-11-30'
stocks = {
    'sh.600036': "招商银行",
    'sh.600196': "复星医药",
    'sh.601607': "上海医药",
    'sz.000002': "万 科Ａ"
}

Trans = {
    'sh.600036': [Transaction('sh.600036', "招商银行")],
    'sh.600196': [Transaction('sh.600196', "复星医药")],
    'sh.601607': [Transaction('sz.002415', "上海医药")],
    'sz.000002': [Transaction('sz.000002', "万 科Ａ")]
}

# 'sh.600036': "招商银行",
Trans['sh.600036'][-1].trade('2018-08-06', 27.900, 300)

# 'sh.600196': "复星医药",
Trans['sh.600196'][-1].trade('2018-08-31', 29.700, 100)

# 'sh.601607': "上海医药",
Trans['sh.601607'][-1].trade('2018-05-23', 26.163, 200)

# 'sz.000002': "万 科Ａ"
Trans['sz.000002'][-1].trade('2018-06-21', 27.649, 200)


for stock in stocks:
    price_latest, price_close_yest = \
        calculate(stock, stocks[stock], date_focus, False)

    # time_str = datetime.now().strftime('%Y-%m-%d_%H.%M')
    # fname = stock.replace('.', '') + '_' + \
    #     stocks[stock] + '_' + time_str + '.md'
    # reports = open(fname, 'w+')
    # reports.close()
    trans = Trans[stock][-1]
    print('------------> Reports <------------')
    print('昨日收盘价：', price_close_yest)
    print('总成本：', trans.cost())
    print('成本价：', trans.cost_price())
    print('+%1.7目标价：', trans.cost_price() * 1.017)
    print('+%5.7目标价：', trans.cost_price() * 1.057)
    print('+%9.7目标价：', trans.cost_price() * 1.097)
    print('最新价：', price_latest)
    print('比昨日收盘涨： %%%.2f' %
          ((price_latest - price_close_yest) / price_close_yest * 100))
    print('盈亏：%d/%%%.2f' % (trans.profit(price_latest),
                            trans.rate(price_latest)))
    print('-----------------------------------\n')
