#! /bin/bash

# This file can be executed by calling "bash run_tests.sh"
# It will then test tube1 against the reference_implementation for
# each tube file in the test-suite
<<<<<<< HEAD
project=tube3
=======
project=tube4
>>>>>>> instructor/master
make clean
if [ -f $project ]; then
    rm $project;
fi;

make
chmod a+x Test_Suite/reference_$project
if [ ! -f $project ]; then 
	echo $project "not correctly compiled";
	exit 1;
fi;
if [ ! -f example.tube ]; then 
	echo "example.tube doesn't exist";
	exit 1;
fi;

function run_error_test {
<<<<<<< HEAD
    ./tube3 $1 $project.tic > $project.cout;
    Test_Suite/reference_$project $1 ref.tic > ref.cout;
    diff -w ref.cout $project.cout;
    result=$?;
    rm $project.cout ref.cout;
    if [ $result -ne 0 ]; then
	echo $1 "failed different error messages";
	rm $project.tic ref.tic;
	exit 1;
    fi;
=======
    $project $1 $project.tic > $project.cout;
    Test_Suite/reference_$project $1 ref.tic > ref.cout;
    grep -qi "ERROR" $project.cout 
    student=$?
    grep -qi "ERROR" ref.cout
    ref=$?
    rm $project.cout ref.cout;
    if [ $student -ne $ref ]; then
	echo $1 " failed to raise ERROR or raised ERROR wrongly";
	rm $project.tic ref.tic 2> /dev/null;
	exit 1;
    fi;
    if [ $ref -eq 0 ]; then
	echo $1 " passed";
	rm $project.tic ref.tic 2> /dev/null;
	return
    fi;

>>>>>>> instructor/master

    ../TubeCode/TubeIC $project.tic > $project.out;
    ../TubeCode/TubeIC ref.tic > ref.out;
    diff -w ref.out $project.out;
    result=$?;
<<<<<<< HEAD
    rm $project.out ref.out;
    rm $project.tic ref.tic;
=======
    rm $project.out ref.out 2> /dev/null
    rm $project.tic ref.tic 2> /dev/null
>>>>>>> instructor/master
    if [ $result -ne 0 ]; then
	echo $1 "failed different executed result on TubeIC";
	exit 1;
    else
	echo $1 "passed";
    fi;


}


<<<<<<< HEAD
for F in Test_Suite/test.*.tube; do 
=======
for F in Test_Suite/good*.tube; do 
	run_error_test $F
done

for F in Test_Suite/fail*.tube; do 
>>>>>>> instructor/master
	run_error_test $F
done


echo Extra Credit Results:

for F in Test_Suite/extra.*.tube; do 
	run_error_test $F
done




