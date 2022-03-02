# Neuron-astrocyte network model

Here we propose a computational model of neuron-astrocyte network under the influence of multiplicative noise.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

- Visual Studio ≥2019
- The minimum required amount of RAM is 16 GB

### Installing

Clone repo:
```
git clone https://github.com/AnastasiaErmolaeva/Neuron-astrocyte-network-Noise-induced-artificial-intelligence.git
```

### Running the experiment

To run the default experiment execute `main.cpp`.

Default experiment consists of 1 image at the learning stage (1 plus 10 percent of salt and pepper.txt) and 1 image at the testing stage (1 plus 40 percent of salt and pepper.txt).

The simulation model time is 1.5 seconds. The time step is 0.000001 seconds.
The program run time for default parameters is around 2.5 hours.

### Results

The following results are presented at the end of the simulation: membrane potential of neurons, intracellular calcium concentration in astrocytes.
 
### Parameters

File in.txt consists of model parameters, including:
- Astrocyte network size
- Neuron network size
- Multiplicative noise intensity
- Coupling strengths
- Timeline

## Authors

* **Alex Zhao** - *Implementation*
* **Anastasia Ermolaeva** - *Implementation* - [AnastasiaErmolaeva](https://github.com/AnastasiaErmolaeva)
* **Ekkehard Ullner** - *Biological model constructing* 
* **Juergen Kurths** - *Biological model constructing*
* **Susan Gordleeva** - *Biological model constructing*
* **Alexey Zaikin** - *Project vision*

## Cite

Noise-induced artificial intelligence
Alex Zhao, Anastasia Ermolaeva, Ekkehard Ullner, Juergen Kurths, Susanna Gordleeva, Alexey Zaikin
