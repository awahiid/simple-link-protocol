CXX ?= g++
TARGET := build/simple-link-protocol
SRC := $(wildcard *.cpp)

CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2 -I.
LDLIBS := linkLayer.a -lpcap -pthread

.PHONY: all clean check-deps run

all: check-deps $(TARGET)

run: all
	./$(TARGET)

check-deps:
	@mkdir -p build
	@if [ ! -f /usr/include/pcap.h ] && [ ! -f /usr/include/pcap/pcap.h ]; then \
		echo "Falta pcap.h. Instala libpcap-dev (o equivalente) para compilar." >&2; \
		exit 2; \
	fi

$(TARGET): $(SRC) linkLayer.a
	$(CXX) $(CXXFLAGS) $(SRC) $(LDLIBS) -o $@

clean:
	rm -rf build
	rm -f *.o

