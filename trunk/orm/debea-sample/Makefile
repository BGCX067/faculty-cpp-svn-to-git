# source files.
SRC =  	src/BlogManagerDebea.cpp

OUT = debea-sample
OUTDIR = Debug

CXXFLAGS=-Wall

# include directories
INCLUDES = -Idba/ -Isqlite3/

LDFLAGS = -ldba -lsqlite3 -ldl -lpthread

# library paths
LIBS = -Llib

all :
	g++ $(INCLUDES) $(CXXFLAGS) $(LIBS) $(SRC) -o $(OUTDIR)/$(OUT) $(LDFLAGS)
