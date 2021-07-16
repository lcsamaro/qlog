qlog: qlog.cpp parser.cpp
	g++ -std=c++11 -O2 qlog.cpp parser.cpp -o qlog

test: test.cpp parser.cpp
	g++ -std=c++11 -O0 test.cpp parser.cpp -o test

