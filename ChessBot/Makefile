CXX = g++
CFLAGS = -std=c++14 -g -Wall

%.o : %.cpp
	$(CXX) -c $(CFLAGS) $<

test.exe: test.o
	$(CXX) -o $@ $^ -lm -I/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/site-packages/selenium -lPython
testSel:
	python3 testSel.py

chessBot:
	python3 moveMaker.py