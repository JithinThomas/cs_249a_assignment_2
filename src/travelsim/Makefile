SRC=../../src
CPPFLAGS = -I$(SRC)
CXX = g++
CXXFLAGS = \
    -g -std=c++11 \
    -Wall \
    -Wno-unused-function

client1: always
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o client1 $(SRC)/travelsim/client1.cxx

client2: always
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o client2 $(SRC)/travelsim/client2.cxx

clean:
	rm -f client1 client2 *.o *~

always:
