CXX=g++
CXXOPTIMIZE= -O2
CXXFLAGS= -g -Wall -pthread -std=c++11 $(CXXOPTIMIZE)
USERID=EDIT_MAKE_FILE
CLASSES=

all: my-router start

my-router: $(CLASSES)
	$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp


clean:
	rm -rf *.o *~ *.gch *.swp *.dSYM my-router start *.tar.gz

tarball: clean
	tar -cvf $(USERID).tar.gz *


kill:
	 killall my-router 
