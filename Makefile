.PHONY: all test

all: test

test: libhttpp.a test/main.cpp
	g++ -std=c++20 test/main.cpp -o bin/test -DCURL_STATICLIB -Lbin/ -lhttpp -lcurl -lws2_32 -lcrypt32 -lwldap32 -lnormaliz

libhttpp.a: src/httpp.cpp
	@mkdir bin -p
	g++ -c src/httpp.cpp -o bin/httpp.o -std=c++20 -DCURL_STATICLIB
	ar rcs bin/libhttpp.a bin/httpp.o
	@rm bin/httpp.o


