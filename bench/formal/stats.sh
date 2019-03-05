#!/bin/bash

echo    SPIX:
echo
echo -n "PASS:    " ; ls spix*_*/PASS    | wc
echo -n "FAIL:    " ; ls spix*_*/FAIL    | wc

echo    DUAL:
echo -n "PASS:    " ; ls dualflex*_*/PASS    | wc
echo -n "FAIL:    " ; ls dualflex*_*/FAIL    | wc

echo
echo    QUAD:
echo -n "PASS:    " ; ls qflex*_*/PASS    | wc
echo -n "FAIL:    " ; ls qflex*_*/FAIL    | wc

find . -name "UNKNOWN" -print
find . -name "ERROR"   -print
