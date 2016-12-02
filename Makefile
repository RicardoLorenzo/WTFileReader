CC=g++
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS=-I. -g -Wall
LIBS=-lm

#%.o: $(DEPS)
#	$(CC) -c $(LIBS) -o $@ %.cc $(CFLAGS)

wtfile: clean
	$(CC) ${ARGS} -std=c++11 -o wtfile wt_file_block.cpp wt_file_reader.cpp wt_file.cpp -I. $(LIBS)


.PHONY: clean

clean:
	rm -f *.o wtfile
