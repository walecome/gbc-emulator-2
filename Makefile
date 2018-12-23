
SOURCE_DIR = src/

define SOURCE_FILES
src/Processor.cc
endef

CCFLAGS = -g -std=c++17 -Wall -Wextra -pedantic -Wsuggest-attribute=const -Wshadow

OUT_FILE = emulator


all:
	g++ $(CCFLAGS) -o $(OUT_FILE) $(SOURCE_DIR)main.cc $(SOURCE_FILES) -I$(SOURCE_DIR)


clean:
	$(RM) $(OUT_FILE)
	$(RM) *.o	
	$(RM) *.out
