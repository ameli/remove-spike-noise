# Remove Spike Noise

Remove spike noise in VTK data files.

## Prerequisites

1. Install *GNU GCC* compiler, *OpenMP*, and *Cmake*:

       sudo apt-get install g++ libomp-dev cmake -y

2. Install *VTK* development:

       sudo apt install libvtk5-dev -y

   Alternatively, [download VTK](https://vtk.org/download/) and compile it.

## Install

Get the source code

    git clone git@github.com:ameli/remove-spike-noise.git 
    cd remove-spike-noise
	
Compile in ``./build`` directory

    mkdir build
    cmake -B build .
    make -C build
	
After the compilation, the executable file is located in ``./bin/RemoveSpikeNoise``.

## Usage

    ./bin/RemoveSpikeNoise InputFile.vtk

## License

BSD 3-clause.
