#
# This file is generated by ModusToolbox during the 'make getlibs' operation
# Any edits to this file will be lost the next time the library manager is run or
# the next time 'make getlibs' is run.
#
bsp-assistant capsense-configurator capsense-tuner config device-configurator config_ezpd ez-pd-configurator modlibs library-manager seglcd-configurator smartio-configurator config_usbdev usbdev-configurator qspi-configurator :
	make -C proj_cm55 $@

config_lin lin-configurator config_audio-fe audio-fe-configurator config_bt bt-configurator config_ml :
	$(error $@ configurator cannot be executed at the application level. Run this command from the desired project directory.)

.PHONY: bsp-assistant capsense-configurator capsense-tuner config device-configurator config_ezpd ez-pd-configurator modlibs library-manager config_lin lin-configurator seglcd-configurator smartio-configurator config_usbdev usbdev-configurator config_audio-fe audio-fe-configurator config_bt bt-configurator config_ml qspi-configurator
