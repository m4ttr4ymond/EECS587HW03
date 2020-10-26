#!/bin/bash

#============================================================================
#Name        : startup.sh
#Author      : Matt Raymond
#Class       : EECS 587, Spring 2020
#Description : Handles compilation and slurm job submission for HW2
#============================================================================

# Load in modules
module load gcc
module load openmpi

# Compiler the program with the given compiler optimization level
mpic++ $1 02.cpp -o 02

# Remove excess slurm files
if compgen -G "./slurm-*" > /dev/null; then
	rm ./slurm-*
fi

# Make the output directory if it doesn't exist
if ! compgen -G "./output/" > /dev/null; then
	mkdir ./output
fi

EXPORT_FILENAME=/home/mattrmd/programs/EECS587HW02/output/result.out

# Remove the output file
if compgen -G $EXPORT_FILENAME > /dev/null; then
	rm $EXPORT_FILENAME
fi

# Cycle through matrix sizes
for i in 2000,500 1000,4000; do IFS=","; set -- $i;
    # Cycle through processor numbers
   for p in 1 4 16 36
    do
        FILENAME=./$1_$2_$p.sh
        # Copy the batch file template
        cp ./batch_template.sh $FILENAME

        # Replace the template terms
        sed -i -e "s/<p_num>/$p/" $FILENAME
        sed -i -e "s/<m_num>/$1/" $FILENAME
        sed -i -e "s/<n_num>/$2/" $FILENAME
        sed -i -e "s|<file_name>|$EXPORT_FILENAME|" $FILENAME

        # Send the file to slurm
        sbatch $FILENAME

        # Remove the batch file
        rm $FILENAME
    done
done

# Print all of the processes that are in the queue
watch squeue -u mattrmd
