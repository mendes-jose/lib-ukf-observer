# Define executable name
BIN = test

# Define source files
CPP_SRCS =  common.cpp\
		sensor.cpp\
		sysmodelparser.cpp\
		observer.cpp\
		main.cpp

# Define header file paths
INCPATH = -I./

# Define the -L library path(s)
LDFLAGS =

# Define the -l library name(s)
LIBS = /usr/local/lib/libmuparserd.so -lrt

# Only in special cases should anything be edited below this line
OBJS      = $(CPP_SRCS:.cpp=.o)
CXXFLAGS  = -Wall -ansi -pedantic
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
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) $<

# For cleaning up the project
#
clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) $(BIN)
	$(RM) $(DEP_FILE)


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
