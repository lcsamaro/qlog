# qlog - quake log parser
This program parses a quake server log file and outputs human-readable matches data in yaml format.

Currently uses fgetc for file reading, but could easily be changed to some other alternatives. Nice comparison of some approaches: https://nothings.org/computer/lexing.html.

## Building
`$ make`

## Running
`$ ./qlog qgames.log`

## Tests
```
$ make run-test
```

## Code coverage
```
$ make run-coverage
```

## External libraries used
* [Catch2](https://github.com/catchorg/Catch2) for tests

## TODO
* Only the parser itself is tested

