CXX = g++
CFLAGS = -std=c++14

%.o : %.cpp
	$(CXX) -c $(CFLAGS) $<

test.exe:  
	g++ -std=c++14 -o driver chessdriver.cpp consumer.o -pthread -lPython


clean:
	$(RM) test.exe $(OBJECTS) *.pyc *.o