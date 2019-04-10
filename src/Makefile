CXX 	 = g++
ROOTCFLAGS  = $(shell root-config --cflags)
ROOTLIBS    = $(shell root-config --libs)
ROOTGLIBS   = $(shell root-config --glibs)
##CXXFLAGS = $(shell root-config --cflags) -Wall -O3
CXXFLAGS   += $(ROOTCFLAGS)
LIBS	 = $(ROOTGLIBS) -lm -L/usr/X11R6 -L/usr/lib -L./ 
##LIBS	 = -lm -L/usr/X11R6 -L/usr/lib -L./

TARGET 	 = ana
OBJS	 = main.o NoiseScan.o ThrScan.o ThreePointGain.o AtlasStyle.o AtlasLabels.o AtlasUtils.o

all: ${TARGET}

${TARGET}: ${OBJS}
	@echo "Now make : $@"
	${CXX} ${CXXFLAGS} ${LDFLAGS} -o $@ ${OBJS} ${LIBS}
	@echo "... Compile done! "

.SUFFIXES: .cc .C .o
.cc.C.o:
	@echo "Start Compiling : $<"
	${CXX} ${CXXFLAGS} -c $<
	@echo "... Compiling Object Files $< -->done"
	@echo ""

.PHONY: clean
clean:
	@echo "-- Now Clean Up --"
	rm -f *.o *~ $(TARGET)

main.o: AtlasStyle.h AtlasLabels.h AtlasUtils.h NoiseScan.h ThrScan.h ThreePointGain.h main.h

