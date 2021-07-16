#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <functional>
#include <utility>
#include <vector>
#include <iostream>

#include "parser.h"

struct result {
	std::string event;
	std::string text;

	result(std::string event, std::string text) :
		event(event), text(text) {}
};

std::vector<result> parse_string(std::string s) {
	size_t cur = 0;
	auto reader = [&] () -> int {
		if (cur >= s.size()) return EOF;
		return s[cur++];
	};

	std::vector<result> v;
	auto cb = [&] (const char *event, const char *data) {
		v.push_back(result(event, data));
	};

	parse_quake_log(reader, cb);

	return v;
}

TEST_CASE( "Parser ignores hyphenetade lines", "[parser]" ) {
	auto r = parse_string(" 20:37 ----------------");
	REQUIRE( r.size() == 0 );
}

TEST_CASE( "Parser ignores malformed lines", "[parser]" ) {
	auto r = parse_string("hello world\n\r\r\n\n");
	REQUIRE( r.size() == 0 );
}

TEST_CASE( "Parser works correctly for InitGame", "[parser]" ) {
	auto r = parse_string(
		" 20:37 InitGame: abc\n"
		" 20:37 InitGame: abc");
	REQUIRE( r.size() == 2 );
	REQUIRE( r.front().event == "InitGame" );
	REQUIRE( r.front().text == "" ); // should be empty
	REQUIRE( r.back().event == "InitGame" );
	REQUIRE( r.back().text == "" ); // should be empty
}

TEST_CASE( "Parser works correctly for ShutdownGame", "[parser]" ) {
	auto r = parse_string(
		" 20:37 ShutdownGame: abc\n"
		" 20:37 ShutdownGame: abc");
	REQUIRE( r.size() == 2 );
	REQUIRE( r.front().event == "ShutdownGame" );
	REQUIRE( r.front().text == "" ); // should be empty
	REQUIRE( r.back().event == "ShutdownGame" );
	REQUIRE( r.back().text == "" ); // should be empty
}

TEST_CASE( "Parser works correctly for Kill", "[parser]" ) {
	auto r = parse_string(
		" 20:37 Kill: 1022 2 22: aaa\n"
		" 20:37 Kill: 1 1 1: bbb\n");
	REQUIRE( r.size() == 2 );
	REQUIRE( r.front().event == "Kill" );
	REQUIRE( r.front().text == "aaa" );
	REQUIRE( r.back().event == "Kill" );
	REQUIRE( r.back().text == "bbb" );
}

