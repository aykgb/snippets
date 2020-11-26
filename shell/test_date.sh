#!/usr/bin/env bash

set -x

DIR=$(cd $(dirname $0) && pwd)
cd $DIR

ts=$(date +%s)
echo ts:$ts
dt=$(date -d @$ts)
echo dt:$dt

# REF http://einverne.github.io/post/2017/09/use-shell-command-to-convert-timestamp-and-date.html