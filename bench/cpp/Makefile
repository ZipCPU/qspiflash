################################################################################
##
## Filename: 	Makefile
##
## Project:	Wishbone Controlled Quad SPI Flash Controller
##
## Purpose:	This makefile coordinates the build of the extended quad spi
##		flash controller (not the normal quad spi flash controller also
##	in this directory).
##
## Creator:	Dan Gisselquist, Ph.D.
##		Gisselquist Technology, LLC
##
################################################################################
##
## Copyright (C) 2015-2017, Gisselquist Technology, LLC
##
## This program is free software (firmware): you can redistribute it and/or
## modify it under the terms of  the GNU General Public License as published
## by the Free Software Foundation, either version 3 of the License, or (at
## your option) any later version.
##
## This program is distributed in the hope that it will be useful, but WITHOUT
## ANY WARRANTY; without even the implied warranty of MERCHANTIBILITY or
## FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
## for more details.
##
## You should have received a copy of the GNU General Public License along
## with this program.  (It's in the $(ROOT)/doc directory.  Run make with no
## target there if the PDF file isn't present.)  If not, see
## <http://www.gnu.org/licenses/> for a copy.
##
## License:	GPL, v3, as defined and found on www.gnu.org,
##		http://www.gnu.org/licenses/gpl.html
##
##
################################################################################
##
##
CXX	:= g++
CFLAGS	:= -Wall -Og -g
OBJDIR  := obj-pc
RTLD	:= ../../rtl
VERILATOR_ROOT   ?= $(shell bash -c 'verilator -V|grep VERILATOR_ROOT | head -1 | sed -e " s/^.*=\s*//"')
VROOT   := $(VERILATOR_ROOT)
VINCD   := $(VROOT)/include
INCS	:= -I$(RTLD)/obj_dir/ -I$(RTLD) -I$(VINCD) -I$(VINCD)/vltstd
EQSOURCES := eqspiflashsim.cpp eqspiflash_tb.cpp
QSOURCES  :=  qspiflashsim.cpp  qspiflash_tb.cpp
SOURCES := $(EQSOURCES) $(QSOURCES)
VOBJDR	:= $(RTLD)/obj_dir
RAWVLIB	:= verilated.cpp verilated_vcd_c.cpp
VSRCS	:= $(addprefix $(VROOT)/include/,$(RAWVLIB))
VOBJS	:= $(addprefix $(OBJDIR)/,$(subst .cpp,.o,$(RAWVLIB)))
QOBJS   :=  $(addprefix $(OBJDIR)/,$(subst .cpp,.o,$(QSOURCES))) $(VOBJS)
EQOBJS   :=  $(addprefix $(OBJDIR)/,$(subst .cpp,.o,$(EQSOURCES))) $(VOBJS)
SIMSRCS := enetctrlsim.cpp eqspiflashsim.cpp
all:	qspiflash_tb pretest

$(OBJDIR)/%.o: %.cpp
	$(mk-objdir)
	$(CXX) $(CFLAGS) $(INCS) -c $< -o $@

$(OBJDIR)/%.o: $(VINCD)/%.cpp
	$(mk-objdir)
	$(CXX) $(CFLAGS) $(INCS) -c $< -o $@

eqspiflash_tb: $(EQOBJS) $(VOBJDR)/Veqspiflash__ALL.a
	$(CXX) $(CFLAGS) $(INCS) $(EQOBJS) $(VOBJDR)/Veqspiflash__ALL.a -o $@

qspiflash_tb: $(QOBJS) $(VOBJDR)/Vwbqspiflash__ALL.a $(VOBJS)
	$(CXX) $(CFLAGS) $(INCS) $(QOBJS) $(VOBJDR)/Vwbqspiflash__ALL.a -o $@

.PHONY: pretest
pretest: qspiflash_tb eqspiflash_tb
	@echo "The test bench has been created.  Type make test, and look at"
	@echo "the end of its output to see if it (still) works."

# .PHONY: test
# test: eqspiflash_tb
#	./eqspiflash_tb

.PHONY: test qtest eqtest
test: qtest eqtest
qtest: qspiflash_tb
	./qspiflash_tb
eqtest: eqspiflash_tb qtest
	./eqspiflash_tb

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
	rm -f ./eqspiflash_tb qspiflash_tb
	rm -rf $(OBJDIR)/

.PHONY: depends
depends: tags
	$(build-depends)

$(OBJDIR)/depends.txt: $(SOURCES) $(HEADERS)
	$(build-depends)

-include $(OBJDIR)/depends.txt
