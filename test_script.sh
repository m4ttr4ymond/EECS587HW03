#!/bin/bash

#============================================================================
#Name        : batch_template.sh
#Author      : Matt Raymond
#Class       : EECS 587, Spring 2020
#Description : Test for HW3
#============================================================================

#SBATCH --job-name=03_matt_testing
#SBATCH --mail-type=NONE
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --mem-per-cpu=1g 
#SBATCH --time=00:05:00
#SBATCH --account=eecs587f20_class
#SBATCH --partition=standard

./hw3_mattrmd
