# qlog - quake log parser
This program parses a quake server log file and outputs human-readable matches data in yaml format.

Currently uses fgetc for file reading, but could easily be changed to some other alternatives. Nice comparison of some approaches: https://nothings.org/computer/lexing.html.

## Building
`$ make`

## Running
`$ ./qlog qgames.log`

## Tests
```
$ make test
$ ./test
```

## External libraries used
* [Catch2](https://github.com/catchorg/Catch2) for tests

