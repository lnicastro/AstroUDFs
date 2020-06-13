##
# Some astro functions as MySQL UDF functions (see udf_astro.c).
# 
# 17/12/2009: LN@INAF-OAS,  Last change: 13/06/2020
##

MAKE = make
CC= gcc
CCPP = g++
CFLAGS= -c -I. -O2 -Wall -Wno-parentheses -fPIC
CPPFLAGS= -c -I. -O2 -Wall -Wno-parentheses -fPIC
LDFLAGS = -O2
MYCFLAGS = `mysql_config --cflags`
MYLDFLAGS = `mysql_config --libs`
BINDIR = .
L = astro
LIB = lib$(L).a
LIBS = -L. -l$(L) -lm
LIBUDF = udf_$(L).so

VER = v05a_`/bin/date +"%Y%m%d"`
dist = $(L)_$(VER)


LINK.c = $(CC) $(LDFLAGS)
LINK.cpp = $(CCPP) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $<


UDFOBJS = udf_astro.o radec2glgb.o radec2eleb.o pm_radec.o utils.o

LIBFILES = $(UDFOBJS:.o=.c)

EXTRAFILES = Makefile udf_astro.h

EXES = eq2ga eq2ec test_ecl2000 test_pm

MAIN = $(EXES:=.c)

default: $(LIB) udf
all: $(LIB) $(EXES) udf

exe: $(EXES)
udf: $(UDFOBJS) $(LIBUDF)


# MySQL UDFs
udf_astro.o: udf_astro.c
	$(CC) $(CFLAGS) $(MYCFLAGS) udf_astro.c

test_pm: test_pm.o $(LIB)
	$(LINK.c) -o $@ $@.o $(LIBS)

test_az: test_az.o $(LIB)
	$(LINK.c) -o $@ $@.o $(LIBS)

eq2ec: eq2ec.o $(LIB)
	$(LINK.c) -o $@ $@.o $(LIBS)

eq2ga: eq2ga.o $(LIB)
	$(LINK.c) -o $@ $@.o $(LIBS)

$(LIB): $(UDFOBJS)
	ar rv $@ $?
	ranlib $@

$(LIBUDF): $(UDFOBJS)
	$(CC) -shared $(LDFLAGS) $(MYLDFLAGS) -o $@ $(UDFOBJS) $(LIBS) -l$(L)


install: $(LIB) $(LIBSO)
	@pd="`mysql_config --plugindir`"; \
	echo "Copying $(LIBUDF) to MySQL plugin dir $$pd"; \
	cp -a $(LIBUDF) $$pd

uninstall:
	@pd="`mysql_config --plugindir`"; \
	echo "Removing $(LIBUDF) from MySQL plugin dir $$pd"; \
	rm $$pd/ $(LIBUDF)

dist:
	tar zcvf $(dist).tar.gz $(LIBFILES) $(MAIN) $(EXTRAFILES)

tar:
	tar cvf $(dist).tar $(LIBFILES) $(MAIN) $(EXTRAFILES)

clean:
	rm -f *.o $(LIB) $(LIBUDF) $(EXES)
