# CEPC-PID, a Simple Tool for CEPC Crystal ECAL Module Simulation Analysis

## Author
Ji-Yuan CHEN (SJTU; <jy_chen@sjtu.edu.cn>)

## Description
This program is designed for PID of CEPC crystal ECAL module. By reconstructing variables describing the topology of the hadronic and electromagnetic showers, we can perform PID with the help of BDT, using the TMVA (the **T**oolkit for **M**ulti-**V**ariate data **A**nalysis with ROOT) package.

For a more intuitive understanding of the shower topology, a python program is also included for event display.

## Usage
At first, you can run
```shell
iPID --help
```
to display help information of analysis, and
```shell
python display.py -h
```
for that of event display (mind the order of `display.py` and `-h`!). For more detail, please refer to the following instructions. :stuck_out_tongue:

### Sample Preparation
This project is only compatible with MC samples generated with `SimCalModule`, which has not been publicised yet… If you have generated samples with different crystal size and/or crystal number, you have to modify `include/PID.h`, such that the program can function smoothly.

### Collecting Original Hit Information
The data in the generated ROOT files does not contain original hit information explicitly; therefore, such information should be converted from the existing branch `vecEcalCellID`. To achieve this, run
```shell
iPID -h -f [file]
```
if the tree in the original ROOT file has default name `treeEvt`. Otherwise, to specify other tree names, run
```shell
iPID -h -f [file] -t [tree]
```
After this, an output file whose name has a prefix ‘hit’ will be created in your current directory, regardless of where the original ROOT file is. The (possibly) unnecessary (or irrelevant) branches have already been deleted to save space.

### Adding Reconstructed Variables
In any directory, execute
```shell
iPID -r -f [file]
# or
iPID -r -f [file] -t [tree]
```
The name of the output file will be given a prefix ‘rec’ in your current directory, and the original branches will not be kept for the sake of saving space. If you need to add some new variables or modify the definitions of some of them, please go to the file `src/Variables.cxx`.

### Performing BDT
Before you begin, make sure that the variables as well as the ROOT files listed in `pid.cxx` re all present (you can also modify this file to meet your own needs). Then, execute
```shell
iPID -b
# or
iPID -b -t [tree]
```
Eventually, you can see the output file containing the data obtained during training and test (`TMVAMulticlass.root`) in your current directory.

Possibly you need to know the performance of BDT on the validation dataset, or use the training results for application. In either case, execute
```shell
iPID -c -f [file]
# or
iPID -c -f [file] -t [tree]
```
Then, the BDT response will be stored in the output ROOT file, whose name has a prefix ‘bdt’, in your current directory. While modifying `src/BDT.cxx`, make sure that the input variables are identical to those in `bdt.cxx`, including the order!

### Event Display
In the directory you have installed, run
```shell
./event_display.sh
```
to obtain a figure of event display, which will be saved in a directory assigned in this shell script (default: `figs/`).

You can modify `event_display.sh` to meet your own needs: input ROOT file, tree name, title of the figure, ID of the event, directory to save the output file, name of the output file, and instantly show the figure or not.

## Environment Set-up
This project requires CMake version >= 3.17. If you are working on the cluster of INPAC, IHEP, etc., the environment can be easily set up by simply executing
```shell
source /cvmfs/sft.cern.ch/lcg/views/LCG_103/x86_64-centos7-gcc12-opt/setup.sh
```
which has already been included in `setup.sh`.

Then, the environment with CMake 3.23.2 and ROOT 6.28/00 will be configured. As long as neither compilation errors are raised, nor the CMake version requirement is met, other versions on the LCG can also be used. :stuck_out_tongue:

### Notice
On WSL2, the above environment cannot be used for event display, since error occurs with X11 because of LCG! Instead, the environment used while designing this program was with
- Python: 3.11.7
- Matplotlib: 3.8.0
- NumPy: 1.26.4
- UpROOT: 5.3.1

Some other versions can also be used, but that listed above have been tested.

## Installation and Compilation
Having set up the environment, this project can be cloned from GitHub and compiled as usual:
```shell
git clone git@github.com:phys-jychen/cepc-pid.git
cd cepc-pid
mkdir build
cd build
cmake ..
make -j100    # Just do it!
source setup.sh
```

Every time you log in to the cluster, and right before the first time of running iPID, remember to execute
```shell
source <build_dir>/setup.sh
```

By now, the compilation have been finished. Prepare your datasets, and have fun! :relaxed:

## To-Do
Definitions of some variables need further check.

## Reference
The framework of this project comes from [dss-pid](https://github.com/phys-jychen/dss-pid). Both are high-granularity homogeneous ECAL, with differences in energy range and material and arrangement of the crystal. The definitions of some variables have been modified; and the execution has been greatly simplified.