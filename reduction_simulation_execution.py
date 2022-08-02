import time
import os
import logging
from project_path import *
import argparse
import json
from slurm_job import *
from math import ceil

parser = argparse.ArgumentParser(description='Add configuration file')

parser.add_argument(dest="number_of_cpus", type=int,
                    help='number of cpus to run on', default=None)

parser.add_argument(dest="directory", type=str,
                    help='data directory', default=None)

args = parser.parse_args()
print(args)

number_of_cpus = args.number_of_cpus
directory = args.directory
onlyfiles = [f for f in os.listdir(directory) if os.path.isfile(os.path.join(directory, f))]

job_factory = SlurmJobFactory("cluster_logs")
files_per_cpu = ceil(len(onlyfiles)/number_of_cpus)


directory_name=os.path.dirname(directory)
print('****',directory_name)
# with open(os.path.join(MODELS_DIR, "%s.json" % configs_file), 'r') as file:
#     configs = json.load(file)

# for i in range(number_of_cpus):
params_string=''
for i,f in enumerate(onlyfiles):
    if i%number_of_cpus==0:
        params_string = "-f '" + str(os.path.join(directory, f)) + "' -d '" + directory_name + "_reduction'"
    else:
        params_string = "-f '" + str(os.path.join(directory, f)) + "' " + params_string
    # if i%files_per_cpu==files_per_cpu-1 or i==len(onlyfiles)-1:
        # job_factory.send_job("%s_%s"%("reduction_simulation",directory_name),
        #                  'python3 $(dirname "$path")/simulate_L5PC_reduction_and_create_dataset.py %s -i $SLURM_JOB_ID' % (params_string),filename_index=i//files_per_cpu)



