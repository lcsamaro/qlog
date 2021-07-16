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
#include "parser.h"

/* using C like code for the parser, although std::function is used for callbacks */
#define skip_spaces() do { \
		ch = ' '; \
		while (ch == ' ') ch = next(); \
		if (ch == EOF) break; /* eof, line ended without all information*/ \
	} while (0)
#define skip_line() do { \
		while (ch != '\n' && ch != EOF) ch = next(); \
		if (ch == EOF) break; \
	} while (0)
#define skip_until_space() do { \
		while (ch != ' ' && ch != EOF) ch = next(); \
		if (ch == EOF) break; /* eof, line ended without all information */ \
	} while (0)

#define EVENT_SZ_MAX (64)
#define TEXT_SZ_MAX  (1024)

void parse_quake_log(std::function<int()> next,
		std::function<void(const char*, const char*)> cb) {
	for (;;) {
		char event[EVENT_SZ_MAX] = {0};
		char text[TEXT_SZ_MAX] = {0};
		int ch, i;

		skip_spaces();
		skip_until_space(); // skip time
		skip_spaces();

		// get event
		if (ch == '-') {
			skip_line();
			continue;
		}

		i = 0;
		while (ch != ':' && ch != EOF) {
			if (i < EVENT_SZ_MAX-1) { // event type too big, skip
				event[i++] = ch;
			}
			ch = next();
		}
		if (ch == EOF) break; // eof, line ended without all information
		ch = next(); // skip colon

		if (!strcmp(event, "InitGame") || !strcmp(event, "ShutdownGame")) {
			cb(event, "");
		}

		if (ch == EOF) break; // eof, line ended without all information

		if (strcmp(event, "Kill")) { // only interested in kill events, skip
			skip_line();
			continue;
		}

		// parse kill event
		while (ch != ':' && ch != EOF) {
			ch = next();
		}
		if (ch == EOF) break; // eof, line ended without all information
		ch = next(); // skip colon

		skip_spaces();

		// read log
		i = 0;
		while (ch != '\n' && ch != EOF) {
			if (i < TEXT_SZ_MAX-1 && ch != '\r') text[i++] = ch;
			ch = next();
		}
		cb(event, text);

		if (ch == EOF) break;
	}

	return false;
}
