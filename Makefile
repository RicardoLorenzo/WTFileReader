CC=g++
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS=-I. -g -Wall
LIBS=-lm
SOURCES=wt_file_block.cpp wt_file_reader.cpp wt_file.cpp wt_utils.cpp

BUILD_DIR=build
BIN=${BUILD_DIR}/wtfile

#%.o: $(DEPS)
#	$(CC) -c $(LIBS) -o $@ %.cc $(CFLAGS)

wtfile: clean
	mkdir ${BUILD_DIR}
	$(CC) ${ARGS} -std=c++11 -o ${BIN} $(SOURCES) -I. $(LIBS)


.PHONY: clean

format:
	clang-format -style=file -i *.cpp include/*.h

clean:
	rm -rf ${BUILD_DIR}
