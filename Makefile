# SPDX-License-Identifier: CC0-1.0
#
# SPDX-FileContributor: Antonio Niño Díaz, 2024

GIT_VERSION := $(shell git describe --always --dirty --tags 2>/dev/null || echo "unknown")
GIT_BRANCH  := $(shell git rev-parse --abbrev-ref HEAD 2>/dev/null || echo "no-branch")

BLOCKSDS	?= /opt/wonderful/thirdparty/blocksds/core
BLOCKSDSEXT	?= /opt/wonderful/thirdparty/blocksds/external

# User config
NAME        := RogueLikeNDS
GAME_TITLE  := RogueLikeNDS
GAME_SUBTITLE := By Zeyzer
GAME_AUTHOR := Zeyzer

# Source code paths
SOURCEDIRS  := source
INCLUDEDIRS := include assets
GFXDIRS     := sprites
BINDIRS     :=
AUDIODIRS   := audio
NITROFSDIR := nitrofs

# Libraries
LIBS        := -lmm9 -lnds9
LIBDIRS     := $(BLOCKSDS)/libs/maxmod \
               $(BLOCKSDS)/libs/libnds

include $(BLOCKSDS)/sys/default_makefiles/rom_arm9/Makefile

CXXFLAGS += -std=c++26
CXXFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\" -DGIT_BRANCH=\"$(GIT_BRANCH)\"
