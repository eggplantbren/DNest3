CFLAGS = -O2 -Wall -Wextra -ansi -pedantic -I ../../include/
LIBS =  -ldnest3 -lgsl -lgslcblas -lboost_system -lboost_thread -L ../../lib/

default:
	g++ $(CFLAGS) -c *.cpp
	g++ -o main *.o $(LIBS)
	rm -f *.o

