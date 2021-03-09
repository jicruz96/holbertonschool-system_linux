#!/bin/bash

if [ "$#" -ne 1 ]
then
    echo "Usage: $0 <pid>"
    exit 1
else
    kill -QUIT $1
fi
