set QSPI_FLASHLOADER bsps/TARGET_APP_KIT_PSOCE84_EVK/config/GeneratedSource/PSE84_SMIF.FLM
source [find interface/kitprog3.cfg]
transport select swd
set ENABLE_CM55 1
source [find target/cat1d.cfg]
adapter speed 12000
cat1d.cm33 configure -rtos auto -rtos-wipe-on-reset-halt 1
cat1d.cm55 configure -rtos auto -rtos-wipe-on-reset-halt 1
gdb_breakpoint_override hard