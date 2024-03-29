# Event-Driven Logic Circuit Simulator
## Overview

This project aims to develop an event-driven logic circuit simulator, providing users with the capability to create virtual representations of digital circuits and observe their behavior under different conditions without the need for physical construction. The simulator utilizes an event-driven approach to model the behavior of digital circuits based on changes in logical states, allowing for accurate representation of dynamic system behavior.

## Requirements

- **C++ Compiler**: Tested with G++.
- **Standard C++ Libraries**: Required libraries include `iostream`, `fstream`, `string`, `vector`, etc.

## Guidelines

Run the executable with the following command-line arguments:

```bash
./simulationSC_copy <library_file> <circuit_file> <stimuli_file> <output_path> <common_output_path>
```

- `<library_file>`: Path to the library file.
- `<circuit_file>`: Path to the circuit description file.
- `<stimuli_file>`: Path to the stimuli file.
- `<output_path>`: Path to the output file for simulation results.
- `<common_output_path>`: Path to the common output file for intermediate results.

Execute `plot.py` to visualize the simulation results.

## Functionality

### Parsing
The code parses input files to extract information about logical gates, circuit structure, and input stimuli.

### Simulation
It simulates the behavior of the circuit over time, considering gate delays and input stimuli.

### Output
Simulation results are written to output files, including intermediate and final results. The output results are expressed through a `.txt` file and visualization using Python.

## Usage

1. **Compilation**: Compile the code using the C++ compiler.

```bash
g++ simulationSC_copy.cpp -o simulationSC_copy
```

2. **Run the Executable**: Execute the compiled executable with the required command-line arguments.

```bash
./simulationSC_copy library_file circuit_file stimuli_file output_path common_output_path
```

3. **Visualization**: Execute `plot.py` to visualize the simulation results.

```bash
python plot.py
```

## File Structure

- `simulationSC_copy.cpp`: Main C++ code for the logic circuit simulator.
- `plot.py`: Python script for visualizing simulation results.
- `library_file`: File containing information about logical gates.
- `circuit_file`: File describing the structure of the circuit.
- `stimuli_file`: File containing input stimuli.
- `output_path`: Path to the output file for simulation results.
- `common_output_path`: Path to the common output file for intermediate results.

## Contributors
- [Ramy Shehata](https://github.com/GM-Sniper)
- [Omar Ganna](https://github.com/omar-ganna)
- [Mohamed Khaled](https://github.com/mmohamedkhaled)
