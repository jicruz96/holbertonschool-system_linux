#!/bin/bash

me=hnm
you=nm
you_options="-p"
# CHANGE THIS TO YOUR EDITOR OF CHOICE
edit_cmd=code

# Ensure it compiles
make $me

if [ $? -ne 0 ]
then
    exit $?
fi

# If an argument was passed, perform check on this argument only
if [ $# -eq 1 ]
then
    files=($1)
else
    files=`(ls tests)`
fi

# Otherwise, perform check on all files in tests folder
for file in $files
do
    # These are the temporary file names. Feel free to change to whatever. 
    if [[ $file != "tests/"* ]]
    then
        me_file=tmp_$me\_$file
        me_err=$me_file\_err
        you_file=tmp_$you\_$file
        you_err=$you_file\_err
        file=tests/$file
    else
        me_file=tmp_$me
        me_err=$me_file\_err
        you_file=tmp_$you
        you_err=$you_file\_err
    fi

    # Execute my version of command
    ./$me $file > $me_file 2> $me_err

    # Execute actual command
    $you $you_options $file > $you_file 2> $you_err

    # Check error outputs
    if [ 0 -ne `diff $me_err $you_err | wc -l` ]
    then
        echo "$me and $you are different for $file. Inspect errors."
        sleep 1
        $edit_cmd -d $me_err $you_err
        exit 1
    fi

    # Compare outputs. If outputs differ open files to compare and exit
    if [ 0 -ne `diff $me_file $you_file | wc -l` ]
    then
        echo "$me and $you are different for $file. Inspect."
        sleep 1
        # Open editor in -d (diff) mode. If you use emacs, take the -d out
        $edit_cmd -d $me_file $you_file
        exit 1
    fi

    # If everything went well, just get rid of the files
    rm $me_file $you_file $me_err $you_err

    echo "A-OK with $file"
    sleep 2
done
