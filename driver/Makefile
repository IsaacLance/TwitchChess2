CXX = g++
CFLAGS = -std=c++14

%.o : %.cpp
	$(CXX) -c $(CFLAGS) $<

driver:  
	g++ -std=c++14 -o driver chessdriver.cpp consumer.cpp -pthread -lPython

test: test.o consumer.o
	$(CXX) -o $@ $^ -lm -lPython


clean:
	$(RM) driver $(OBJECTS) *.pyc *.o

Reset:
	make clean
	make driver 
	./driver