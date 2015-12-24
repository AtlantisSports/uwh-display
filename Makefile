
VERBOSE ?= 0

CXX_FLAGS=-O0 -g -std=c++11

RGB_INCDIR=matrix/include
RGB_LIBDIR=matrix/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

SRC_FILES=$(wildcard src/*.cpp)
OBJ_FILES=$(addprefix obj/,$(notdir $(SRC_FILES:.cpp=.o)))

ifeq ($(VERBOSE),0)
  V := @
else
  V :=
endif

define colorecho
	@tput setaf 5
	@echo $1
	@tput sgr0
endef

.PHONY: all
all: bin/uwh-display

.PHONY: clean
clean:
	$(call colorecho, "Cleaning: obj/")
	$(V)rm -rf obj/
	$(call colorecho, "Cleaning: bin/")
	$(V)rm -rf bin/

.PHONY: mrproper
mrproper: clean
	$(call colorecho, "Cleaning: $(RGB_LIBDIR)")
	$(V)$(MAKE) -C $(RGB_LIBDIR) clean

.PHONY: $(RGB_LIBRARY)
$(RGB_LIBRARY):
	$(call colorecho, "Building: lib$(RGB_LIBRARY_NAME).a...")
	$(V)$(MAKE) -C $(RGB_LIBDIR)

bin/uwh-display: $(OBJ_FILES) $(RGB_LIBRARY)
	$(call colorecho, "Linking: $@")
	$(V)mkdir -p bin
	$(V)$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $@ $(LDFLAGS)

obj/%.o: src/%.cpp
	$(call colorecho, "Compiling: $@")
	$(V)mkdir -p obj
	$(V)$(CXX) -I$(RGB_INCDIR) $(CXX_FLAGS) -c -o $@ $<

