CXXFLAGS += -I../../../include
CXXFLAGS += -I/home/pairs/share/project/nsky/include/nsky/common/slog/spdlog/include
CXX = g++
CPPFLAGS += -std=c++11
LDFLAGS += -pthread

slog_test:slog_test.o
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)
#slog_test.o: slog_impl.h slog_test.cc
#	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c  $<
clean:
	rm -rf *.o slog_test *.txt

