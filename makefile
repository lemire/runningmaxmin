#
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h

RELEASEFLAGS =  -O3 -Wall -Wextra  -fexceptions -fPIC
DEBUGFLAGS =  -g3 -Wall   -Wextra -fexceptions -fPIC

all: runningmaxmin 

debug: runningmaxmin.h runningmaxmin.cpp
	g++ $(DEBUGFLAGS) -o runningmaxmin runningmaxmin.cpp

runningmaxmin : runningmaxmin.h runningmaxmin.cpp
	g++ $(RELEASEFLAGS) -o runningmaxmin runningmaxmin.cpp


clean:
	rm -f *.o runningmaxmin
