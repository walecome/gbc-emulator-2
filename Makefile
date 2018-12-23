
SOURCE_DIR = src/

SOURCE_FILES = src/Processor.cc src/Register8bit.cc src/Register16bit.cc

CCFLAGS = -g -std=c++17 -Wall -Wextra -pedantic -Wsuggest-attribute=const

OUT_FILE = emulator


all:
	g++ $(CCFLAGS) -o $(OUT_FILE) $(SOURCE_DIR)main.cc $(SOURCE_FILES) -I$(SOURCE_DIR)


clean:
	$(RM) $(OUT_FILE)
	$(RM) *.o	
	$(RM) *.out
