#!/usr/local/bin/python3
# coding = utf-8


class Chain():

    def __init__(self, path=''):
        self._path = path

    def __getattr__(self, path):
        self._path = '%s/%s' % (self._path, path)
        return self

    def __str__(self):
        return self._path

    def __repr__(self):
        return self._path

if __name__ == '__main__':
    print(Chain('api.demo.com').users.path)
