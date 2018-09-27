CXXFLAGS = -Wall -std=c++17 -I ../include
LDFLAGS = -L..
LDLIBS = -lpyro

src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)

alltests: $(obj)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

.PHONY: clean
clean:
	rm -f $(obj)
	rm -f *.png
	rm -f *.d
	rm -f alltests

%.o: %.cpp
	$(CXX) -MMD -MP $(CXXFLAGS) -o $@ -c $<