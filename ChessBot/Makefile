CXX = g++
CFLAGS = -std=c++14 -g -Wall

%.o : %.cpp
	$(CXX) -c $(CFLAGS) $<

test.exe: test.o
	$(CXX) -o $@ $^ -lm -lPython

bot: consumer.o
	$(CXX) -o $@ $^ -lm -lPython

testSel:
	python3 testSel.py

chessBot:
	python3 moveMaker.py

clean:
	$(RM) test.exe $(OBJECTS) *.pyc *.o