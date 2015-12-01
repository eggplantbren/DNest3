CFLAGS = -O3 -Wall -Wextra -ansi -pedantic
LIBS =  -ldnest3 -lgsl -lgslcblas -lboost_system -lboost_thread -ljulia

default:
	g++ $(CFLAGS) -c *.cpp
	g++ -L/usr/local/lib/julia -o main *.o $(LIBS)
	rm -f *.o

