
SOURCE_DIR = src/

SOURCE_FILES = src/Processor.cc src/RegisterGet.cc src/RegisterSet.cc src/Register8bit.cc src/Register16bit.cc src/Memory.cc

TEST_DIR = tests/
TEST_SOURCE = tests/TestUtils.cc tests/TestAddition.cc tests/TestCPUFunctions.cc tests/TestRegister8bit.cc
TEST_MAIN = $(TEST_DIR)test_main.cc

CCFLAGS = -g -std=c++17 -Wall -Wextra -pedantic -Wsuggest-attribute=const

OUT_FILE = emulator



all:
	g++ $(CCFLAGS) -o $(OUT_FILE) $(SOURCE_DIR)main.cc $(SOURCE_FILES) -I$(SOURCE_DIR)

test:
	g++ $(CCFLAGS) -o $(OUT_FILE)_test $(TEST_MAIN) $(SOURCE_FILES) $(TEST_SOURCE) -I$(TEST_DIR) -I$(SOURCE_DIR)
	

clean:
	$(RM) $(OUT_FILE)
	$(RM) *.o	
	$(RM) *.out
