# Set vars for compile args and program name
CC=g++ -std=c++11
CC_FLAGS=-Wall -Werror -ansi -pedantic
EXEC=rshell

# Set vars for source and output folders
# main code folder
SRCDIR=src
# output folder
BINDIR=bin
# output folder for the .o files
# we want to keep these because they can be used
# to determine which files need to be recompiled
# do to changes. This avoids recompiling the entire
# project every time
LIBDIR=$(BINDIR)/libs

# make a list of all the files that need to be compiled
# notice sub-folders need to be specified
# the '\' just means the statements continues on the next line
# which is important a bash need to resolve the vars and append
# it to the previous line
SOURCES:=$(wildcard $(SRCDIR)/*.cpp)

# same thing is done for the object & dependency files
# the difference here is that it changes the file suffix
# before adding it to the list
#DEPENDS: speeds up compile time by only recompiling the changed file
OBJECTS:=$(SOURCES:$(SRCDIR)/%.cpp=$(LIBDIR)/%.o)
DEPENDS:=$(OBJECTS:.o=.d) 

# this case defines what to do if the user just calls 'make'
# instead of 'make rshell'
all: $(EXEC)

# this is the task to build the final rshell program from
# the object files. because this depends on the object files
# it will call for each of them to be built first which is handled
# by the tasks below
$(EXEC) : $(OBJECTS)
	$(CC) -o $(BINDIR)/$@ $^

# this tells bash to read all the dependency info that is saved
# after the first build. it then makes it so the only files compiled
# are the ones that have changes made to them or files that depend on
# files that where changed
-include $(DEPENDS)

# simple compile tasks. there sadly needs to be a separate task
# for each sub-folder.
$(LIBDIR)/%.o : $(SRCDIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(CC_FLAGS) -c $< -o $@
	$(CC) $(CC_FLAGS) $< -MM -MT $@ > $(@:.o=.d)
# fist line makes the destination folder if it does not exist
# otherwise it would throw an error if './bin' or './bin/libs/' DNE

# second line is the standard compile .cpp file into .o
# note that '$<' is the input file name and '$@' is the output file name

# the third line save our dependency info for future builds

# sub-folder './src/tasks/'

# task to delete all .o files and the complied program
.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXEC)

# task to delete all the dependency files. useful to force a
# full project rebuild
.PHONY: cleandep
cleandep:
	rm -f $(DEPENDS)