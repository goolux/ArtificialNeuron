# import pickle as pickle #python 3.7 compatibility
import os
import pickle  # python 3.8+ compatibility
from enum import Enum

# from torchviz import make_dot
import torch
import torch.nn as nn

import train_nets.neuron_network.basic_convolution_blocks as basic_convolution_blocks
import train_nets.neuron_network.temporal_convolution_blocks as temporal_convolution_blocks
from project_path import MODELS_DIR
from train_nets.synapse_tree import SectionNode, SectionType
from utils.general_aid_function import *


class ArchitectureType(Enum):
    BASIC_CONV = "BASIC_CONV"
    LAYERED_TEMPORAL_CONV = "LAYERED_TEMPORAL_CONV"


SYNAPSE_DIMENTION_POSITION = 1


# ======================
#     TCN Components
# ======================

class NeuronConvNet(nn.Module):
    def __init__(self, segment_tree, time_domain_shape, architecture_type,
                 is_cuda=False, include_dendritic_voltage_tracing=False, segemnt_ids=None, **network_kwargs):
        super(NeuronConvNet, self).__init__()
        self.architecture_type = architecture_type
        self.include_dendritic_voltage_tracing = include_dendritic_voltage_tracing
        self.segment_tree = segment_tree
        self.segemnt_ids = segemnt_ids if segemnt_ids is not None else dict()
        self.time_domain_shape = time_domain_shape
        self.modules_dict = nn.ModuleDict()
        self.is_cuda = is_cuda
        self.network_kwargs = network_kwargs
        self.build_model(**self.network_kwargs)

    def build_model(self, **network_kwargs):
        # assert kernel_size_1d % 2 == 1 and kernel_size_2d % 2 == 1, "cannot assert even kernel size"
        if self.architecture_type == ArchitectureType.BASIC_CONV.value: #probability wont work becuse synapse became channels
            branch_class = basic_convolution_blocks.BranchBlock
            branch_leaf_class = basic_convolution_blocks.BranchLeafBlock
            intersection_class = basic_convolution_blocks.IntersectionBlock
            root_class = basic_convolution_blocks.RootBlock
        elif self.architecture_type == ArchitectureType.LAYERED_TEMPORAL_CONV.value:
            branch_class = temporal_convolution_blocks.BranchBlock
            branch_leaf_class = temporal_convolution_blocks.BranchLeafBlock
            intersection_class = temporal_convolution_blocks.IntersectionBlock
            root_class = temporal_convolution_blocks.RootBlock
        else:
            assert False, "type is not known"

        # model = NeuronConvNet(segment_tree, time_domain_shape, is_cuda, include_dendritic_voltage_tracing)
        activation_function_base_function = getattr(nn, self.network_kwargs["activation_function_name"])
        activation_function = lambda: (activation_function_base_function(
            **self.network_kwargs["activation_function_kargs"]))  # unknown bug
        sub_network_kargs = dict()
        sub_network_kargs["activation_function"] = activation_function
        for k in network_kwargs.keys():
            if "activation_function" in k:
                continue
            sub_network_kargs[k] = network_kwargs[k]
        input_shape = (0, 0)  # default for outer scope usage
        for segment in self.segment_tree:
            self.segemnt_ids[segment] = segment.id
            param_number = segment.get_number_of_parameters_for_nn()
            input_shape = (param_number,self.time_domain_shape)
            if segment.type == SectionType.BRANCH:
                self.modules_dict[self.segemnt_ids[segment]] = branch_class(input_shape,
                                                                            **sub_network_kargs)
            elif segment.type == SectionType.BRANCH_INTERSECTION:
                self.modules_dict[self.segemnt_ids[segment]] = intersection_class(input_shape,
                                                                                  **sub_network_kargs)
            elif segment.type == SectionType.BRANCH_LEAF:
                channel_output_number = min(sub_network_kargs["channel_output_number"], len(input_indexes))
                self.modules_dict[self.segemnt_ids[segment]] = branch_leaf_class(input_shape,channel_output_number=channel_output_number
                                                                                 **sub_network_kargs)

            elif segment.type == SectionType.SOMA:
                self.last_layer = root_class(input_shape,
                                             **sub_network_kargs)
            else:
                assert False, "Type not found"
        self.double()
        return self



    def forward(self, x):
        x = x.type(torch.cuda.DoubleTensor) if self.is_cuda else x.type(torch.DoubleTensor)
        if self.include_dendritic_voltage_tracing:  # todo add functionality
            pass
        representative_dict = {}
        out = None
        for segments_level_list in self.segment_tree.iterate_by_levels():
            for node in segments_level_list:
                if node.type == SectionType.BRANCH_LEAF:
                    input = x[:, list(node.synapse_nodes_dict.keys()),...]  # todo make it in order
                    representative_dict[node.representative] = self.modules_dict[self.segemnt_ids[node]](input)


                elif node.type == SectionType.BRANCH_INTERSECTION:
                    indexs = [child.representative for child in node.prev_nodes]
                    input = [representative_dict[i] for i in indexs]
                    input = torch.cat(input, dim=SYNAPSE_DIMENTION_POSITION)
                    representative_dict[node.representative] = self.modules_dict[self.segemnt_ids[node]](input)


                elif node.type == SectionType.BRANCH:
                    input = x[:, list(node.synapse_nodes_dict.keys()),...]
                    representative_dict[node.representative] = self.modules_dict[self.segemnt_ids[node]](
                        representative_dict[node.prev_nodes[0].representative], input)

                elif node.type == SectionType.SOMA:
                    indexs = [child.representative for child in node.prev_nodes]
                    input = [representative_dict[i] for i in indexs]
                    input = torch.cat(input, dim=SYNAPSE_DIMENTION_POSITION)
                    out = self.last_layer(input)
                    break
                else:
                    assert False, "Type not found"
            if self.is_cuda:
                pass
                # torch.cuda.synchronize()
        return out

    def count_parameters(self):
        return sum(p.numel() for p in self.parameters() if p.requires_grad)

    def cuda(self, **kwargs):
        super(NeuronConvNet, self).cuda(**kwargs)
        torch.cuda.synchronize()
        self.is_cuda = True

    def cpu(self, **kwargs):
        super(NeuronConvNet, self).cpu(**kwargs)
        self.is_cuda = False

    def save(self, path):  # todo fix
        data_dict = dict(include_dendritic_voltage_tracing=self.include_dendritic_voltage_tracing,
                         segment_tree=self.segment_tree, architecture_type=self.architecture_type,
                         segemnt_ids=self.segemnt_ids,
                         time_domain_shape=self.time_domain_shape,
                         is_cuda=False)
        data_dict.update(self.network_kwargs)
        state_dict = self.state_dict()
        with open('%s.pkl' % path, 'wb') as outp:
            pickle.dump((data_dict, state_dict), outp)

    @staticmethod
    def load(path):
        neuronal_model = None
        with open('%s.pkl' % path if path[-len(".pkl"):] != ".pkl" else path, 'rb') as outp:
            neuronal_model_data = pickle.load(outp)
        neuronal_model = NeuronConvNet(**neuronal_model_data[0])
        neuronal_model.load_state_dict(neuronal_model_data[1])  # fixme this this should
        return neuronal_model

    @staticmethod
    def build_model_from_config(config: AttrDict):
        if config.model_path is None:
            if "config_version" in config:
                # architecture_dict = copy.deepcopy(config)
                # architecture_dict[segment_tree]=load_tree_from_path(architecture_dict.segment_tree_path)
                # del architecture_dict[segment_tree_path]
                network = NeuronConvNet(**(config),segment_tree = load_tree_from_path(config.segment_tree_path))

            else:
                architecture_dict = dict(
                    architecture_type=config.architecture_type,
                    activation_function_name=config.activation_function_name,
                    activation_function_kargs=config.activation_function_kargs,
                    segment_tree=load_tree_from_path(config.segment_tree_path),
                    include_dendritic_voltage_tracing=config.include_dendritic_voltage_tracing,
                    time_domain_shape=config.input_window_size, stride=config.stride, dilation=config.dilation,
                    channel_input_number=config.channel_input_number,
                    inner_scope_channel_number=config.inner_scope_channel_number,
                    channel_output_number=config.channel_output_number
                )
                if config.architecture_type== ArchitectureType.BASIC_CONV.value: # todo make it more generic !!!!!!!!!!!!!!!
                    architecture_dict.update(dict(kernel_size_2d=config.kernel_size_2d,kernel_size_1d=config.kernel_size_1d))
                elif  config.architecture_type == ArchitectureType.LAYERED_TEMPORAL_CONV.value:
                    architecture_dict.update(
                        dict(kernel_size=config.kernel_size, number_of_layers=config.number_of_layers,skip_conections=True))
                network = NeuronConvNet(**(architecture_dict))

        else:
            network = NeuronConvNet.load(os.path.join(MODELS_DIR, *config.model_path))
        network.cuda()
        return network

    # def plot_model(self, config, dummy_file=None): fixme
    #     if dummy_file is None:
    #         dummy_file = glob.glob(TRAIN_DATA_DIR + '*_128_simulationRuns*_6_secDuration_*')
    #     train_data_generator = SimulationDataGenerator(dummy_file, buffer_size_in_files=1,
    #                                                    batch_size=1, epoch_size=1,
    #                                                    window_size_ms=config.input_window_size,
    #                                                    file_load=config.train_file_load,
    #                                                    DVT_PCA_model=None)
    #     batch = next(iter(train_data_generator))
    #     yhat = self(batch.text)
    #     make_dot(yhat,param=dict(list(self.named_parameters())).render("model",format='png') )

    def init_weights(self, sd=0.05):
        def init_params(m):
            if hasattr(m, "weight"):
                m.weight.data.normal_(0, sd)
            if hasattr(m, "bias"):
                m.bias.data.normal_(0, sd)

        self.apply(init_params)


def load_tree_from_path(path: str) -> SectionNode:
    with open(path, 'rb') as file:
        tree = pickle.load(file)
    return tree