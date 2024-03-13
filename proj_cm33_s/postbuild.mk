#This file cofngures the MCU Boot Image header/signature

#MCUboot header size
HEADER_SIZE?=0x400

#The value, which is read back from erased flash. Default: 0
ERASED_VAL?=0xff

#The user application file
INPUT_IMAGE?=$(COMPILED_HEX).hex

# Output application image
OUTPUT_IMAGE?=$(COMPILED_HEX).hex

#Start offset of the secure application in flash
#NOTE: this address should be same as defined in the "extended_boot_policy"
#Using Non Secure aliased address for programming Ext flash. See PROGTOOLS-5855
APP_START?=0x60100000 

#Slot size 1245KB ; Default is 0x20000 or 131KB
SLOT_SIZE?=0x130000 

ADDITIONAL_ARGS?=

ifeq ($(EDGE_PROTECT_BOOTLOADER),TRUE)

#Software component name 
BOOT_RECORD?=proj_cm33_s

#alignment of the flash device ;default 8
FLASH_ALIGNMENT?=1

#Sets minimum erase size. Default 0x8000
MIN_ERASE_SIZE?=0x40000

#Update type can be 'overwrite' or 'swap'
UPDATE_TYPE?=overwrite

#Image Type can be BOOT or UPDATE
IMG_TYPE?=BOOT

#Image version for Boot usecase
ifeq ($(IMG_TYPE),BOOT)
IMAGE_VERSION=1.0.0
endif

#Image version for update usecase
ifeq ($(IMG_TYPE),UPDATE)
IMAGE_VERSION=1.1.0
endif

ADDITIONAL_ARGS=--align $(FLASH_ALIGNMENT) --image-version $(IMAGE_VERSION) \
--min-erase-size $(MIN_ERASE_SIZE) --boot-record $(BOOT_RECORD)

ifeq ($(UPDATE_TYPE),overwrite)
ADDITIONAL_ARGS+=--overwrite-only
else
ADDITIONAL_ARGS+=
endif

endif

#Add MCUboot metadata/ sign image
POSTBUILD+=cp $(COMPILED_HEX).hex $(COMPILED_HEX)_unsigned.hex 
POSTBUILD+=&&
ifeq ($(SECURED_BOOT),TRUE)
#Signing the image for secured boot
POSTBUILD+=edgeprotecttools sign-image --image $(INPUT_IMAGE) \
                                           --output $(OUTPUT_IMAGE) \
                                           --erased-val $(ERASED_VAL) \
                                           --header-size $(HEADER_SIZE) \
                                           --hex-addr $(APP_START) \
                                           --slot-size $(SLOT_SIZE) \
                                           --key $(KEY_PATH) \
                                           $(ADDITIONAL_ARGS)
                                           
else
#Only MCUboot header is added to the image. No Signature required, if SECURED_BOOT is disabled.
POSTBUILD+=edgeprotecttools image-metadata --image $(INPUT_IMAGE) \
                                           --output $(OUTPUT_IMAGE) \
                                           --erased-val $(ERASED_VAL) \
                                           --header-size $(HEADER_SIZE) \
                                           --slot-size $(SLOT_SIZE) \
                                           --hex-addr $(APP_START) \
                                           $(ADDITIONAL_ARGS)
endif