#
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h

RELEASEFLAGS = -std=c++11 -O3 -Wall -mavx -msse4.2 -Wextra  -fexceptions -fPIC
DEBUGFLAGS = -std=c++11 -g3 -Wall -mavx -msse4.2   -Wextra -fexceptions -fPIC

all: runningmaxmin  unit

debug: runningmaxmin.h runningmaxmin.cpp
	$(CXX) $(DEBUGFLAGS) -o runningmaxmin runningmaxmin.cpp

runningmaxmin : runningmaxmin.h runningmaxmin.cpp
	$(CXX) $(RELEASEFLAGS) -o runningmaxmin runningmaxmin.cpp
unit : runningmaxmin.h unit.cpp
	$(CXX) $(RELEASEFLAGS) -o unit unit.cpp



clean:
	rm -f *.o runningmaxmin unit
