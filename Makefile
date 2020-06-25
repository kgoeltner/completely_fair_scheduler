# Karl Goeltner
# 917006087
# ECS 36C - 05/22/2020
CXX = g++
CXXFLAGS += -std=c++11 -Wall -Werror

all: test_multimap cfs_sched

# PROGRAM COMPILATION

test_multimap: test_multimap.o multimap.h
	$(CXX) $(CXXFLAGS) test_multimap.cc -o test_multimap -pthread -lgtest

cfs_sched: cfs_sched.o multimap.h
	$(CXX) $(CXXFLAGS) cfs_sched.cc -o cfs_sched


# STYLE CHECK

lint_test_multimap:
	/home/cs36cjp/public/cpplint/cpplint test_multimap.cc

lint_multimap:
	/home/cs36cjp/public/cpplint/cpplint multimap.h

lint_cfs:
	/home/cs36cjp/public/cpplint/cpplint cfs_sched.cc

clean:
	rm -f test_multimap cfs_sched *.o
