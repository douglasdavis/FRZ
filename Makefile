###########################################################
## Makefile for the FRZ (Fake Rate using Z+jets) library ## 
## Author: Douglas Davis < ddavis@phy.duke.edu >         ##
###########################################################

ifndef FRZ_BASE
$(error FRZ_BASE not set, source config/setup)
endif

CXX      := g++
CXXFLAGS := -Wall -I$(FRZ_BASE) -I$(FRZ_BASE)/include -O2 $(shell root-config --cflags)
LDFLAGS  := -L$(FRZ_BASE)/lib $(shell root-config --glibs)

ARCH      = $(shell uname)
ifeq ($(ARCH),Darwin)
	LIBSUFFIX = dylib
else
	LIBSUFFIX = so
endif

TARGET  := lib/libFRZ.$(LIBSUFFIX)

DICT    := FRZdict
SOURCES := $(wildcard src/*.cxx)
OBJECTS := $(patsubst %.cxx,%.o,$(SOURCES))
HEADERS := $(filter-out include/FRZ/LinkDef.h \
		include/FRZ/Common.h include/FRZ/Utils.h,\
		$(wildcard include/FRZ/*.h))

all: $(TARGET)
	-@$(RM) $(FRZ_BASE)/lib/$(DICT)_rdict.pcm
	-@ln -s $(FRZ_BASE)/src/$(DICT)_rdict.pcm $(FRZ_BASE)/lib/$(DICT)_rdict.pcm
	@echo "<< Built >>"

$(TARGET): $(OBJECTS) src/$(DICT).o
	@mkdir -p $(FRZ_BASE)/lib
	@echo "<< Linking shared object library $@ >>";$(CXX) $(LDFLAGS) -shared -o $@ $^

src/$(DICT).cxx: $(HEADERS) include/FRZ/LinkDef.h
	@echo "<< Generating ROOT dictionary $@ >>";rootcint -f $@ -I$(FRZ_BASE)/include $^

src/$(DICT).o: src/$(DICT).cxx
	@echo "<< Compiling object $@ >>";$(CXX) -fPIC $(CXXFLAGS) -c $< -o $@

src/%.o: src/%.cxx
	@echo "<< Compiling object $@ >>";$(CXX) -fPIC $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	@echo "Cleaning...";$(RM) src/*.o src/*dict* lib/*.$(LIBSUFFIX) lib/*.pcm
