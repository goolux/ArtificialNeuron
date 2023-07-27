import neuron
from neuron import h, gui
from neuron.units import ms, mV
import matplotlib.pyplot as plt
import numpy as np
import time
import os

# os.system("rm -rf x86_64")
# os.system("nrnivmodl mechanisms")

h.load_file('stdrun.hoc')
neuron.h.stdinit();
h.nrn_load_dll('x86_64/nrnmech.dll')




"""
Import morphology and create cell
"""
h.load_file('import3d.hoc')

from pathlib import Path
script_dir = Path( __file__ ).parent.absolute()

class MyCell:
    def __init__(self):
        morph_reader = h.Import3d_Neurolucida3()
        morph_reader.input(f'{script_dir}/morphologies/1148_H42_01.asc')
        i3d = h.Import3d_GUI(morph_reader, 0)
        i3d.instantiate(self)

cell = MyCell()


"""
create sectionlist and put the sections into it
"""
apical = h.SectionList()
for sec in cell.apic:
    apical.append(sec)
somatic = h.SectionList()
for sec in cell.soma:
    somatic.append(sec)
basal = h.SectionList()
for sec in cell.dend:
    basal.append(sec)
axonal = h.SectionList()
for sec in cell.axon:
    axonal.append(sec)
myelinated = h.SectionList()




"""
Replace axon with a 60 micrometers axon initial segment (stub axon) followed by 1000 micrometers myelinated section
"""
#set nseg of basal and apical dendrites
for section in basal:
    section.nseg = int(section.L / 14.79) + 1
for section in apical:
    section.nseg = int(section.L / 14.79) + 1

L_target = 60  # length of stub axon
nseg0 = 5  # number of segments for each of the two axon sections

nseg_total = nseg0 * 2
chunkSize = L_target / nseg_total

diams = []
lens = []

count = 0
for section in axonal:
    L = section.L
    nseg = 1 + int(L / chunkSize / 2.) * 2  # nseg to get diameter
    section.nseg = nseg

    for seg in section:
        count = count + 1
        diams.append(seg.diam)
        lens.append(L / nseg)
        if count == nseg_total:
            break
    if count == nseg_total:
        break

for section in axonal:
    neuron.h.delete_section(sec=section)

#  new axon array
axon = [h.Section(name='axon[%d]' % i) for i in range(2)]

L_real = 0
count = 0

for index, section in enumerate(axon):
    section.nseg = int(nseg_total / 2)
    section.L = int(L_target / 2)

    for seg in section:
        seg.diam = diams[count]
        L_real = L_real + lens[count]
        count = count + 1

    axonal.append(sec=section)

#childsec.connect(parentsec, parentx, childx)
axon[0].connect(cell.soma[0], 1.0, 0.0)
axon[1].connect(axon[0], 1.0, 0.0)

myelin=neuron.h.Section(name='myelin')
myelinated.append(sec=myelin)
myelin.nseg = 5
myelin.L = 1000
myelin.diam = diams[count-1]
myelin.connect(axon[1], 1.0, 0.0)



"""
Insert channels and define parameters of the different sections
"""
ax_res=100
capa_mb=1
erev_pas=-94.9999999825
conduct_pas=4.648495583975217e-05
erev_na=50
erev_k=-90

for sec in myelinated:
    sec.Ra=ax_res
    sec.cm=0.02
    sec.insert('pas')
    sec.g_pas=conduct_pas
    sec.e_pas=erev_pas

for sec in axonal:
    sec.Ra=ax_res
    sec.cm=capa_mb
    sec.insert('pas')
    sec.g_pas=conduct_pas
    sec.e_pas=erev_pas 
    sec.insert("NaTg")
    sec.vshifth_NaTg=10
    sec.slopem_NaTg=9
    sec.gNaTgbar_NaTg=1.49999999925
    sec.insert("Nap_Et2")
    sec.gNap_Et2bar_Nap_Et2=0.007515151097008083
    sec.insert("K_Pst")
    sec.gK_Pstbar_K_Pst=0.9999999995
    sec.insert("K_Tst")
    sec.gK_Tstbar_K_Tst=9.999999439624929e-11
    sec.insert("SKv3_1")
    sec.gSKv3_1bar_SKv3_1=0.12149415785237955
    sec.insert("Ca_HVA2")
    sec.gCa_HVAbar_Ca_HVA2=0.0003653833289355131
    sec.insert("Ca_LVAst")
    sec.gCa_LVAstbar_Ca_LVAst=0.0033545108232307854
    sec.insert("SK_E2")
    sec.gSK_E2bar_SK_E2=0.09482378864339042
    sec.insert("CaDynamics_DC0")
    sec.decay_CaDynamics_DC0=286.69926223985755
    sec.gamma_CaDynamics_DC0=0.005000000022499996                
    sec.ena=erev_na
    sec.ek=erev_k

