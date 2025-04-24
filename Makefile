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

# ----- Precompiled Header Configuration -----
PCH_HEADER  := include/pch.h
PCH_FILE    := $(PCH_HEADER).gch

# Force generation of PCH before building the ROM
$(NAME).nds: $(PCH_FILE)

# Rule to generate the PCH
$(PCH_FILE): $(PCH_HEADER)
	@echo "Generating PCH: $<"
	$(CXX) $(CXXFLAGS) -x c++-header -c $< -o $@

# Inject PCH usage into all compilations
CXXFLAGS   += -include $(PCH_HEADER) -Winvalid-pch

# ---------------------------------------------

include $(BLOCKSDS)/sys/default_makefiles/rom_arm9/Makefile
