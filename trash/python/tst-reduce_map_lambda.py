#!/usr/local/bin/python3
# -*- coding: utf-8 -*-
import os

from functools import reduce

def str2float(s):
     l = s.split('.')
     return reduce(lambda x, y : x * 10 + y, map(int, l[0])) + \
            reduce(lambda x, y : x * 0.1 + y, map(int, l[1][::-1])) * 0.1

print(str2float('1324.123'))

print("%d, %s", 3, 'test')