for sec in somatic:
    sec.Ra=ax_res
    sec.cm=capa_mb
    sec.insert('pas')
    sec.g_pas=conduct_pas
    sec.e_pas=erev_pas
    sec.insert("NaTg")
    sec.vshiftm_NaTg=13
    sec.vshifth_NaTg=15
    sec.slopem_NaTg=7
    sec.gNaTgbar_NaTg=0.29999999985000003
    sec.insert("K_Pst")
    sec.gK_Pstbar_K_Pst=0.14004846952125172
    sec.insert("K_Tst")
    sec.gK_Tstbar_K_Tst=0.0008361070916362151
    sec.insert("SKv3_1")
    sec.gSKv3_1bar_SKv3_1=0.13774240719478914
    sec.insert("Ca_HVA2")
    sec.gCa_HVAbar_Ca_HVA2=0.0006428123240152061
    sec.insert("Ca_LVAst")
    sec.gCa_LVAstbar_Ca_LVAst=0.008747427972029107
    sec.insert("SK_E2")
    sec.gSK_E2bar_SK_E2=4.9999997198124646e-11
    sec.insert("CaDynamics_DC0")
    sec.decay_CaDynamics_DC0=221.50710508816383
    sec.gamma_CaDynamics_DC0=0.04430639954859035
    sec.insert("Ih")
    for seg in sec:
        distance_from_soma = h.distance(cell.soma[0](0.5), seg)
        seg.gIhbar_Ih=(-0.8696 + 2.087*np.exp(distance_from_soma*0.0031))*5.752827870822203e-05
    sec.ena=erev_na
    sec.ek=erev_k

for sec in apical:
    sec.Ra=ax_res
    sec.cm=capa_mb*2
    sec.insert('pas')
    sec.g_pas=conduct_pas
    sec.e_pas=erev_pas
    sec.insert("NaTg")
    sec.vshiftm_NaTg=6
    sec.vshifth_NaTg=6
    for seg in sec:
        distance_from_soma = h.distance(cell.soma[0](0.5), seg)
        sec.gNaTgbar_NaTg=np.exp(distance_from_soma*-0.013305177946562975)*0.03161970891152374
    sec.insert("SKv3_1")
    sec.gSKv3_1bar_SKv3_1=0.0019555565644044817
    sec.insert("Ca_HVA2")
    sec.gCa_HVAbar_Ca_HVA2=6.716941004725377e-05
    sec.insert("Ca_LVAst")
    sec.gCa_LVAstbar_Ca_LVAst=6.521518048693982e-05
    sec.insert("CaDynamics_DC0")
    sec.gamma_CaDynamics_DC0=0.005000000022499996
    sec.insert("Ih")
    for seg in sec:
        distance_from_soma = h.distance(cell.soma[0](0.5), seg)
        seg.gIhbar_Ih=(-0.8696 + 2.087*np.exp(distance_from_soma*0.0031))*5.752827870822203e-05
    sec.ena=erev_na
    sec.ek=erev_k
    
for sec in basal:
    sec.Ra=ax_res
    sec.cm=capa_mb*2
    sec.insert('pas')
    sec.g_pas=conduct_pas
    sec.e_pas=erev_pas
    sec.insert("Ca_HVA2")
    sec.gCa_HVAbar_Ca_HVA2=2.4459961657117288e-05
    sec.insert("CaDynamics_DC0")
    sec.gamma_CaDynamics_DC0= 0.04531948689794783
    sec.insert("Ca_LVAst")
    sec.gCa_LVAstbar_Ca_LVAst=0.0009999999995
    sec.insert("Ih")
    for seg in sec:
        distance_from_soma = h.distance(cell.soma[0](0.5), seg)
        seg.gIhbar_Ih=(-0.8696 + 2.087*np.exp(distance_from_soma*0.0031))*5.752827870822203e-05

    
print('model created!')