qlog: qlog.cpp
	g++ -std=c++11 -O2 qlog.cpp -o qlog

test: test.c
	gcc -O2 test.c -o test

