# SPDX-License-Identifier: CC0-1.0
#
# SPDX-FileContributor: Antonio Niño Díaz, 2024

BLOCKSDS    ?= /opt/blocksds/core

# User config
NAME        := RogueLikeNDS
GAME_TITLE  := RogueLikeNDS
GAME_SUBTITLE := By Zeyzer
GAME_AUTHOR := Zeyzer

# Source code paths
SOURCEDIRS  := source
INCLUDEDIRS := include
GFXDIRS     := sprites
BINDIRS     :=
AUDIODIRS   :=

# Libraries
LIBS        := -lmm9 -lnds9
LIBDIRS     := $(BLOCKSDS)/libs/maxmod \
               $(BLOCKSDS)/libs/libnds

include $(BLOCKSDS)/sys/default_makefiles/rom_arm9/Makefile
