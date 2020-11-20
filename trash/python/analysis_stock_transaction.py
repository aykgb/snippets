import numpy as np


class Transaction:
    def __init__(self, code: str, name: str):
        self.code = code
        self.name = name
        self.finised = False
        self.date = []
        self.price = []
        self.amount = []
        self.amount_buy = 0
        self.funds_cost = 0
        self.handling_fee = 0

    def trade(self, date: str, price: float, amount: int):
        if (self.finised):
            print('this trade had been turned down.')
            return
        self.date.append(date)
        self.price.append(price)
        self.amount.append(amount)
        fee = price * amount * 0.00025

        if fee <= 5:
            self.handling_fee += 5
        else:
            self.handling_fee += fee

        if (amount > 0):
            self.amount_buy += amount
            self.funds_cost += price * amount
        else:
            self.finised = True

    def dump(self):
        print('\nTransactions:')
        for i in range(len(self.date)):
            print('date:%s price:%.2f amount:%d' %
                  (self.date[i], self.price[i], self.amount[i]))

    def cost(self):
        return self.funds_cost

    def cost_price(self):
        return self.funds_cost/self.amount_buy

    def profit(self, price_latest: float):
        if self.finised:
            return - self.funds_cost - \
                self.amount[len(self.amount) - 1] * \
                self.price[len(self.price) - 1] - self.handling_fee
        else:
            return price_latest * self.amount_buy - \
                self.funds_cost - self.handling_fee

    def rate(self, price_latest: float):
        return (self.profit(price_latest) / self.cost()) * 100


def TEST():
    price_latest = 23.37
    trans = Transaction('sz.000651', '格力电器')
    trans.trade('2017-01-01', 22, 100)
    trans.trade('2017-01-10', 21.2, 100)
    trans.trade('2017-02-01', 23, -200)
    print('cost  : ', trans.cost())
    print('profit: ', trans.profit(price_latest))
    print('profit_rate  : %%%.2f' % trans.rate(price_latest))
    trans.dump()
