#! /bin/bash
source /afs/ihep.ac.cn/users/c/chenjiyuan/conda.env

job=1
energy=60
particle="e-"

if [ $particle = "e-" ]
then
    job=1
elif [ $particle = "pi-" ]
then
    job=2
fi

filename="/cefs/higgs/chenjiyuan/cepc-pid/run/cubic/root/thick1_layer30/job${job}_${particle}_${energy}GeV/hit_calo_${particle}_${energy}GeV_Run0.root"
#tree=treeEvt
event_index=2024
save_dir="/cefs/higgs/chenjiyuan/cepc-pid/figs/"
output="Display_${particle}_${energy}GeV.pdf"
#show=0

title="${energy}-GeV "
if [ $particle = "e-" ]
then
    title+='$e^-$'
elif [ $particle = "mu-" ]
then
    title+='$\mu^-$'
elif [ $particle = "pi-" ]
then
    title+='$\pi^-$'
elif [ $particle = "e+" ]
then
    title+='$e^+$'
elif [ $particle = "mu+" ]
then
    title+='$\mu^+$'
elif [ $particle = "pi+" ]
then
    title+='$\pi^+$'
elif [ $particle = "gamma" ]
then
    title+='$\gamma$'
elif [ $particle = "neutron" ]
then
    title+='$n$'
elif [ $particle = "proton" ]
then
    title+='$p$'
fi

#python display.py -f=$filename -i="$title" -e=$event_index -d=$save_dir -o=$output -s=$show
python display.py -f=$filename -i="$title" -e=$event_index -d=$save_dir -o="$output"
