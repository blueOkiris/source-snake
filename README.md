# Source Snake

## Description

A simple snake clone written entirely in C++ using SFML

The engine itself has been separated into its own [library](https://github.com/blueOkiris/link)

![image](demo.gif)

## Build

On a Debian system, run `make install-deps` to install dependencies
If not on a Debian system, install the [dependencies](#dependencies)

Once those are installed, build the project with `make`

If you want to remove all traces of the built project, including highscore, run `make clean`

## Dependencies
 - g++
 - GNU make
 - SFML headers and libraries installed to primary include path
