#!/bin/bash

#============================================================================
#Name        : batch_template.sh
#Author      : Matt Raymond
#Class       : EECS 587, Spring 2020
#Description : Template for HW2
#============================================================================

#SBATCH --job-name=02_p<p_num>_m<m_num>_n<n_num>
#SBATCH --mail-type=NONE
#SBATCH --nodes=<p_num>
#SBATCH --ntasks-per-node=1
#SBATCH --mem-per-cpu=1g 
#SBATCH --time=00:05:00
#SBATCH --account=eecs587f20_class
#SBATCH --partition=standard

./hw3_mattrmd
