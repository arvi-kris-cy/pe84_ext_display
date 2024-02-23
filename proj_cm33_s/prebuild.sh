#!/bin/bash

#input params
APP_DIR=$1
APP_NAME=$2

if [ -e "$APP_DIR/$APP_NAME.hex" ]; then
    rm "$APP_DIR/$APP_NAME.hex"
    echo "$APP_DIR/$APP_NAME.hex deleted"
fi

if [ -e "$APP_DIR/unsigned.hex" ]; then
    rm "$APP_DIR/unsigned.hex"
    echo "$APP_DIR/unsigned.hex deleted"
fi

