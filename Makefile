
SOURCE_DIR = src/

SOURCE_FILES = src/Processor.cc src/ProcessorUtil.cc src/Register8bit.cc src/Register16bit.cc src/Memory.cc

TEST_DIR = tests/
TEST_SOURCE = tests/TestUtils.cc tests/TestAddition.cc tests/TestCPUFunctions.cc tests/TestRegister8bit.cc tests/TestRegister16bit.cc
TEST_MAIN = $(TEST_DIR)test_main.cc

CCFLAGS = -g -std=c++17 -Wall -Wextra -Weffc++ -pedantic -Wsuggest-attribute=const

OUT_FILE = emulator
OUT_FILE_TEST = $(OUT_FILE)_test

GCC_FILTER = gccfilter -c

all:
	$(GCC_FILTER) g++ $(CCFLAGS) -o $(OUT_FILE) $(SOURCE_DIR)main.cc $(SOURCE_FILES) -I$(SOURCE_DIR)

test:
	$(GCC_FILTER) g++ $(CCFLAGS) -o $(OUT_FILE_TEST) $(TEST_MAIN) $(SOURCE_FILES) $(TEST_SOURCE) -I$(TEST_DIR) -I$(SOURCE_DIR)
	

clean:
	$(RM) $(OUT_FILE)
	$(RM) $(OUT_FILE_TEST)
	$(RM) *.o
	$(RM) *.out
