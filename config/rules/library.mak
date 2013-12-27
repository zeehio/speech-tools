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
 ##                                                                       ##
 ##                 Author: Richard Caley (rjc@cstr.ed.ac.uk)             ##
 ##                   Date: Wed Oct  1 1997                               ##
 ## --------------------------------------------------------------------  ##
 ## Library maintenance rules.                                            ##
 ##                                                                       ##
 ###########################################################################

 ###########################################################################
 ##                                                                       ##
 ## Rules for the lib directory itself                                    ##
 ##                                                                       ##
 ###########################################################################

# if we are in that directory, make sure it's up to date

ifndef MADE_FROM_ABOVE
.libraries: make_library
# .build_shared : make_library
endif

# if we requested a shared library, build it

ifdef SHARED
ifneq ($(SHARED),0)
ifndef MAKE_SHARED_LIB
.config_error:: FORCE
	@echo "+-----------------------------------------------------"
	@echo "| Can't make shared libraries for compiler $(COMPILER)
	@echo "+-----------------------------------------------------"
	@exit 1
endif

ifeq ($(SHARED),2)
    PROJECT_SHARED_LIBRARIES:=$(PROJECT_ALL_LIBRARIES)
endif

.libraries: .build_shared

.build_shared : $(PROJECT_SHARED_LIBRARIES:%=lib%.so)
endif
endif

.libraries:
	@$(DO_NOTHING)

# Make the libraries

make_library:
	@ $(MAKE) --no-print-directory -C $(TOP) JUST_LIB=YES

 ###########################################################################
 ##                                                                       ##
 ## If we are in a sub directory which explicitly says something          ##
 ## depends on the library, check it is OK.                               ##
 ##                                                                       ##
 ###########################################################################

ifndef MADE_FROM_ABOVE
$(PROJECT_LIBRARY_DIR_$(PROJECT_DEFAULT_LIBRARY))/lib$(PROJECT_DEFAULT_LIBRARY).a : FORCE
	@ echo remake libraries $@
	@ $(MAKE) --no-print-directory -C $(TOP) JUST_LIB=YES

endif

 ###########################################################################
 ##                                                                       ##
 ## Shared library rule. Expand the normal library and link the           ##
 ## bits together.                                                        ##
 ##                                                                       ##
 ###########################################################################

libestools.so : libestools.a
	echo Make Shared Library estools
	if [ ! -d shared_space ] ; then mkdir shared_space ; else $(RM) -f shared_space/*.o ; fi
	(cd shared_space ; $(AR) x ../$< )
	echo Link Shared Library estools
	if [ -n "$(PROJECT_LIBRARY_NEEDS_SYSLIBS_estools)" ] ; then libs='$(JAVA_PROJECT_LIBS)' ; fi ;\
	$(subst YYY,$@.$(PROJECT_LIBRARY_VERSION_estools),\
		$(subst XXX,$@.$(PROJECT_VERSION),$(MAKE_SHARED_LIB))) \
		shared_space/*.o $(PROJECT_LIBRARY_USES_estools:%=-L. -l%) $$libs -L. -lestbase -leststring -lncurses
	$(RM) -f shared_space/*.o $@
	-ln -sf $@.$(PROJECT_VERSION) $@.$(PROJECT_LIBRARY_VERSION_estools)
	-ln -sf $@.$(PROJECT_LIBRARY_VERSION_estools) $@

libestbase.so : libestbase.a
	echo Make Shared Library estbase
	if [ ! -d shared_space ] ; then mkdir shared_space ; else $(RM) -f shared_space/*.o ; fi
	(cd shared_space ; $(AR) x ../$< )
	echo Link Shared Library estbase
	if [ -n "$(PROJECT_LIBRARY_NEEDS_SYSLIBS_estbase)" ] ; then libs='$(JAVA_PROJECT_LIBS)' ; fi ;\
	$(subst YYY,$@.$(PROJECT_LIBRARY_VERSION_estbase),\
		$(subst XXX,$@.$(PROJECT_VERSION),$(MAKE_SHARED_LIB))) \
		shared_space/*.o $(PROJECT_LIBRARY_USES_estbase:%=-L. -l%) $$libs -L.
	$(RM) -f shared_space/*.o $@
	-ln -sf $@.$(PROJECT_VERSION) $@.$(PROJECT_LIBRARY_VERSION_estbase)
	-ln -sf $@.$(PROJECT_LIBRARY_VERSION_estbase) $@


 ###########################################################################
 ##                                                                       ##
 ## Rules for normal source directories.                                  ##
 ##                                                                       ##
 ###########################################################################

.PHONY:: .buildlib .buildlibs

## synonym
.buildlib: .buildlibs

## update libraries
.buildlibs: $(foreach lib,$(LIBRARIES),.buildlib_$(lib))

.buildlib_% : $(OBJS)
	@echo look at library $* $(OBJS_$*);
	@if [ -n '$(OBJS_$*)' ] ; then \
	    echo Update library $* $(OBJS_$*);\
	    $(BUILD_LIB) $(PROJECT_LIBRARY_DIR_$*)/lib$*.a $(OBJS_$*) ;\
	    $(INDEX_LIB) $(PROJECT_LIBRARY_DIR_$*)/lib$*.a ;\
	fi
	@$(RM) -f .buildlib_$*; touch .buildlib_$*



.copy_libs: FORCE
	@$(ECHO_N) "Install static libraries '$(PROJECT_LIBRARIES)':"
	@for l in $(PROJECT_LIBRARIES:%=lib%.a) ;\
		do \
		$(ECHO_N) " $$l" ;\
		cp $$l $(LIB)/ ;\
		$(INDEX_LIB) $(LIB)/$$l ;\
	done
	@echo
	@echo
ifdef SHARED
ifneq (,$(PROJECT_SHARED_LIBRARIES))
	@$(ECHO_N) "Install shared libraries '$(PROJECT_SHARED_LIBRARIES)':"
	@for l in $(PROJECT_SHARED_LIBRARIES:%=lib%.so) ;\
		do \
		$(ECHO_N) " $$l" ;\
		cp $$l.* $(LIB)/ ;\
		$(RM) -f $(LIB)/$$l ;\
		ln -s $$l.* $(LIB)/$$l ;\
	done
	@echo
	@echo

endif
endif
