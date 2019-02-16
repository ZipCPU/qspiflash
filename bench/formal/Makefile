################################################################################
##
## Filename:	Makefile
##
## Project:	A Set of Wishbone Controlled SPI Flash Controllers
##
## Purpose:	To direct the formal verification of the quad-spi flash
##		controller.
##
## Creator:	Dan Gisselquist, Ph.D.
##		Gisselquist Technology, LLC
##
################################################################################
##
## Copyright (C) 2017-2019, Gisselquist Technology, LLC
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
TESTS := spi dspi qspi qflexpress
.PHONY: $(TESTS)
all: $(TESTS)
RTL := ../../rtl
YOSYS   := yosys
SMTBMC  := yosys-smtbmc
# SOLVER  := -s z3
SOLVER  := -s yices
# SOLVER  := -s boolector
# SOLVER := -s cvc4 ... not properly installed ??
# SOLVER := -s mathsat
BMCARGS := --presat $(SOLVER) --unroll
# BMCARGS := $(SOLVER) --unroll
# BMCARGS := $(SOLVER)
INDARGS := $(SOLVER) -i

LLQSPI := llqspi
QSPI := qflexpress

$(LLQSPI).smt2: $(RTL)/$(LLQSPI).v $(LLQSPI).ys
	$(YOSYS) -ql $(LLQSPI).yslog -s $(LLQSPI).ys

$(LLQSPI) : $(LLQSPI).check
$(LLQSPI).check: $(LLQSPI).smt2
	@rm -f $(LLQSPI).check
	$(SMTBMC)    $(BMCARGS) -t 88 --dump-vcd $(LLQSPI).vcd $(LLQSPI).smt2
	$(SMTBMC) -g $(BMCARGS) -t 88 --dump-vcd $(LLQSPI).vcd $(LLQSPI).smt2
	$(SMTBMC)    $(INDARGS) -t 86 --dump-vcd $(LLQSPI).vcd $(LLQSPI).smt2
	touch $(LLQSPI).check

.PHONY: $(QSPI)
$(QSPI) : $(QSPI)_bare/PASS      $(QSPI)_barep/PASS      $(QSPI)_barecfg/PASS
$(QSPI) : $(QSPI)_cfgonly/PASS
$(QSPI) : $(QSPI)_ice40/PASS     $(QSPI)_ice40c/PASS     $(QSPI)_ice40s/PASS
$(QSPI) : $(QSPI)_xilinx/PASS    $(QSPI)_xilinxc/PASS    $(QSPI)_xilinxs/PASS
$(QSPI) : $(QSPI)_ice40div/PASS  $(QSPI)_ice40divc/PASS  $(QSPI)_ice40divs/PASS
$(QSPI) : $(QSPI)_xilinxdiv/PASS $(QSPI)_xilinxdivc/PASS $(QSPI)_xilinxdivs/PASS
$(QSPI) : $(QSPI)_divthree/PASS  $(QSPI)_divfive/PASS    $(QSPI)_divfives/PASS
$(QSPI)_bare/PASS:      $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby bare
$(QSPI)_barep/PASS:      $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby barep
$(QSPI)_barecfg/PASS:    $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby barecfg
$(QSPI)_cfgonly/PASS:    $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby cfgonly
$(QSPI)_ice40/PASS:      $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby ice40
$(QSPI)_ice40c/PASS:     $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby ice40c
$(QSPI)_ice40s/PASS:     $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby ice40s
$(QSPI)_xilinx/PASS:     $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby xilinx
$(QSPI)_xilinxc/PASS:    $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby xilinxc
$(QSPI)_xilinxs/PASS:    $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby xilinxs
$(QSPI)_ice40div/PASS:  $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby ice40div
$(QSPI)_ice40divc/PASS: $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby ice40divc
$(QSPI)_ice40divs/PASS: $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby ice40divs
$(QSPI)_xilinxdiv/PASS:  $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby xilinxdiv
$(QSPI)_xilinxdivc/PASS: $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby xilinxdivc
$(QSPI)_xilinxdivs/PASS: $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby xilinxdivs
$(QSPI)_divthree/PASS:   $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby divthree
$(QSPI)_divfive/PASS:    $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby divfive
$(QSPI)_divfives/PASS:   $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby divfives

$(XPRESS).check: $(XPRESS).smt2
	@rm -f $(XPRESS).check
	$(SMTBMC)    $(BMCARGS) -t 88 --dump-vcd $(XPRESS).vcd $(XPRESS).smt2
	$(SMTBMC) -g $(BMCARGS) -t 88 --dump-vcd $(XPRESS).vcd $(XPRESS).smt2
	$(SMTBMC)    $(INDARGS) -t 86 --dump-vcd $(XPRESS).vcd $(XPRESS).smt2
	touch $(XPRESS).check


.PHONY: clean
clean:
	rm $(LLQSPI).smt2 $(LLQSPI) *.vcd $(LLQSPI).yslog
	rm $(XPRESS).smt2 $(XPRESS) *.vcd $(XPRESS).yslog
