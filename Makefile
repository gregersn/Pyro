#
#	TODO: Stuff that I need to do
#

SRCDIR := src
BUILDDIR := build
TARGET := libpyro.so
DESTDIR := /usr/local

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
DEPS := $(OBJECTS:.o=.d)

# -g Produce debugging information in the operating system's native format
CXXFLAGS = -Wall -std=c++17 -I include -I subprojects/eigen-3.4.0
LDFLAGS = -L. -L./build/src
LDLIBS = -lpyro

PYRO_CXXFLAGS := -g -Wall `pkg-config --cflags cairo sdl2` -std=c++17 -I include -fPIC
PYRO_LDFLAGS = -shared
LIB := `pkg-config --libs cairo sdl2` -lfreeimage -lstdc++
# INC := -I include

ifeq ($(OS), Windows_NT)
	LIB += -LC:/msys64/mingw64/lib -lmingw32 -pthread -L lib
	PYRO_LDFLAGS += --enable-runtime-pseudo-reloc
else
endif

$(TARGET): $(OBJECTS)
	@mkdir -p lib
	$(CXX) $(PYRO_LDFLAGS) -o $(TARGET) $^ $(LIB)

-include $(DEPS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	$(CXX) $(PYRO_CXXFLAGS) $(INC) -c -o $@ $<

.PHONY: clean
clean:
	@echo "Cleaning...";
	$(RM) -r $(BUILDDIR) $(TARGET)
	$(MAKE) -C tests clean

.PHONY: test
test: $(TARGET)
	$(MAKE) -C tests
	./tests/alltests

.PHONY: install
install: $(TARGET)
	mkdir -p $(DESTDIR)/lib
	mkdir -p $(DESTDIR)/include/pyro
	cp $(TARGET) $(DESTDIR)/lib/$(TARGET)
	cp include/*.h $(DESTDIR)/include/pyro/

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)/lib/$(TARGET)
	rm -f $(DESTDIR)/include/pyro


%.o: %.cpp                                                                                          
	$(CXX) -MMD -MP $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<                                           
%.o: %.cc                                                                                              
	$(CXX) -MMD -MP $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

%.d: %.cpp
	@$(CXX) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

