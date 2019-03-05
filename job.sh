#!/bin/bash

#SBATCH --comment "Projet_GPU_OMP"
#SBATCH -J "Sim_Chaleur"
#SBATCH --error=logs/job.%J.err
#SBATCH --output=logs/job.%J.out
#SBATCH -p short
#SBATCH --time=02:00:00
#SBATCH --mem 50000
#SBATCH -c 28
#SBATCH -N 1
##SBATCH --mail-user=<shurizaidu02300@gmail.com>
##SBATCH --mail-type=ALL,TIME_LIMIT_80

./main