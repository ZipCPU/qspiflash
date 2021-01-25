################################################################################
##
## Filename:	Makefile
## {{{
## Project:	A Set of Wishbone Controlled SPI Flash Controllers
##
## Purpose:	To direct the formal verification of the quad-spi flash
##		controller.
##
## Creator:	Dan Gisselquist, Ph.D.
##		Gisselquist Technology, LLC
##
################################################################################
## }}}
## Copyright (C) 2017-2021, Gisselquist Technology, LLC
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
## along with this program.  (It's in the $(ROOT)/doc directory.  Run make with
## no target there if the PDF file isn't present.)  If not, see
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
TESTS := spi dspi qspi spixpress dualflexpress qflexpress
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
SPIX := spixpress
DSPI := dualflexpress
QSPI := qflexpress
WB   := fwb_slave.v

$(LLQSPI).smt2: $(RTL)/$(LLQSPI).v $(LLQSPI).ys
	$(YOSYS) -ql $(LLQSPI).yslog -s $(LLQSPI).ys

$(LLQSPI) : $(LLQSPI).check
## {{{
$(LLQSPI).check: $(LLQSPI).smt2
	@rm -f $(LLQSPI).check
	$(SMTBMC)    $(BMCARGS) -t 88 --dump-vcd $(LLQSPI).vcd $(LLQSPI).smt2
	$(SMTBMC) -g $(BMCARGS) -t 88 --dump-vcd $(LLQSPI).vcd $(LLQSPI).smt2
	$(SMTBMC)    $(INDARGS) -t 86 --dump-vcd $(LLQSPI).vcd $(LLQSPI).smt2
	touch $(LLQSPI).check
## }}}

.PHONY: $(SPIX)
## {{{
$(SPIX) : $(SPIX)_safety/PASS $(SPIX)_safety_pipe/PASS $(SPIX)_safety_nocfg/PASS
$(SPIX) : $(SPIX)_safety_pipe_nocfg/PASS $(SPIX)_cover/PASS $(SPIX)_cover_pipe/PASS
$(SPIX)_safety/PASS:       $(SPIX).sby $(RTL)/$(SPIX).v $(WB)
	sby -f $(SPIX).sby safety
$(SPIX)_safety_pipe/PASS:  $(SPIX).sby $(RTL)/$(SPIX).v $(WB)
	sby -f $(SPIX).sby safety_pipe
$(SPIX)_safety_nocfg/PASS: $(SPIX).sby $(RTL)/$(SPIX).v $(WB)
	sby -f $(SPIX).sby safety_nocfg
$(SPIX)_safety_pipe_nocfg/PASS: $(SPIX).sby $(RTL)/$(SPIX).v $(WB)
	sby -f $(SPIX).sby safety_pipe_nocfg
$(SPIX)_cover/PASS: $(SPIX).sby $(RTL)/$(SPIX).v $(WB)
	sby -f $(SPIX).sby cover
$(SPIX)_cover_pipe/PASS: $(SPIX).sby $(RTL)/$(SPIX).v $(WB)
	sby -f $(SPIX).sby cover_pipe
## }}}

