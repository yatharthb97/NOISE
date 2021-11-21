# NOISE



<img src="https://github.com/yatharthb97/NOISE/blob/master/Resource/logo_transparent.png?raw=true" alt="logo" style="width:200px;"/>

**Note: This code is not currently compiling.**

`NOISE` is a lattice random walker simulator. The implementation allows **arbitrary** number of random walkers and dimension. The system consists of a concentric pair of hypercubes with the bigger one forming the bounding box and obeys periodic boundary conditions.  This implementation cross-compiles on an Arduino compatible microcontroller as well as a PC system. When compiled on a microcontroller, the code can be used to generate Poisson Noise signals on any available digital pin. The compiled PC system can be used as an emulator or a stand alone application.

The random walkers in the  `HyperCube` emit a `HIGH` signal, whenever a random walker passes through the smaller concentric cube. The inspiration behind this model was to crudely simulate diffusive fluorescent particles passing through a confocal volume. Outside the confocal volume (or the smaller hypercube), the signal emitted by the random walkers is `LOW`.



The file `macros.hpp` contains the macros the are used to enable/disable features and control the build environment.



→ For PC Systems, build using `gcc`:

```bash
g++ -std=c++11 main.cpp -o noise
```

Execute:

```bash
./noise <optional-name>
```





## File Descriptions

`main.cpp` - Executor code for PC System .

`NOISE.ino` - Executor for Arduino compatible microcontrollers.

`hypercube_engine.hpp` - Describes the simulation box.

`minmax.hpp` - Describes the utility class used for controlling boundary conditions.

`rand.hpp` - Random number generation interface.

`pulse.hpp` - Describes the pulsing mechanism.

`macros.hpp` - Describes the macros that control the features and the build.

`utilities.hpp` - Generic utility functions.





---

