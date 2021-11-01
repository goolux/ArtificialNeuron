from os import listdir
import logging
from os.path import isfile, join,isdir
import matplotlib.pyplot as plt
import simulation_data_generator as sdg
from neuron_network import neuronal_model
import numpy as np
import torch
from typing import List
import re
import argparse
from tqdm import tqdm


def plot_network_and_actual_results(file_path: [str, List[str]], model_path: [str, List[str]] = '',
                                    sample_idx: [None, int] = None, time_idx: [None, int] = None,
                                    window_size: int = 2000, include_DVT=True, DVT_PCA_model=None):
    if include_DVT:
        X, y_spike, y_soma, y_DVT = sdg.parse_sim_experiment_file_with_DVT(file_path, DVT_PCA_model=DVT_PCA_model)
        y_DVT = np.transpose(y_DVT, axes=[2, 1, 0])
    else:
        X, y_spike, y_soma = sdg.parse_sim_experiment_file(file_path)
    # reshape to what is needed
    X = np.transpose(X, axes=[2, 1, 0])
    y_spike = y_spike.T[:, :, np.newaxis]
    y_soma = y_soma.T[:, :, np.newaxis]
    if sample_idx is None:
        sample_idx = np.random.choice(range(X.shape[0]), size=1,
                                      replace=True)[0]  # number of simulations per file
    if time_idx is None:
        time_idx = np.random.choice(range(0, X.shape[1] - window_size),
                                    size=1, replace=False)[0]  # simulation duration

    X_batch = torch.from_numpy(X[sample_idx, time_idx:time_idx + window_size, ...][np.newaxis, np.newaxis, ...])
    y_spike_batch = y_spike[sample_idx, time_idx:time_idx + window_size, ...][:, ...]
    y_soma_batch = y_soma[sample_idx, time_idx:time_idx + window_size, ...][:, ...]
    if include_DVT:
        y_DVT_batch = y_DVT[sample_idx, time_idx:time_idx + window_size, ...][:, ...]
    fig, axs = plt.subplots(2)
    window_size=0
    if isinstance(model_path, str):
        model_path = [model_path]
    first_path_name=""
    for p in model_path:
        network = neuronal_model.NeuronConvNet.load(p)
        # network.cpu()
        regex_match = re.search('(?<=TCN__)[0-9]{4}-[0-9]{2}-[0-9]{2}__[0-9]{2}_[0-9]{2}__ID_[0-9]+(?=\.pkl)', p)
        try:
            model_id = regex_match.group(0)
            first_path_name=model_id
            logging.error(first_path_name)
        except Exception as e:
            logging.error(p)
            raise e
        out_var = []
        spike = []
        for i in tqdm(range(network.time_domain_shape,y_soma_batch.shape[0])):
            out = network(X_batch[...,i-network.time_domain_shape:i,:])
            out_var.append(out[1].detach().numpy()[0, 0, :, :])
            spike.append(out[0].detach().numpy()[0, 0, :, :])
        spike = np.array(spike).squeeze()
        out_var = np.array(out_var).squeeze()
        # plt.scatter(np.arange(out_var.shape[0]),spike)
        # plt.plot((out_var-np.min(out_var))/(np.max(out_var)-np.min(out_var))*(np.max(y_soma_batch)-np.min(y_soma_batch))+np.min(y_soma_batch), label=model_id)
        axs[0].plot(np.arange(network.time_domain_shape,y_soma_batch.shape[0]),out_var, label=model_id)
        axs[1].plot(np.arange(network.time_domain_shape,y_soma_batch.shape[0]),spike, label=model_id)
        window_size=network.time_domain_shape
    axs[1].set_xlim([0,y_soma_batch.shape[0]])
    axs[0].plot(y_soma_batch, label='original')
    plt.savefig(join("evaluation_plots","%s_%d_%d_%d.png")%(first_path_name,sample_idx,time_idx,window_size))
    plt.legend()
    plt.show()

plot_network_and_actual_results( r"/ems/elsc-labs/segev-i/david.beniaguev/Reseach/Single_Neuron_InOut/ExperimentalData/L5PC_NMDA_valid_mixed/exBas_0_1100_inhBasDiff_-1100_600__exApic_0_1100_inhApicDiff_-1100_600_SpTemp__saved_InputSpikes_DVTs__1062_outSpikes__128_simulationRuns__6_secDuration__randomSeed_402117.p" \
,r"models/NMDA/evaluation_file_filter_NMDA_Tree_TCN__2021-11-01__18_31__ID_23775/evaluation_file_filter_NMDA_Tree_TCN__2021-11-01__18_31__ID_23775.pkl"\
 ,0 ,1300, 400)