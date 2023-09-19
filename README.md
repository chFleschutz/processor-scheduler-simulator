# Processor Scheduler Simulator

## Introduction

The Processor Scheduler Simulator is a C++ program that calculates the average waiting time for processor jobs using various scheduling algorithms. It allows you to input a number of processor jobs with specified durations and then choose an algorithm to schedule them. The program then computes the average waiting time for the jobs based on your chosen algorithm.

## Features

- Calculates the average waiting time for processor jobs.
- Support for multiple scheduling algorithms.
- Support for preemptive (P) and non-preemptive (NP)

## Getting Started

1. Clone this repository:
   ```bash
   git clone https://github.com/chFleschutz/processor-scheduler-simulator.git
   ```

2. Open the Visual Studio solution

3. Run the project and calculate the average waiting time

## Algorithms

The simulator currently supports the following scheduling algorithms:

  - First Come First Serve  
  - Shortest Job First 
  - Earliest Deadline First
  - Least Laxity First (on 2 Processors, only NP)
  - Round Robin (with selectable Quantum, only NP)
