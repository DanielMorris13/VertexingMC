# MC Simulation for Particle Vertex Reconstruction

**Authors:** Giulia Italia (UniTo), Daniel Morris Sulluchuco (UniTo)

This project implements a Monte Carlo simulation of particle collisions along a beam line.

Particles are generated with a configurable multiplicity and propagate through two cylindrical tracking detector layers surrounding the beam pipe. The interaction (vertex) point is reconstructed using the measured impact positions in these detectors.

The simulation allows the study of reconstruction efficiency and acceptance for the given geometry. 

Users can tune:

	-	Particle multiplicity generation: 
 		- Fixed, 
   		- Uniform,
	 	- or sampled from a user-provided distribution
   
	-	Vertex position:
		-	In the transverse plane (XY): Gaussian distribution.
		-	Along the beam axis (Z): Gaussian or Uniform distribution.

Dependencies:

	-	ROOT (>= v6.36)
	-	CMake (>= 3.10)
	-	C++17 or newer

## Quick Start

	git clone https://github.com/DanielMorris13/VertexingMC.git

	cd VertexingMC

	mkdir build && cd build
	
	cmake ../VertexingMC

	make 
 
	./fastMC

### Output

	-	Simulation results are stored in ROOT files (.root) and can be visualized with ROOT.
	-	Efficiency and acceptance plots can be generated for different multiplicity and vertex position distributions.

#### Note

remeber to put your path in the main.cpp file :)
