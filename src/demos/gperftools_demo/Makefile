CXX = g++
CPPFLAGS += -I/usr/local/include -I../../../include 
CXXFLAGS += -std=c++11
LDFLAGS += -L/usr/local/lib -ltcmalloc -lprofiler -lpthread

SIGNAL_CODE_PATH = ../../nsky_master
vpath %.cc $(SIGNAL_CODE_PATH)

signal_example: signal_demo.o signal_handle.o
	$(CXX) $^ $(LDFLAGS) $(CPPFLAGS) -o $@

clean:
	rm -rf *.o signal_example 
