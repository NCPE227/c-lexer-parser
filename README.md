This project contains everything you need to make a lexer/parser which can detect regular expressions. 
The tool can be used from the command line. The tool was developed with Ubuntu 24.04 as the OS, and
usage on other systems may result in your mileage varying.

# Requirements
- [Flex](https://github.com/westes/flex)
- [Bison](https://github.com/akimd/bison)
- C programming language

# Operation
You can build the executable by running `make` from the root folder. Once you've done that,
you can use the tool from the command line like so: `./parse <path_to_file>`. It will likely
require elevated user status.

If you'd like to perform a bulk test, a bash script is included in the project along with a suite of valid and invalid
regular expressions. Running `bash test.sh` will run and output all test values. This is done automatically, and will
even generate your executable if it doesn't already exist. If you add more tests, it should pick those up automatically.

# Licensure
This project is licensed under [GNU 3.0](https://www.gnu.org/licenses/gpl-3.0.en.html) and is provided "AS IS".