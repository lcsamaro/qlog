/* MIT License
 * 
 * Copyright (c) 2021 Lucas Amaro
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
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

TEST_CASE( "Pareser correctly ignores some lines", "[parser]" ) {
	auto r = parse_string(
		"20:37 InitGame: abc\n"
		" 20:37 ----------------\n"
		"20:37 InitGame: abc"
		);
	REQUIRE( r.size() == 2 );
}

TEST_CASE( "Parser ignores malformed lines", "[parser]" ) {
	auto r = parse_string("hello world\n\r\r\n\n");
	REQUIRE( r.size() == 0 );
}

TEST_CASE( "Parser works correctly without blanks at begining", "[parser]" ) {
	auto r = parse_string(
		"20:37 InitGame: abc\n"
		"20:37 InitGame: abc");
	REQUIRE( r.size() == 2 );
}

TEST_CASE( "Parser works correctly with bigger times", "[parser]" ) {
	auto r = parse_string(
		"88820:37 InitGame: abc\n"
		"   88820:37 InitGame: abc");
	REQUIRE( r.size() == 2 );
}

TEST_CASE( "Parser works correctly without spaces after event type", "[parser]" ) {
	auto r = parse_string(
		"88820:37 InitGame:abc\n"
		"   88820:37 InitGame: abc");
	REQUIRE( r.size() == 2 );
}

TEST_CASE( "Parser works correctly with multiple spaces after time", "[parser]" ) {
	auto r = parse_string(
		"88820:37      InitGame:abc\n"
		"   88820:37    InitGame: abc");
	REQUIRE( r.size() == 2 );
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

