#
#	TODO: Stuff that I need to do
#

SRCDIR := src
BUILDDIR := build
TARGET := libpyro.so

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

# -g Produce debugging information in the operating system's native format
CXXFLAGS = -Wall -std=c++14 -I include
LDFLAGS = -L.
LDLIBS = -lpyro

PYRO_CXXFLAGS := -g -Wall `pkg-config --cflags cairo` -std=c++14 -I include
PYRO_LDFLAGS = -shared
LIB := `pkg-config --libs cairo` -lfreeimage -lstdc++
# INC := -I include

ifeq ($(OS), Windows_NT)
	LIB += -LC:/msys64/mingw64/lib -lmingw32 -pthread -L lib
	PYRO_LDFLAGS += --enable-runtime-pseudo-reloc
else
endif

$(TARGET): $(OBJECTS)
	@mkdir -p lib
	$(CXX) $(PYRO_LDFLAGS) -o $(TARGET) $^ $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	$(CXX) $(PYRO_CXXFLAGS) $(INC) -c -o $@ $<

clean:
	@echo "Cleaning...";
	$(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean

%.o: %.cpp                                                                                          
	$(CXX) -MMD -MP $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<                                           
%.o: %.cc                                                                                              
	$(CXX) -MMD -MP $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<
DEPS=$(OBJS:.o=.d)
-include $(DEPS)