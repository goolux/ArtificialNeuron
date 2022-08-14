import time
import os
import logging
# from ...project_path import *
from ... import project_path
from project_path import *
import argparse
import json
from slurm_job import *
from math import ceil
from art import tprint

parser = argparse.ArgumentParser(description='Add configuration file')

parser.add_argument('-c',dest="number_of_cpus", type=int,
                    help='number of cpus to run on', default=None)

parser.add_argument('-d',dest="directory", type=str,
                    help='data directory', default=None)

parser.add_argument('-e',dest="files_that_do_not_exist", type=bool,
                    help='simulate only files that do not exist', default=False)

args = parser.parse_args()
print(args)

number_of_cpus = args.number_of_cpus
directory = args.directory
only_files = [f for f in os.listdir(directory) if os.path.isfile(os.path.join(directory, f))]
# number_of_cpus = len(onlyfiles)



job_factory = SlurmJobFactory("cluster_logs")
files_per_cpu = ceil(len(only_files) / number_of_cpus)


directory_name=os.path.dirname(directory)
directory_name=os.path.basename(directory_name)


if args.files_that_do_not_exist:
    directory_dest , _ = get_dir_name_and_filename("",directory_name)
    files_that_exists = set([f for f in os.listdir(directory_dest) if os.path.isfile(os.path.join(directory_dest, f))])
    new_files=[]
    for f in only_files:
        if f not in files_that_exists:
            new_files.append(f)
    only_files = new_files

params_string=''

print(len(only_files), flush=True)

for i,f in enumerate(only_files):
    print(directory_name)
    if i%files_per_cpu==0:
        params_string = 'python3 $(dirname "$path")/simulate_L5PC_reduction_and_create_dataset.py %s -i $SLURM_JOB_ID'%("-f '" + str(os.path.join(directory, f)) + "' -d '" + directory_name + "_reduction'")
    else:
        params_string = params_string+'&& python3 $(dirname "$path")/simulate_L5PC_reduction_and_create_dataset.py %s -i -1'%("-f '" + str(os.path.join(directory, f)) + "' -d '" + directory_name + "_reduction'")

    if i%files_per_cpu==files_per_cpu-1 or i==len(only_files)-1:
        pass
        # job_factory.send_job("%s_%s"%("reduction_simulation",directory_name), params_string,filename_index=i//files_per_cpu)


