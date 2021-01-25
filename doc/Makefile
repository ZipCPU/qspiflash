.PHONY: all
all:
LGPL := lgpl-3.0
GPL := gpl-3.0
SPEC := spec
DSRC := src
all:	$(LGPL).pdf $(SPEC).pdf

## LGPL
## {{{
$(LGPL).pdf: $(DSRC)/$(LGPL).tex
	latex $(DSRC)/$(LGPL).tex
	latex $(DSRC)/$(LGPL).tex
	dvips -q -z -t letter -P pdf -o $(LGPL).ps $(LGPL).dvi
	ps2pdf -dAutoRotatePages=/All $(LGPL).ps $(LGPL).pdf
	rm $(LGPL).dvi $(LGPL).log $(LGPL).aux $(LGPL).ps
## }}}

## GPL
## {{{
$(GPL).pdf: $(DSRC)/$(GPL).tex
	latex $(DSRC)/$(GPL).tex
	latex $(DSRC)/$(GPL).tex
	dvips -q -z -t letter -P pdf -o $(GPL).ps $(GPL).dvi
	ps2pdf -dAutoRotatePages=/All $(GPL).ps $(GPL).pdf
	rm $(GPL).dvi $(GPL).log $(GPL).aux $(GPL).ps
## }}}

## SPEC
## {{{
$(SPEC).pdf: $(DSRC)/$(SPEC).tex $(DSRC)/gqtekspec.cls $(DSRC)/GT.eps
	cd $(DSRC)/; latex $(SPEC).tex
	cd $(DSRC)/; latex $(SPEC).tex
	dvips -q -z -t letter -P pdf -o $(SPEC).ps $(DSRC)/$(SPEC).dvi
	ps2pdf -dAutoRotatePages=/All $(SPEC).ps $(SPEC).pdf
	rm $(DSRC)/$(SPEC).dvi $(DSRC)/$(SPEC).log
	rm $(DSRC)/$(SPEC).aux $(DSRC)/$(SPEC).toc
	rm $(DSRC)/$(SPEC).lof $(DSRC)/$(SPEC).lot
	rm $(SPEC).ps
## }}}
