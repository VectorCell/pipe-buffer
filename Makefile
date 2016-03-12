CSTD   := c99
CPPSTD := c++11

ifeq "$(CXX)" "g++"
	GCCVERSIONLT48 := $(shell expr `gcc -dumpversion` \< 4.8)
	ifeq "$(GCCVERSIONLT48)" "1"
		CPPSTD := c++0x
	endif
endif

CFLAGS   := -pedantic -std=$(CSTD) -Wall -Werror -O3
CPPFLAGS := -pedantic -std=$(CPPSTD) -Wall -Werror -O3
LIBFLAGS := 

all : buffer

buffer : buffer.cc
	$(CXX) $(CPPFLAGS) -o buffer buffer.cc $(LIBFLAGS)

test : all
	md5sum < buffer.cc
	./buffer < buffer.cc | md5sum
	./buffer 64k < buffer.cc | md5sum

clean :
	rm -f *.d
	rm -f *.o
	rm -f buffer

-include *.d
