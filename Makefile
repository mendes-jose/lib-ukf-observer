# Define executable name
BIN = build/test

# Define source files
CPP_SRCS =  src/common.cpp\
		src/sensor.cpp\
		src/sysmodelparser.cpp\
		src/observer.cpp\
		src/main.cpp

# Define header file paths
INCPATH = -Ihdr/

# Define the -L library path(s)
LDFLAGS =

# Define the -l library name(s)
LIBS = /usr/local/lib/libmuparserd.so -lrt

# Only in special cases should anything be edited below this line
OBJS      = $(CPP_SRCS:.cpp=.o)
CXXFLAGS  = -c -Wall -ansi -pedantic
DEP_FILE  = .depend


.PHONY = all clean distclean


# Main entry point
#
all: depend $(BIN)


# For linking object file(s) to produce the executable
#
$(BIN): $(OBJS)
	@echo Linking $@
	@$(CXX) $^ $(LDFLAGS) $(LIBS) -o $@


# For compiling source file(s)
#
.cpp.o:
	@echo Compiling $<
	@$(CXX) $(CXXFLAGS) $(INCPATH) $< -o $@

# For cleaning up the project
#
clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) $(BIN)
	$(RM) $(DEP_FILE)

# For documenting
#
doc:
	doxygen docs/doxy_config ; cd latex ; sh docs/fixtex.sh ; $(MAKE)
	mv -f src/latex/refman.pdf src/html docs/
	mv -f src/html docs/	
	$(RM) src/latex src/html

# For determining source file dependencies
#
depend: $(DEP_FILE)
	@touch $(DEP_FILE)

$(DEP_FILE):
	@echo Generating dependencies in $@
	@-$(CXX) -E -MM $(CXXFLAGS) $(INCPATH) $(CPP_SRCS) >> $(DEP_FILE)

ifeq (,$(findstring clean,$(MAKECMDGOALS)))
ifeq (,$(findstring distclean,$(MAKECMDGOALS)))
-include $(DEP_FILE)
endif
endif
