################################################################################
# \file common.mk
# \version 1.0
#
# \brief
# Settings shared across all projects.
#
################################################################################
# \copyright
# Copyright 2023-2024, Cypress Semiconductor Corporation (an Infineon company)
# SPDX-License-Identifier: Apache-2.0
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
################################################################################

MTB_TYPE=PROJECT

# Target board/hardware (BSP).
# To change the target, it is recommended to use the Library manager
# ('make library-manager' from command line), which will also update Eclipse IDE launch
# configurations.
TARGET=APP_KIT_PSOCE84_EVK

# Name of toolchain to use. Options include:
#
# GCC_ARM -- GCC provided with ModusToolbox software
# ARM     -- ARM Compiler (must be installed separately)
# IAR     -- IAR Compiler (must be installed separately)
#
# See also: CY_COMPILER_PATH below
TOOLCHAIN=GCC_ARM

# Default build configuration. Options include:
#
# Debug -- build with minimal optimizations, focus on debugging.
# Release -- build with full optimizations
# Custom -- build with custom configuration, set the optimization flag in CFLAGS
# 
# If CONFIG is manually edited, ensure to update or regenerate launch configurations 
# for your IDE.
CONFIG=Debug

COMPONENTS+=GFXSS

# Variable to compile GPU disabled app (value '1', this is default setting) or
# the GPU enabled app (value '0').
GPU_DISABLED=0

ifeq  ($(GPU_DISABLED), 1)
SOCMEMSRAM_CM55NS_APP_SIZE=0x00080000
SOCMEMSRAM_GPUBUF_SIZE=0x00100000
SOCMEMSRAM_SHARED_SIZE=0x00040000
SOCMEMSRAM_CM55NS_DATA_SIZE=0x00280000
SOCMEMSRAM_CM33NS_APP_SIZE=0x00040000
SOCMEMSRAM_CM33NS_DATA_SIZE=0x00080000
else
SOCMEMSRAM_CM55NS_APP_SIZE=0x0000000
SOCMEMSRAM_GPUBUF_SIZE=0x00300000
SOCMEMSRAM_SHARED_SIZE=0x00100000
SOCMEMSRAM_CM55NS_DATA_SIZE=0x00100000
SOCMEMSRAM_CM33NS_APP_SIZE=0x00000000
SOCMEMSRAM_CM33NS_DATA_SIZE=0x00000000
endif

DEFINES+=SOCMEMSRAM_CM55NS_APP_SIZE=$(SOCMEMSRAM_CM55NS_APP_SIZE)
DEFINES+=SOCMEMSRAM_GPUBUF_SIZE=$(SOCMEMSRAM_GPUBUF_SIZE)
DEFINES+=SOCMEMSRAM_SHARED_SIZE=$(SOCMEMSRAM_SHARED_SIZE)
DEFINES+=SOCMEMSRAM_CM55NS_DATA_SIZE=$(SOCMEMSRAM_CM55NS_DATA_SIZE)
DEFINES+=SOCMEMSRAM_CM33NS_APP_SIZE=$(SOCMEMSRAM_CM33NS_APP_SIZE)
DEFINES+=SOCMEMSRAM_CM33NS_DATA_SIZE=$(SOCMEMSRAM_CM33NS_DATA_SIZE)

LDFLAGS+=-Wl,--defsym,APP_SOCMEMSRAM_CM55NS_APP_SIZE=$(SOCMEMSRAM_CM55NS_APP_SIZE)
LDFLAGS+=-Wl,--defsym,APP_SOCMEMSRAM_GPUBUF_SIZE=$(SOCMEMSRAM_GPUBUF_SIZE)
LDFLAGS+=-Wl,--defsym,APP_SOCMEMSRAM_SHARED_SIZE=$(SOCMEMSRAM_SHARED_SIZE)
LDFLAGS+=-Wl,--defsym,APP_SOCMEMSRAM_CM55NS_DATA_SIZE=$(SOCMEMSRAM_CM55NS_DATA_SIZE)
LDFLAGS+=-Wl,--defsym,APP_SOCMEMSRAM_CM33NS_APP_SIZE=$(SOCMEMSRAM_CM33NS_APP_SIZE)
LDFLAGS+=-Wl,--defsym,APP_SOCMEMSRAM_CM33NS_DATA_SIZE=$(SOCMEMSRAM_CM33NS_DATA_SIZE)

include ../common_app.mk
