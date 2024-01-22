YFLAGS = -dy
TEST_OUTPUT = /tmp/test_output.txt
BINARIES = dropword mol

all: $(BINARIES)

dropword: dropword.cc
	g++ $^ -o $@

mol: lexer.o parser.o 
	cc $^ -o $@

lexer.o: parser.o

# run mol and compare against known output
test: mol
	./mol < test_input.txt >$(TEST_OUTPUT)
	diff test_output.txt $(TEST_OUTPUT)

clean:
	rm -f *.o y.tab.h $(BINARIES) $(TEST_OUTPUT)
