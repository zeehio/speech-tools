###########################################################################
##                                                                       ##
##                Centre for Speech Technology Research                  ##
##                     University of Edinburgh, UK                       ##
##                         Copyright (c) 1996                            ##
##                        All Rights Reserved.                           ##
##                                                                       ##
##  Permission is hereby granted, free of charge, to use and distribute  ##
##  this software and its documentation without restriction, including   ##
##  without limitation the rights to use, copy, modify, merge, publish,  ##
##  distribute, sublicense, and/or sell copies of this work, and to      ##
##  permit persons to whom this work is furnished to do so, subject to   ##
##  the following conditions:                                            ##
##   1. The code must retain the above copyright notice, this list of    ##
##      conditions and the following disclaimer.                         ##
##   2. Any modifications must be clearly marked as such.                ##
##   3. Original authors' names are not deleted.                         ##
##   4. The authors' names are not used to endorse or promote products   ##
##      derived from this software without specific prior written        ##
##      permission.                                                      ##
##                                                                       ##
##  THE UNIVERSITY OF EDINBURGH AND THE CONTRIBUTORS TO THIS WORK        ##
##  DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING      ##
##  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   ##
##  SHALL THE UNIVERSITY OF EDINBURGH NOR THE CONTRIBUTORS BE LIABLE     ##
##  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES    ##
##  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN   ##
##  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,          ##
##  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF       ##
##  THIS SOFTWARE.                                                       ##
##                                                                       ##
###########################################################################
##                     Author :  Paul Taylor                             ##
##                     Date   :  March 1995                              ##
##-----------------------------------------------------------------------##
##                                                                       ##
##                    Documentation directory                            ##
##                                                                       ##
##=======================================================================##
TOP=../..
DIRNAME=doc/man

PROGRAMS = align bcat ch_lab ch_track ch_utt ch_wave design_filter \
           dp na_play na_record ngram_build ngram_test \
           ols ols_test pda pitchmark scfg_make scfg_parse scfg_test \
           scfg_train sig2fv sigfilter siod spectgen tilt_analysis \
           tilt_synthesis viterbi wagon wagon_test wfst_build wfst_run \
           wfst_train xml_parser

BODYFILES = $(addsuffix _man.dox.body, $(PROGRAMS))
DOXFILES = $(addsuffix _man.dox, $(PROGRAMS))


XSLT_FILES = convert-options.xslt convert-synopsis.xslt
SCRIPTS = create_dox.sh

FILES = $(BODYFILES) $(XSLT_FILES) $(SCRIPTS) Makefile

LOCAL_CLEAN = $(DOXFILES)

-include $(TOP)/config/common_make_rules

%_man.dox: %_man.dox.body
	LD_LIBRARY_PATH=$(TOP_BUILDDIR)/lib ./create_dox.sh $(TOP_BUILDDIR)/main/$* $@.body || exit 1

all_dox: $(DOXFILES)

