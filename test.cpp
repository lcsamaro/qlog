#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <functional>
#include <utility>
#include <vector>

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

TEST_CASE( "Parser works correctly", "[parser]" ) {
	REQUIRE( parse_string(" 20:37 InitGame: abc").size() == 1 );
}

