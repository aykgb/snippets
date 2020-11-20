import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import baostock as bs
from datetime import datetime, timedelta


lg = bs.login()
# plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
# plt.rcParams['axes.unicode_minus'] = False  # 用来正常显示负号


def calculate(stock='sh.601398', name='工商银行', date_focus='2018-09-28',
              is_sim=False):
    time_now = datetime.now()
    # 获取历史90日K线数据（不包含当日） 和 当日5分钟K线数据（用来模拟当日的实时股价）
    date_beg = (datetime.strptime(date_focus, '%Y-%m-%d') -
                timedelta(days=90)).strftime('%Y-%m-%d')
    date_end = (datetime.strptime(date_focus, '%Y-%m-%d') -
                timedelta(days=1)).strftime('%Y-%m-%d')
    rs_days = \
        bs.query_history_k_data(stock,
                                "date,open,close,high,low,volume,amount,turn",
                                start_date=date_beg,
                                end_date=date_end,
                                frequency="d")
    rs_5min = \
        bs.query_history_k_data(stock,
                                "date,time,open,close,high,low,volume,amount",
                                start_date=date_focus,
                                end_date=date_focus,
                                frequency="5")
    data_days = rs_days.get_data()
    data_5min = rs_5min.get_data()
    if len(data_5min) < 3:
        print('too few 5 minutes data point')
        return

    # 计算历史日K的ma3/ma5/ma3-ma5
    data_days['ma3'] = np.round(
        pd.Series.rolling(data_days['close'], 3).mean(), 2)
    data_days['ma5'] = np.round(
        pd.Series.rolling(data_days['close'], 5).mean(), 2)
    data_days['ma3-ma5'] = data_days['ma3'] - data_days['ma5']

    # 根据当前的5分钟数据，计算当日的实时 ma3/ma5/ma3-ma5
    # -------------------------------------------
    # |          |   0   |   1   |  ...  |   47 | <-- 当日各个5分钟的点
    # -------------------------------------------
    # | ...      |  ...  |          ...         |
    # | 20180827 | 27.01 |          ...         |
    # | ...      |  ...  |          ...         | <-- 以上都是历史ma3(or ma5/ma3-ma5)
    # | 20180928 | 25.21 |          ...         | <-- 以当日的第i个5分钟close price
    # -------------------------------------------       计算出的ma3(or ma5/ma3-ma5)
    data_realtime_ma3_all = pd.DataFrame()
    data_realtime_ma5_all = pd.DataFrame()
    data_realtime_ma3_ma5_all = pd.DataFrame()
    data_realtime = pd.DataFrame()
    data_realtime['date'] = data_days['date']
    data_realtime['close'] = data_days['close']
    data_realtime.loc[len(data_realtime)] = [date_focus, 0]
    num_of_5min_point = 0
    for price in data_5min['close']:
        # print(i, price)
        # 将当日的close price设置为当前的5分钟close price
        data_realtime.loc[len(data_realtime) - 1, 'close'] = price
        data_realtime['ma3'] = np.round(
            pd.Series.rolling(data_realtime['close'], 3).mean(), 2)
        data_realtime['ma5'] = np.round(
            pd.Series.rolling(data_realtime['close'], 5).mean(), 2)
        data_realtime_ma3_all[num_of_5min_point] = data_realtime['ma3']
        data_realtime_ma5_all[num_of_5min_point] = data_realtime['ma5']
        data_realtime_ma3_ma5_all[num_of_5min_point] = \
            data_realtime['ma3'] - data_realtime['ma5']
        num_of_5min_point += 1
        # data[['ma3', 'ma5']].plot(
        # marker='o',xticks=[18, 19, 20, 21, 22],xlim=(18,23))

    # 绘制数据(取最近5天的数据)。
    dates_str = [date for date in data_realtime['date'].values]
    # dates = [datetime.strptime(date, '%Y-%m-%d')
    #          for date in data['date'].values]
    plot_x = dates_str[len(dates_str) - 5: len(dates_str)]
    plot_y = pd.to_numeric(
        data_realtime['close'].loc[len(data_realtime)-5:len(data_realtime)],
        downcast='float')
    plot_y_ma3 = data_realtime_ma3_all.loc[len(data_realtime_ma3_all)-5:
                                           len(data_realtime_ma3_all)]
    plot_y_ma5 = data_realtime_ma5_all.loc[len(data_realtime_ma5_all)-5:
                                           len(data_realtime_ma5_all)]
    plot_y_ma3_ma5 = \
        data_realtime_ma3_ma5_all.loc[len(data_realtime_ma3_ma5_all)-5:
                                      len(data_realtime_ma3_ma5_all)]
    plt.figure(figsize=(5, 8))
    plt.subplot(211)
    plt.title(stock)
    if (is_sim):  # 绘制回测图
        plt.plot(plot_x, plot_y_ma3[36], marker='o',
                 label='36_ma3', color='#943126')
        plt.plot(plot_x, plot_y_ma3[35], marker='o',
                 label='35_ma3', color='#CB4335')
        plt.plot(plot_x, plot_y_ma3[34], marker='o',
                 label='34_ma3', color='#EC7063')
        plt.plot(plot_x, plot_y_ma5[36], marker='o',
                 label='36_ma5', color='#1A5276')
        plt.plot(plot_x, plot_y_ma5[35], marker='o',
                 label='35_ma5', color='#2980B9')
        plt.plot(plot_x, plot_y_ma5[34], marker='o',
                 label='34_ma5', color='#7FB3D5')
        plt.plot(plot_x, plot_y, marker='o', label='close', color='#000000')
    else:  # 绘制实际使用图像】
        time_delta_5min = timedelta(minutes=5)
        time_market_open = datetime(
            time_now.year, time_now.month, time_now.day, 9, 30, 0)
        colors_ma3 = ['#943126', '#CB4335', '#EC7063']
        colors_ma5 = ['#1A5276', '#2980B9', '#7FB3D5']
        for i in range(3):
            index = plot_y_ma3.columns.size - i - 1
            label_ma3 = ''
            label_ma5 = ''
            if index < 24:
                label_ma3 = (time_market_open + time_delta_5min *
                             (index + 1)).strftime('%H:%M') + '_ma3'
                label_ma5 = (time_market_open + time_delta_5min *
                             (index + 1)).strftime('%H:%M') + '_ma5'
            else:
                label_ma3 = (time_market_open + time_delta_5min *
                             (index + 1 + 18)).strftime('%H:%M') + '_ma3'
                label_ma5 = (time_market_open + time_delta_5min *
                             (index + 1 + 18)).strftime('%H:%M') + '_ma5'
            plt.plot(plot_x, plot_y_ma3[index], marker='o',
                     label=label_ma3, color=colors_ma3[i])
            plt.plot(plot_x, plot_y_ma5[index], marker='o',
                     label=label_ma5, color=colors_ma5[i])
        plt.plot(plot_x, plot_y, marker='o', label='close', color='#000000')

    plt.gcf().autofmt_xdate()
    plt.legend(loc='upper left')

    plt.subplot(212)
    plt.title(stock)
    if (is_sim):  # 回测图
        plt.plot(plot_x, plot_y_ma3_ma5[36],
                 marker='o', label='36', color='#A04000')
        plt.plot(plot_x, plot_y_ma3_ma5[35],
                 marker='o', label='35', color='#D35400')
        plt.plot(plot_x, plot_y_ma3_ma5[34],
                 marker='o', label='34', color='#E59866')
        plt.plot(plot_x, plot_y_ma3_ma5[10],
                 marker='o', label='10', color='#148F77')
        plt.plot(plot_x, plot_y_ma3_ma5[9],
                 marker='o', label='9', color='#1ABC9C')
        plt.plot(plot_x, plot_y_ma3_ma5[8],
                 marker='o', label='8', color='#76D7C4')
    else:  # 绘制实际使用图
        time_now = datetime.now()
        time_delta_5min = timedelta(minutes=5)
        time_market_open = datetime(
            time_now.year, time_now.month, time_now.day, 9, 30, 0)
        colors = ['#A04000', '#D35400', '#E59866']
        for i in range(3):
            index = plot_y_ma3.columns.size - i - 1
            if index < 24:
                label_str = (time_market_open + time_delta_5min *
                             (index + 1)).strftime('%H:%M') + '_ma3_ma5'
            else:
                label_str = (time_market_open + time_delta_5min *
                             (index + 1 + 18)).strftime('%H:%M') + '_ma3_ma5'

            plt.plot(
                plot_x, plot_y_ma3_ma5[index], marker='o', label=label_str,
                color=colors[i])

    plt.gcf().autofmt_xdate()
    plt.legend(loc='upper left')
    time_str = time_now.strftime('%Y-%m-%d_%H.%M')
    plt.savefig(stock.replace('.', '') + '_' +
                name + '_' + time_str + '.png', format='png', dpi=400)

    # 生成报告
    print('历史数据', date_beg, ' 至 ', date_end)
    data_days['close-open'] = \
        pd.to_numeric(data_days['close'], downcast='float') - \
        pd.to_numeric(data_days['open'], downcast='float')
    index_max = data_days['close-open'].idxmax()
    print(name, ' code:', stock)
    print("最近90天最大涨幅：%s, %%%.2f" %
          (data_days['date'][index_max],
           data_days['close-open'][index_max] /
           float(data_days['open'][index_max]) * 100))
    index_min = data_days['close-open'].idxmin()
    print('最近90天最大跌幅：%s, %%%.2f' %
          (data_days['date'][index_min],
           data_days['close-open'][index_min] /
           float(data_days['open'][index_min]) * 100))

    data_latest_13days = pd.DataFrame()
    data_latest_13days['date'] = data_days['date'].loc[len(
        data_days)-13: len(data_days)]
    data_latest_13days['open'] = data_days['open'].loc[len(
        data_days)-13: len(data_days)]
    data_latest_13days['close-open'] = data_days['close-open'].loc[len(
        data_days)-13: len(data_days)]
    index_min = data_latest_13days['close-open'].idxmin()
    index_max = data_latest_13days['close-open'].idxmax()
    print("最近13日最大涨幅：%s, %%%.2f" %
          (data_latest_13days['date'][index_max],
           data_latest_13days['close-open'][index_max] /
           float(data_latest_13days['open'][index_max]) * 100))
    print('最近13日最大跌幅：%s, %%%.2f' %
          (data_latest_13days['date'][index_min],
           data_latest_13days['close-open'][index_min] /
           float(data_latest_13days['open'][index_min]) * 100))
    if (is_sim):
        print('今日：', date_focus, '本周', datetime.strptime(
            date_focus, '%Y-%m-%d').weekday() + 1, '\n',
            data_5min[['time', 'close']].loc[[8, 9, 10, 34, 35, 36]])
    else:
        print('今日：', date_focus, '本周',
              datetime.strptime(date_focus, '%Y-%m-%d').weekday() + 1, '\n',
              data_5min[['time', 'close']].
              loc[len(data_5min)-3:len(data_5min) - 1])
    if (is_sim):
        return (float(data_5min['close'][34]),
                float(data_days['close'].loc[len(data_days) - 1]))
    else:
        return (float(data_5min['close'][len(data_5min) - 1]),
                float(data_days['close'].loc[len(data_days) - 1]))
