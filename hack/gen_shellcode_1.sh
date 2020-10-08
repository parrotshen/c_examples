#!/bin/bash

if [ $# == 0 ]; then
  echo "Usage: $0 <object_file>"
  echo ""
  exit
fi

# generate shellcode for C programming
objdump -d $1 | grep '[0-9a-f]:' \
              | grep -v 'file' \
              | cut -f2 -d: \
              | cut -f1-6 -d' ' \
              | tr '\t' ' ' \
              | sed 's/ *$//g' \
              | sed 's/ /\\x/g' \
              | sed 's/^/"/' \
              | sed 's/$/"/g'

