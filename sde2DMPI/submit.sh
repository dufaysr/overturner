#!/bin/bash
# Submission script for VEGA
#SBATCH --job-name=overturner
#SBATCH --time=01:00:00 # hh:mm:ss
#
#SBATCH --ntasks=150
#SBATCH --mem-per-cpu=2500 # megabytes
#SBATCH --partition=defq
#
#SBATCH --mail-user=renaud.dufays@student.uclouvain.be
#SBATCH --mail-type=ALL
#
#SBATCH --comment=tfe 

rm -f /bin/overturner.exe
make
sleep 20
# mpirun ./bin/overturner.exe
srun ./bin/overturner.exe
