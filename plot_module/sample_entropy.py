from model_evaluation_multiple import GroundTruthData
import matplotlib.pyplot as plt
import EntropyHub as EH
import os
import numpy as np
import time
import multiprocessing
from typing import List
import argparse
from multiprocessing import Process,Queue
number_of_cpus = multiprocessing.cpu_count()
import queue
MAX_INTERVAL = 200
print("start job")
import pickle as pickle
number_of_jobs=number_of_cpus-1//5
number_of_jobs=1
def create_sample_entropy_file(q,use_process=True):

    # gt_original_name = 'davids_ergodic_train'
    # gt_reduction_name = 'reduction_ergodic_train'
    # gt_reduction = GroundTruthData.load(os.path.join( 'evaluations', 'ground_truth', gt_reduction_name + '.gteval'))
    # gt_original = GroundTruthData.load(os.path.join('evaluations', 'ground_truth', gt_original_name + '.gteval'))
    if not use_process:
        sr, so, index, r_tag,o_tag=data
        # r, o = gt_reduction.get_by_index(index), gt_original.get_by_index(index)
        # vr, sr = r
        # vo, so = o
        sr= sr.astype(np.float64)
        so= so.astype(np.float64)
        t = time.time()
        r_Mobj = EH.MSobject('SampEn', m=2,tau =1)
        o_Mobj = EH.MSobject('SampEn', m=2,tau =1)
        r_MSx, r_Ci = EH.MSEn(sr, r_Mobj, Scales=MAX_INTERVAL)
        o_MSx, o_Ci = EH.MSEn(so, o_Mobj, Scales=MAX_INTERVAL)
        # se_r, _, _ = EH.SampEn(sr, m=MAX_INTERVAL)
        # se_o, _, _ = EH.SampEn(so, m=MAX_INTERVAL)
        print(
            f"current sample number {i}   total: {time.time() - t} seconds",
            flush=True)
        with open(os.path.join("sample_entropy",f"sample_entropy_{r_tag}_{i}_{MAX_INTERVAL}d.p"),'wb') as f:
            pickle.dump((r_MSx,r_Ci),f)
        with open(os.path.join("sample_entropy",f"sample_entropy_{o_tag}_{i}_{MAX_INTERVAL}d.p"),'wb') as f:
            pickle.dump((o_MSx,o_Ci),f)
        return
    while True:
        if q.empty():
            return
        data = q.get(block=120)
        if data is None:
            return
        sr, so, index, r_tag,o_tag=data
        # r, o = gt_reduction.get_by_index(index), gt_original.get_by_index(index)
        # vr, sr = r
        # vo, so = o
        sr= sr.astype(np.float64)
        so= so.astype(np.float64)
        t = time.time()
        r_Mobj = EH.MSobject('SampEn', m=2,tau =1)
        o_Mobj = EH.MSobject('SampEn', m=2,tau =1)
        r_MSx, r_Ci = EH.MSEn(sr, r_Mobj, Scales=MAX_INTERVAL)
        o_MSx, o_Ci = EH.MSEn(so, o_Mobj, Scales=MAX_INTERVAL)
        # se_r, _, _ = EH.SampEn(sr, m=MAX_INTERVAL)
        # se_o, _, _ = EH.SampEn(so, m=MAX_INTERVAL)
        print(
            f"current sample number {i}   total: {time.time() - t} seconds",
            flush=True)
        with open(os.path.join("sample_entropy",f"sample_entropy_{r_tag}_{i}_{MAX_INTERVAL}d.p"),'wb') as f:
            pickle.dump((r_MSx,r_Ci),f)
        with open(os.path.join("sample_entropy",f"sample_entropy_{o_tag}_{i}_{MAX_INTERVAL}d.p"),'wb') as f:
            pickle.dump((o_MSx,o_Ci),f)
        # np.save(os.path.join("sample_entropy",f"sample_entropy_reduction_{i}_{MAX_INTERVAL}d.npy"), np.array(se_r))
        # np.save(os.path.join("sample_entropy",f"sample_entropy_original_{i}_{MAX_INTERVAL}d.npy"), np.array(se_o))

def get_sample_entropy(indexes:[int,List[int]]):
    if isinstance(indexes,int):
        indexes=[indexes]
    number_of_jobs = min(number_of_cpus - 1,len(indexes))

    gt_original_name = 'davids_ergodic_train'
    gt_reduction_name = 'reduction_ergodic_train'
    gt_reduction = GroundTruthData.load(os.path.join( 'evaluations', 'ground_truth', gt_reduction_name + '.gteval'))
    gt_original = GroundTruthData.load(os.path.join('evaluations', 'ground_truth', gt_original_name + '.gteval'))
    if number_of_jobs>1:
        queue=Queue(maxsize=number_of_jobs)
        process = [Process(target=create_sample_entropy_file, args=(queue,)) for i in range(number_of_jobs)]
    print('starting')
    for j,index in enumerate(indexes):
        r,o = gt_reduction.get_by_index(index), gt_original.get_by_index(index)
        vr, sr = r
        vo, so = o
        if number_of_jobs>1:
            queue.put((sr,so,index,gt_reduction_name,gt_original_name))
            if j<len(process):
                process[j].start()
        else:
            create_sample_entropy_file((sr,so,index,gt_reduction_name,gt_original_name),False)

    if number_of_jobs>1:
        for p in process:
            p.join()


if __name__ == "__main__":
    # parser = argparse.ArgumentParser(description='Add configuration file')
    # parser.add_argument('-mem', dest="memory", type=int,
    #                     help='set memory', default=-1)
    # args = parser.parse_args()
    from utils.slurm_job import *
    number_of_clusters=10
    job_factory = SlurmJobFactory("cluster_logs")
    gt_reduction_name = 'reduction_ergodic_validation'
    size = len(GroundTruthData.load(os.path.join( 'evaluations', 'ground_truth', gt_reduction_name + '.gteval')))
    jumps=size//number_of_clusters

    for i in range(number_of_clusters):
        indexes=list(range(i*jumps,min((i+1)*jumps,size)))
        print(indexes)
        job_factory.send_job(f"sample_entropy_{i}_{MAX_INTERVAL}d", f'python -c "from plot_module.sample_entropy import get_sample_entropy; get_sample_entropy({indexes})"',mem=400000)
        print('job sent')