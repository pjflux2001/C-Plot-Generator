#!/bin/bash

total_tests=5
tests_passed=0
tests_failed=0
tests_not_covered=0

testsCounter() {
	if [ $1 -eq 0 ]
	then
		tests_passed=$((tests_passed+1))
	elif [ $1 -eq 1 ]
	then
		tests_failed=$((tests_failed+1))
	else
		tests_not_covered=$((tests_not_covered+1))
	fi
	echo ""
	echo "----------------------------------------"
	echo ""
}

echo "Test 1"
echo "Running g++ sol.cpp; ./a.out input.dat pT 20 0 "
g++ sol.cpp; ./a.out input.dat pT 20 0
testsCounter $?

echo "Test 2"
echo "Running g++ sol.cpp; ./a.out input.dat pT 20 0 100 50 "
g++ sol.cpp; ./a.out input.dat pT 20 0 100 50
testsCounter $?

echo "Test 3"
echo "Running g++ sol.cpp; ./a.out input.dat randomIncorrect 20 0 100"
g++ sol.cpp; ./a.out input.dat randomIncorrect 20 0 100
testsCounter $?

echo "Test 4"
echo "Running g++ sol.cpp; ./a.out input.dat pX lol -100 100"
g++ sol.cpp; ./a.out input.dat pX lol lol lol
testsCounter $?

echo "Test 5"
echo "Running g++ sol.cpp; ./a.out input.dat mass 20 0 100"
g++ sol.cpp; ./a.out input.dat mass 20 0 100
testsCounter $?

echo "Done!!!"
echo "Total Tests : $total_tests"
echo "Tests Passed : $tests_passed"
echo "Tests Failed : $tests_failed" 
echo "Tests UnCovered : $tests_not_covered"