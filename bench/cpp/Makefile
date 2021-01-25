################################################################################
##
## Filename: 	Makefile
## {{{
## Project:	A Set of Wishbone Controlled SPI Flash Controllers
##
## Purpose:	This makefile coordinates the build of the extended quad spi
##		flash controller (not the normal quad spi flash controller also
##	in this directory).
##
## Creator:	Dan Gisselquist, Ph.D.
##		Gisselquist Technology, LLC
##
################################################################################
## }}}
## Copyright (C) 2015-2021, Gisselquist Technology, LLC
## {{{
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
## You should have received a copy of the GNU Lesser General Public License
## along with this program.  (It's in the $(ROOT)/doc directory.  Run make
## with no target there if the PDF file isn't present.)  If not, see
## <http://www.gnu.org/licenses/> for a copy.
## }}}
## License:	LGPL, v3, as defined and found on www.gnu.org,
## {{{
##		http://www.gnu.org/licenses/lgpl.html
##
##
################################################################################
##
## }}}
CXX	:= g++
CFLAGS	:= -Wall -Og -g
OBJDIR  := obj-pc
RTLD	:= ../../rtl
VERILATOR_ROOT   ?= $(shell bash -c 'verilator -V|grep VERILATOR_ROOT | head -1 | sed -e " s/^.*=\s*//"')
VROOT   := $(VERILATOR_ROOT)
VDEFS   := $(shell ./vversion.sh)
VINCD   := $(VROOT)/include
INCS	:= -I$(RTLD)/obj_dir/ -I$(RTLD) -I$(VINCD) -I$(VINCD)/vltstd
SIMSRCS :=  flashsim.cpp byteswap.cpp
LEGACYSRC := wbqspiflash_tb.cpp $(SIMSRCS)
SPISRC  := spixpress_tb.cpp     $(SIMSRCS)
DSPISRC := dualflexpress_tb.cpp $(SIMSRCS)
QSPISRC := qflexpress_tb.cpp    $(SIMSRCS)
SOURCES := flashsim.cpp byteswap.cpp dualflexpress_tb.cpp flashsim.cpp \
	qflexpress_tb.cpp qspiflashsim.cpp qspiflash_tb.cpp spixpress_tb.cpp \
	wbqspiflash_tb.cpp
VOBJDR	:= $(RTLD)/obj_dir
RAWVLIB	:= verilated.cpp verilated_vcd_c.cpp
VSRCS	:= $(addprefix $(VROOT)/include/,$(RAWVLIB))
VOBJS	:= $(addprefix $(OBJDIR)/,$(subst .cpp,.o,$(RAWVLIB)))
LOBJS   :=  $(addprefix $(OBJDIR)/,$(subst .cpp,.o,$(LEGACYSRC))) $(VOBJS)
SOBJS   :=  $(addprefix $(OBJDIR)/,$(subst .cpp,.o,$(SPISRC)))  $(VOBJS)
DOBJS   :=  $(addprefix $(OBJDIR)/,$(subst .cpp,.o,$(DSPISRC))) $(VOBJS)
QOBJS   :=  $(addprefix $(OBJDIR)/,$(subst .cpp,.o,$(QSPISRC))) $(VOBJS)
all:	spixpress_tb dualflexpress_tb qflexpress_tb wbqspiflash_tb pretest

$(OBJDIR)/%.o: %.cpp
	$(mk-objdir)
	$(CXX) $(CFLAGS) $(VDEFS) $(INCS) -c $< -o $@

$(OBJDIR)/%.o: $(VINCD)/%.cpp
	$(mk-objdir)
	$(CXX) $(CFLAGS) $(INCS) -c $< -o $@

wbqspiflash_tb: $(LOBJS) $(VOBJDR)/Vwbqspiflash__ALL.a
	$(CXX) $(CFLAGS) $(INCS) $(LOBJS) $(VOBJDR)/Vwbqspiflash__ALL.a -o $@

spixpress_tb: $(SOBJS) $(VOBJDR)/Vspixpress__ALL.a
	$(CXX) $(CFLAGS) $(INCS) $(SOBJS) $(VOBJDR)/Vspixpress__ALL.a -o $@

dualflexpress_tb: $(DOBJS) $(VOBJDR)/Vdualflexpress__ALL.a
	$(CXX) $(CFLAGS) $(INCS) $(DOBJS) $(VOBJDR)/Vdualflexpress__ALL.a -o $@

qflexpress_tb: $(QOBJS) $(VOBJDR)/Vqflexpress__ALL.a
	$(CXX) $(CFLAGS) $(INCS) $(QOBJS) $(VOBJDR)/Vqflexpress__ALL.a -o $@

.PHONY: pretest
pretest: spixpress_tb dualflexpress_tb qflexpress_tb
	@echo "The test bench has been created.  Type make test, and look at"
	@echo "the end of its output to see if it (still) works."

# .PHONY: test
# test: eqspiflash_tb
#	./eqspiflash_tb

.PHONY: test stest dtest qtest legacytest
test: stest dtest qtest
stest: spixpress_tb
	./spixpress_tb
dtest: dualflexpress_tb
	./dualflexpress_tb
qtest: qflexpress_tb
	./qflexpress_tb
legacytest: wbqpiflash_tb
	./wbqpiflash_tb

define	mk-objdir
	@bash -c "if [ ! -e $(OBJDIR) ]; then mkdir -p $(OBJDIR); fi"
endef


define build-depends
	@echo "Building dependency file(s)"
	$(mk-objdir)
	@$(CXX) $(CFLAGS) $(INCS) -MM $(SOURCES) > $(OBJDIR)/xdepends.txt
	@sed -e 's/^.*.o: /$(OBJDIR)\/&/' < $(OBJDIR)/xdepends.txt > $(OBJDIR)/depends.txt
	@rm $(OBJDIR)/xdepends.txt
endef

tags: $(SOURCES) $(HEADERS)
	@echo "Generating tags"
	@ctags $(SOURCES) $(HEADERS)

.PHONY: clean
clean:
	rm -f spixpress_tb dualflexpress_tb qflexpress_tb
	rm -f *.vcd
	rm -rf wbqspiflash_tb $(OBJDIR)/

.PHONY: depends
depends: tags
	$(build-depends)

$(OBJDIR)/depends.txt: $(SOURCES) $(HEADERS)
	$(build-depends)

ifneq ($(MAKECMDGOALS),clean)
-include $(OBJDIR)/depends.txt
endif
