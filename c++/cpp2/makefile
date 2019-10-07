# executable files for this directory
OBJECTS = tst-bind.exe
# tells make to use the file "../GNU_makefile_template", which
# defines general rules for making .o and .exe files
include ../GNU_makefile_template

# 7 has Sales_data files, 6 has make_plural
LOCFLAGS = 

querymain.exe: querymain.o TextQuery.o
	$(CC) $(CCFLAGS) $(LOCFLAGS) querymain.o TextQuery.o \
			-o querymain.exe

