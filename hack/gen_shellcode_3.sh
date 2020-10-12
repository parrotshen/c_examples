#!/bin/bash

if [ $# == 0 ]; then
  echo "Usage: $0 <object_file>"
  echo ""
  exit
fi

# generate shellcode for udp_send
objdump -d $1 | grep '[0-9a-f]:' \
              | grep -v 'file' \
              | cut -f2 -d: \
              | cut -f1-6 -d' ' \
              | tr '\t' ' ' \
              | sed 's/ *$//g'

