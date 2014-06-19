CXX = g++

all:
	@mkdir -p bin
	$(CXX) -Wall -Wextra -O3 -frtti -std=c++11 ./../Source/Main.cpp ./../Source/Salsa20.cpp -o bin/salsa20

clean:
	@rm -rf bin

install:
	cp bin/salsa20 /usr/local/bin/salsa20

uninstall:
	rm /usr/local/bin/salsa20
