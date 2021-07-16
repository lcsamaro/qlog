qlog: qlog.cpp parser.cpp
	g++ -Wall -std=c++11 -O2 qlog.cpp parser.cpp -o qlog

test: test.cpp parser.cpp
	g++ -Wall -std=c++11 -O0 test.cpp parser.cpp -o test

test-coverage:
	g++ -Wall -std=c++11 -O0 -fprofile-arcs -ftest-coverage test.cpp parser.cpp -o test-coverage

.PHONY: run-tests
run-tests: test
	./test

.PHONY: run-coverage
run-coverage: test-coverage
	./test-coverage
	gcov parser.cpp

