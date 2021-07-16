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
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <string>
#include <unordered_map>

#include "parser.h"

int main(int argc, char **argv) {
	if (argc != 2) {
		puts("usage: qlog <input>");
		return 1;
	}

	/* file reader */
	FILE *f = fopen(argv[1], "rb");
	if (!f) {
		fprintf(stderr, "failed to open %s\n", argv[1]);
		return 1;
	}

	auto reader = [f] () {
		return fgetc(f);
	};

	/* aggregate output */
	unsigned current_game = 0;
	unsigned kills = 0;
	std::unordered_map<std::string, int> scores;
	std::unordered_map<std::string, int> kills_by_means;

	auto event_cb = [&] (const char *event, const char *data) {
		if (!strcmp(event, "Kill")) {
			// parse data
			std::string s(data);

			// get death cause
			auto p = s.find_last_of(' ');
			if (p == std::string::npos) return;
			auto mean = s.substr(p+1);

			// get killer/killed
			s = s.substr(0, p-2);
			p = s.find(" killed"); // not very good, but no other option
			if (p == std::string::npos) return;

			auto killer = s.substr(0, p);
			auto killed = s.substr(p + 8);

			kills++;
			kills_by_means[mean]++;

			if (killer != "<world>") {
				scores[killer]++;
			} else {
				scores[killed]--;
			}
		} else if (!strcmp(event, "InitGame")) {
			kills = 0;
			scores.clear();
			kills_by_means.clear();
		} else if (!strcmp(event, "ShutdownGame")) { // output results
			current_game++;
			printf("game_%u:\n", current_game);
			printf("  total_kills: %u\n", kills);
			printf("  players:\n");
			for (auto e : scores) {
				printf("    - %s\n", e.first.c_str());
			}
			printf("  kills:\n");
			for (auto e : scores) {
				printf("    %s: %d\n", e.first.c_str(), e.second);
			}
			printf("  kills_by_means:\n");
			for (auto e : kills_by_means) {
				printf("    %s: %d\n", e.first.c_str(), e.second);
			}
			puts("");
		}
	};

	parse_quake_log(reader, event_cb);

	fclose(f);

	return 0;
}

