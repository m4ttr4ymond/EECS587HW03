#!/bin/bash

#============================================================================
#Name        : startup.sh
#Author      : Matt Raymond
#Class       : EECS 587, Spring 2020
#Description : Handles compilation and slurm job submission for HW3
#============================================================================

# Load in modules
module load gcc

# Compiler the program with the given compiler optimization level
g++ -fopenmp hw3_mattrmd.cpp -o hw3_mattrmd

# Remove excess slurm files
if compgen -G "./slurm-*" > /dev/null; then
	rm ./slurm-*
fi

# Make the output directory if it doesn't exist
if ! compgen -G "./output/" > /dev/null; then
	mkdir ./output
fi

EXPORT_FILENAME=/home/mattrmd/programs/EECS587HW03/output/result.out

# Remove the output file
if compgen -G $EXPORT_FILENAME > /dev/null; then
	rm $EXPORT_FILENAME
fi

for p in 1 4 10 36
    do
        FILENAME=./$p.sh
        # Copy the batch file template
        cp ./batch_template.sh $FILENAME

        # Replace the template terms
        sed -i -e "s/<p_num>/$p/" $FILENAME
        sed -i -e "s|<file_name>|$EXPORT_FILENAME|" $FILENAME

        # Send the file to slurm
        sbatch $FILENAME

        # Remove the batch file
        rm $FILENAME
    done

# Print all of the processes that are in the queue
watch squeue -u mattrmd