.PHONY: $(DSPI)
## {{{
$(DSPI) : $(DSPI)_bare/PASS      $(DSPI)_barep/PASS      $(DSPI)_barecfg/PASS
$(DSPI) : $(DSPI)_cfgonly/PASS
$(DSPI) : $(DSPI)_ice40/PASS     $(DSPI)_ice40c/PASS     $(DSPI)_ice40s/PASS
$(DSPI) : $(DSPI)_xilinx/PASS    $(DSPI)_xilinxc/PASS    $(DSPI)_xilinxs/PASS
$(DSPI) : $(DSPI)_ice40div/PASS  $(DSPI)_ice40divc/PASS  #$(DSPI)_ice40divs/PASS
$(DSPI) : $(DSPI)_xilinxdiv/PASS $(DSPI)_xilinxdivc/PASS#$(DSPI)_xilinxdivs/PASS
$(DSPI) : $(DSPI)_arrow/PASS     $(DSPI)_arrowc/PASS     $(DSPI)_arrows/PASS
$(DSPI) : $(DSPI)_divthree/PASS  $(DSPI)_divfive/PASS
$(DSPI) : $(DSPI)_x32/PASS       $(DSPI)_x32c/PASS       $(DSPI)_x32swap/PASS
$(DSPI) : $(DSPI)_xilinxswap/PASS
$(DSPI) : $(DSPI)_bareswap/PASS
$(DSPI) : $(DSPI)_barepswap/PASS
$(DSPI) : $(DSPI)_barecfgswp/PASS
$(DSPI) : $(DSPI)_cfgonlyswp/PASS
# $(DSPI)_divfives/PASS
$(DSPI)_bare/PASS:      $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby bare
$(DSPI)_barep/PASS:      $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby barep
$(DSPI)_barecfg/PASS:    $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby barecfg
$(DSPI)_cfgonly/PASS:    $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby cfgonly
$(DSPI)_ice40/PASS:      $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby ice40
$(DSPI)_ice40c/PASS:     $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby ice40c
$(DSPI)_ice40s/PASS:     $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby ice40s
$(DSPI)_xilinx/PASS:     $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby xilinx
$(DSPI)_xilinxc/PASS:    $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby xilinxc
$(DSPI)_xilinxs/PASS:    $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby xilinxs
$(DSPI)_ice40div/PASS:  $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby ice40div
$(DSPI)_ice40divc/PASS: $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby ice40divc
$(DSPI)_ice40divs/PASS: $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby ice40divs
$(DSPI)_xilinxdiv/PASS:  $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby xilinxdiv
$(DSPI)_xilinxdivc/PASS: $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby xilinxdivc
$(DSPI)_xilinxdivs/PASS: $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby xilinxdivs
$(DSPI)_divthree/PASS:   $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby divthree
$(DSPI)_divfive/PASS:    $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby divfive
$(DSPI)_divfives/PASS:   $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby divfives
$(DSPI)_arrow/PASS:   $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby arrow
$(DSPI)_arrowc/PASS:   $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby arrowc
$(DSPI)_arrows/PASS:   $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby arrows
$(DSPI)_x32/PASS:        $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby x32
$(DSPI)_x32c/PASS:       $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby x32c
$(DSPI)_x32swap/PASS:    $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby x32swap
$(DSPI)_xilinxswap/PASS: $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby xilinxswap
$(DSPI)_bareswap/PASS:   $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby bareswap
$(DSPI)_barepswap/PASS:  $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby barepswap
$(DSPI)_barecfgswp/PASS: $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby barecfgswp
$(DSPI)_cfgonlyswp/PASS: $(DSPI).sby $(RTL)/$(DSPI).v $(WB)
	sby -f $(DSPI).sby cfgonlyswp
## }}}

.PHONY: $(QSPI)
## {{{
$(QSPI) : $(QSPI)_bare/PASS      $(QSPI)_barep/PASS      $(QSPI)_barecfg/PASS
$(QSPI) : $(QSPI)_cfgonly/PASS
$(QSPI) : $(QSPI)_ice40/PASS     $(QSPI)_ice40c/PASS     $(QSPI)_ice40s/PASS
$(QSPI) : $(QSPI)_xilinx/PASS    $(QSPI)_xilinxc/PASS    $(QSPI)_xilinxs/PASS
$(QSPI) : $(QSPI)_ice40div/PASS  $(QSPI)_ice40divc/PASS  $(QSPI)_ice40divs/PASS
$(QSPI) : $(QSPI)_xilinxdiv/PASS $(QSPI)_xilinxdivc/PASS $(QSPI)_xilinxdivs/PASS
$(QSPI) : $(QSPI)_divthree/PASS  $(QSPI)_divfive/PASS    $(QSPI)_divfives/PASS
$(QSPI) : $(QSPI)_bareswap/PASS  $(QSPI)_barepswap/PASS
$(QSPI) : $(QSPI)_barecfgswp/PASS $(QSPI)_cfgonlyswp/PASS
$(QSPI) : $(QSPI)_xilinxswap/PASS $(QSPI)_xilinxdvsw/PASS
$(QSPI) : $(QSPI)_divthrswp/PASS  $(QSPI)_x32/PASS
$(QSPI) : $(QSPI)_x32c/PASS       $(QSPI)_x32swap/PASS
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
$(QSPI)_bareswap/PASS:   $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby bareswap
$(QSPI)_barepswap/PASS:  $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby barepswap
$(QSPI)_barecfgswp/PASS: $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby barecfgswp
$(QSPI)_cfgonlyswp/PASS: $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby cfgonlyswp
$(QSPI)_divthrswp/PASS:  $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby divthrswp
$(QSPI)_xilinxswap/PASS: $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby xilinxswap
$(QSPI)_xilinxdvsw/PASS: $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby xilinxdvsw
$(QSPI)_x32/PASS:        $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby x32
$(QSPI)_x32c/PASS:       $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby x32c
$(QSPI)_x32swap/PASS:    $(QSPI).sby $(RTL)/$(QSPI).v
	sby -f $(QSPI).sby x32swap
## }}}


.PHONY: clean
## {{{
clean:
	rm -f $(LLQSPI).smt2 $(LLQSPI) *.vcd $(LLQSPI).yslog
	rm -rf $(SPIX)_*/ $(DSPI)_*/ $(QSPI)_*/
## }}}
