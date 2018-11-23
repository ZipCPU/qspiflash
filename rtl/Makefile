################################################################################
##
## Filename:	Makefile
##
## Project:	A Set of Wishbone Controlled SPI Flash Controllers
##
## Purpose:	To direct the Verilator build of the SoC sources.  The result
##		is C++ code (built by Verilator), that is then built (herein)
##	into a library that you can find in obj_dir.
##
##
## Creator:	Dan Gisselquist, Ph.D.
##		Gisselquist Technology, LLC
##
################################################################################
##
## Copyright (C) 2018, Gisselquist Technology, LLC
##
## This file is part of the set of Wishbone controlled SPI flash controllers
## project
##
## The Wishbone SPI flash controller project is free software (firmware):
## you can redistribute it and/or modify it under the terms of the GNU Lesser
## General Public License as published by the Free Software Foundation, either
## version 3 of the License, or (at your option) any later version.
##
## The Wishbone SPI flash controller project is distributed in the hope
## that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
## warranty of MERCHANTIBILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public License along
## with this program.  (It's in the $(ROOT)/doc directory.  Run make with no
## target there if the PDF file isn't present.)  If not, see
## <http://www.gnu.org/licenses/> for a copy.
##
## License:	LGPL, v3, as defined and found on www.gnu.org,
##		http://www.gnu.org/licenses/lgpl.html
##
##
################################################################################
##
##
all:	test
YYMMDD=`date +%Y%m%d`
VDIRFB := ./obj_dir
SPI    := spixpress
DSPI   := dualflexpress
QSPI   := qflexpress
LEGACY := wbqspiflash
SUBMAKE := make --no-print-directory -C
VERILATOR := verilator
VFLAGS    := -Wall --MMD --trace -cc

.PHONY: test
test: $(VDIRFB)/V$(SPI)__ALL.a $(VDIRFB)/V$(LEGACY)__ALL.a
test: $(VDIRFB)/V$(DSPI)__ALL.a $(VDIRFB)/V$(QSPI)__ALL.a

$(VDIRFB)/V$(LEGACY).mk:  $(VDIRFB)/V$(LEGACY).h
$(VDIRFB)/V$(LEGACY).cpp: $(VDIRFB)/V$(LEGACY).h
$(VDIRFB)/V$(LEGACY).h: $(LEGACY).v llqspi.v
	$(VERILATOR) $(VFLAGS) $(LEGACY).v 

$(VDIRFB)/V$(SPI).mk:  $(VDIRFB)/V$(SPI).h
$(VDIRFB)/V$(SPI).cpp: $(VDIRFB)/V$(SPI).h
$(VDIRFB)/V$(SPI).h: $(SPI).v
	$(VERILATOR) $(VFLAGS) $(SPI).v 

$(VDIRFB)/V$(DSPI).mk:  $(VDIRFB)/V$(DSPI).h
$(VDIRFB)/V$(DSPI).cpp: $(VDIRFB)/V$(DSPI).h
$(VDIRFB)/V$(DSPI).h: $(DSPI).v
	$(VERILATOR) $(VFLAGS) $(DSPI).v 

$(VDIRFB)/V$(QSPI).mk:  $(VDIRFB)/V$(QSPI).h
$(VDIRFB)/V$(QSPI).cpp: $(VDIRFB)/V$(QSPI).h
$(VDIRFB)/V$(QSPI).h: $(QSPI).v
	$(VERILATOR) $(VFLAGS) $(QSPI).v 

$(VDIRFB)/V%__ALL.a: $(VDIRFB)/V%.mk
	$(SUBMAKE) $(VDIRFB) -f V$*.mk

tags: $(LEGACY).v llqspi.v $(SPI).v $(DSPI).v $(QSPI.v)
	ctags $^

.PHONY: clean
clean:
	rm -rf $(VDIRFB)/


DEPS = $(wildcard $(VDIRFB)/*.d)
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(DEPS),)
include $(DEPS)
endif
endif
