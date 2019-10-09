#!/bin/bash

# ref https://blog.csdn.net/yi412/article/details/77061869

string="hello bash, I'am a string. lalalala..."
echo $string
echo "==> length of \"$string\":"${#string}
echo "==> get substring from position 12:"${string:12}