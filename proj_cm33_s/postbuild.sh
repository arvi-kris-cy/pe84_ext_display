#!/bin/bash

#input params
APP_DIR=$1
APP_NAME=$2
APP_START_ADDR=$3
EPT_CMD_ARG=$4
SECURED_BOOT=$5
KEY_PATH=$6

#verify params
if [[ ! (-e $APP_DIR/$APP_NAME.hex) ]];then
    echo "postbuild error: The hex file $APP_DIR/$APP_NAME.hex cannot be found."
    exit 1
fi
if [[ $3 = '' ]];then
    echo "postbuild error: Start address not available"
    exit 1
fi
if [[ $4 = '' ]];then
    echo "postbuild error: command arguments not available"
    exit 1
fi

#keep unsigned hex backup
cp $APP_DIR/$APP_NAME.hex $APP_DIR/unsigned.hex

if [[ $SECURED_BOOT = 'TRUE' ]];then
    if [[ ! (-e $KEY_PATH) ]];then
        echo "postbuild error: signing key not found"
        exit 1
    fi 
    #Sign Image    
    edgeprotecttools sign-image --image $APP_DIR/$APP_NAME.hex  --output $APP_DIR/$APP_NAME.hex --hex-addr $APP_START_ADDR --key $KEY_PATH $EPT_CMD_ARG
    if [[ ! $? = 0 ]]; then
        echo "postbuild error: signed image is not generated"
        exit 1
    fi
else
    #Add MCUboot Metadata
    edgeprotecttools image-metadata --image $APP_DIR/$APP_NAME.hex  --output $APP_DIR/$APP_NAME.hex --hex-addr $APP_START_ADDR $EPT_CMD_ARG 
    if [[ ! $? = 0 ]]; then
        echo "postbuild error: signed image is not generated"
        exit 1
    fi
fi

