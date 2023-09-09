NAME = Project
CC = gcc -std=c++17 -g -lstdc++ -Wall -Werror -Wextra
GCOVFLAGS = -fprofile-arcs -ftest-coverage
SOURSES = ./Includes/models/s21_*.cc
OBJECT = $(patsubst ./Includes/models/%*.cc, %*.o, ${SOURSES})
OS := $(shell uname -s)

ifeq ($(OS), Darwin)
 CC += -D OS_MAC
 ADD_LIB =
 TEST_FLAGS =-lm -lgtest -lgmock -pthread
endif
ifeq ($(OS), Linux)
 CC += -D OS_LINUX
 ADD_LIB = -lcheck -lm -lrt -lpthread
 TEST_FLAGS =-lpthread -lcheck -pthread -lrt -lm
endif

all: clean install

dvi:
	open readme.html

$(NAME): $(OBJECT)
	$(CC) $(OBJECT) -o $(NAME)

s21_SmartCalc.a:$(SOURSES)
	$(CC) -c -lcheck $(SOURSES)
	@ar rcs s21_SmartCalc.a $(OBJECT)

test: Tests/school21_unitTest.cc s21_SmartCalc.a
	$(CC) $< s21_SmartCalc.a $(TEST_FLAGS) -o g-test
	mv *.o ./obj
	mv *.a ./obj
	mv *.dSYM ./obj
	./g-test

gcov_report: s21_SmartCalc.a
	@gcc -lstdc++ $(GCOVFLAGS) ./Tests/school21_unitTest.cc -lcheck $(SOURSES) $(TEST_FLAGS) -o GcovReport
	@./GcovReport
	@lcov -t "GcovReport" -o GcovReport.info -c -d .
	@genhtml -o report GcovReport.info
	@open ./report/index-sort-f.html
	@rm -rf ./*.gcno ./*.gcda ./GcovReport.*
	mv *.o ./obj
	mv *.a ./obj

clean:
	@rm -rf *.{a,o,dSYM,out}
	@rm -rf ./obj/*.{a,o,dSYM,out}
	@rm -rf $(EXECUTABLE)
	@rm -rf gcov_report
	@rm -rf report
	@rm -rf CPPLINT.cfg
	@rm -rf *.gcda
	@rm -rf *.gcno
	@rm -rf *.info
	@rm -rf test
	@rm -rf ../Archive_SmartCalc_v2.0
	@rm -rf *tgz
	@rm -rf build
	@rm -rf gcov_test
	@rm -rf ../Archive_SmartCalc_v2.0.zip
	@rm -rf ../Archive_SmartCalc_v2.0.tgz
	@rm -rf unit.out.dSYM
	@rm -rf g-test
	@rm -rf GcovReport

install:
	make clean
	mkdir build
	cd Includes/view && qmake && make && make clean && rm Makefile && mv SmartCalc.app build && mv build ../../
	cd build/Contents/MacOS && ./SmartCalc

uninstall:
	@rm -rf build*

open:
	cd build/Contents/MacOS && ./SmartCalc

open_leaks:
	cd build/Contents/MacOS && CK_FORK=no leaks --atExit -- ./SmartCalc

dist:
	rm -rf ../Archive_SmartCalc_v2.0/
	mkdir ../Archive_SmartCalc_v2.0/
	mkdir ../Archive_SmartCalc_v2.0/src
	cp build/Contents/MacOS/SmartCalc ../Archive_SmartCalc_v2.0/src/
	cd .. && tar cvzf Archive_SmartCalc_v2.0.tgz ./Archive_SmartCalc_v2.0
	@rm -rf ../Archive_SmartCalc_v2.0

check: leaks
	@echo -------------------STYLE_CHECK--------------------
	@clang-format -style=Google -n ./Includes/controllers/*.h ./Includes/models/*.h ./Includes/models/*.cc ./Includes/view/*.h ./Includes/view/*.cpp ./Tests/*.cc

leaks: Tests/school21_unitTest.cc s21_SmartCalc.a
	@echo -------------------MEMORY_LEAKS-------------------
	$(CC) $< s21_SmartCalc.a $(TEST_FLAGS) -o g-test
	mv *.o ./obj
	mv *.a ./obj
	mv *.dSYM ./obj
	CK_FORK=no leaks --atExit -- ./g-test

correct:
	@clang-format -style=Google -i ./Includes/controllers/*.h ./Includes/models/*.h ./Includes/models/*.cc ./Includes/view/*.h ./Includes/view/*.cpp ./Tests/*.cc

valgrind_check: school21_unitTest_tree.cc s21_SmartCalc.a
	$(CC) $< s21_SmartCalc.a $(TEST_FLAGS) -o g-test
	mv *.o ./obj
	mv *.a ./obj
	mv *.dSYM ./obj
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all  --track-origins=yes --verbose ./g-test

rebuild:
	$(MAKE) clean
	$(MAKE) all

.PHONY: all, install, uninstall, clean, dvi, dist, tests, gcov_report
