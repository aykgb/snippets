import time
import urllib.request
from datetime import datetime

time_now_str = datetime.now().strftime('%Y_%m_%d-%H_%M_%S')
filename = time_now_str + '-realtime.txt'
file = open(filename, 'w+')


def get_market_data(stock: str):
    time.sleep(0.2)
    url = 'http://hq.sinajs.cn/list=' + stock.replace('.', '')
    content = urllib.request.urlopen(url).read().decode('gb2312', 'replace')
    print(content)


stocks = {
    'sh.600036': "招商银行",
    'sh.600196': "复星医药",
    'sh.601607': "上海医药",
    'sz.000002': "万 科Ａ"
}

for stock in stocks:
    get_market_data(stock)
