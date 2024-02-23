# KIT_PSOCE84_EVK BSP Release Notes


NOTE: BSPs are versioned by family. This means that version 1.2.0 of any BSP in a family (eg: XMC™ ) will have the same software maturity level. However, not all updates are necessarily applicable for each BSP in the family so not all version numbers will exist for each board. Additionally, new BSPs may not start at version 1.0.0. In the event of adding a common feature across all BSPs, the libraries are assigned the same version number. For example if BSP_A is at v1.3.0 and BSP_B is at v1.2.0, the event will trigger a version update to v1.4.0 for both BSP_A and BSP_B. This allows the common feature to be tracked in a consistent way.

### What's Included?
The KIT_PSOCE84_EVK library includes the following:
* BSP specific makefile to configure the build process for the board
* cybsp.c/h files to initialize the board and any system peripherals
* cybsp_types.h file describing basic board setup
* CM33 Linker script & startup code for GCC and ARM toolchains
* CM55 Linker script & startup code for GCC and ARM toolchains
* Configurator design files (and generated code) to setup board specific peripherals
* .lib file references for all dependent libraries
* API documentation

### What Changed?
#### v0.5.2
* System Idle Power Mode = System Deep Sleep
* Removed prebuilt binaries for CM33 secure application
* Arm compiler support
#### v0.5.1
* CLK_HF3 = 200MHz
* System Idle Power Mode = CPU Sleep
#### v0.5.0
* Initial pre-production release

### Supported Software and Tools
This version of the KIT_PSOCE84_EVK BSP was validated for compatibility with the following Software and Tools:

| Software and Tools                        | Version |
| :---                                      | :----:  |
| ModusToolbox™ Software Environment        | 3.1.0   |
| GCC Compiler                              | 12.2.1  |
| ARM Compiler                              | 6.16    |

Minimum required ModusToolbox™ Software Environment: v3.1.0

### More information
* [KIT_PSOCE84_EVK BSP API Reference Manual][api]
* [KIT_PSOCE84_EVK Documentation](https://www.infineon.com/cms/en/product/evaluation-boards/placeholder/)
* [Cypress Semiconductor, an Infineon Technologies Company](http://www.cypress.com)
* [Infineon GitHub](https://github.com/infineon)
* [ModusToolbox™](https://www.cypress.com/products/modustoolbox-software-environment)

[api]: https://infineon.github.io/TARGET_KIT_PSOCE84_EVK/html/modules.html

---
© Cypress Semiconductor Corporation (an Infineon company) or an affiliate of Cypress Semiconductor Corporation, 2019-2023.