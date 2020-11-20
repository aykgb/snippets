#!/bin/bash

ls | grep -E "public.log.*.20190[0-9]*" | xargs -d "\n" rm