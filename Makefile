CXX      := g++
CXXFLAGS := -I$(FRZ_BASE) -I$(FRZ_BASE)/include -O2 $(shell root-config --cflags)
LDFLAGS  := -L$(FRZ_BASE)/lib $(shell root-config --glibs)

TARGET := lib/libFRZ.so
DICT   := FRZdict

all: $(TARGET)
	-@$(RM) $(FRZ_BASE)/lib/$(DICT)_rdict.pcm
	-@ln -s $(FRZ_BASE)/src/$(DICT)_rdict.pcm $(FRZ_BASE)/lib/$(DICT)_rdict.pcm

$(TARGET): src/FinalState.o src/Lepton.o src/LeptonPair.o src/MET.o \
		src/Jet.o src/Sample.o src/SampleHolder.o \
		src/Swizzler.o src/HistMaker.o src/$(DICT).o
	@mkdir -p $(FRZ_BASE)/lib
	@echo "Linking $@";$(CXX) $(LDFLAGS) -shared -o $@ $^

src/$(DICT).cxx: include/FRZ/FinalState.h include/FRZ/Jet.h include/FRZ/Lepton.h \
			include/FRZ/LeptonPair.h include/FRZ/MET.h include/FRZ/Sample.h \
			include/FRZ/SampleHolder.h include/FRZ/Swizzler.h \
			include/FRZ/HistMaker.h include/FRZ/LinkDef.h
	@echo "Generating dictionary $@";rootcling -f $@ -I$(FRZ_BASE)/include $^

src/$(DICT).o: src/$(DICT).cxx
	@echo "Compiling $@";$(CXX) -fPIC $(CXXFLAGS) -c $< -o $@

src/FinalState.o: src/FinalState.cxx
	@echo "Compiling $@";$(CXX) -fPIC $(CXXFLAGS) -c $< -o $@

src/Lepton.o: src/Lepton.cxx
	@echo "Compiling $@";$(CXX) -fPIC $(CXXFLAGS) -c $< -o $@

src/LeptonPair.o: src/LeptonPair.cxx
	@echo "Compiling $@";$(CXX) -fPIC $(CXXFLAGS) -c $< -o $@

src/Jet.o: src/Jet.cxx
	@echo "Compiling $@";$(CXX) -fPIC $(CXXFLAGS) -c $< -o $@

src/MET.o: src/MET.cxx
	@echo "Compiling $@";$(CXX) -fPIC $(CXXFLAGS) -c $< -o $@

src/Sample.o: src/Sample.cxx
	@echo "Compiling $@";$(CXX) -fPIC $(CXXFLAGS) -c $< -o $@

src/SampleHolder.o: src/SampleHolder.cxx
	@echo "Compiling $@";$(CXX) -fPIC $(CXXFLAGS) -c $< -o $@

src/Swizzler.o: src/Swizzler.cxx
	@echo "Compiling $@";$(CXX) -fPIC $(CXXFLAGS) -c $< -o $@

src/HistMaker.o: src/HistMaker.cxx
	@echo "Compiling $@";$(CXX) -fPIC $(CXXFLAGS) -c $< -o $@

clean:
	-@$(RM) src/*.o src/*dict* lib/*.so lib/*.pcm
