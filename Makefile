SHELL = /bin/sh

prefix = .
exec_prefix = ${prefix}
srcdir = ${prefix}/src
bindir = ${exec_prefix}/bin

vpath %.cpp ${srcdir} 

# Insert your program definition(s) here
# Example:
#    PROGRAMS = foo
#    foo_SRCS = fooer.c error_msg.c
#    all_LDFLAGS = -pthread

PROGRAMS = solver

solver_SRCS = main.cpp print_board.cpp \
	simple_solver.cpp

all_LDFLAGS =

# End of program definitions

SRCS = ${foreach prog,${PROGRAMS},${${prog}_SRCS}}

define name_to_OBJS
${1}_OBJS = $${${1}_SRCS:%.cpp=$${bindir}/%.o}
endef

${foreach prog,${PROGRAMS},${eval ${call name_to_OBJS,${prog}}}}
OBJS = ${sort ${foreach prog,${PROGRAMS},${${prog}_OBJS}}}
EXECS = ${addprefix ${bindir}/,${PROGRAMS}}

CFLAGS = -Wall -g -O2

.PHONY: all
all: ${EXECS}

.SECONDEXPANSION:
${EXECS}: ${bindir}/%: $${$$*_OBJS}
	${CXX} -o $@ $^ ${all_LDFLAGS} ${$*_LDFLAGS}

${OBJS}: ${bindir}/%.o: %.cpp | ${bindir}
	${CXX} ${CFLAGS} -c -o $@ $^

${bindir}:
	mkdir ${bindir}
  
.PHONY: clean
clean:
	${RM} ${EXECS} ${OBJS}
