## Overview

This repository is related to the _RegBL completion_ research project. The _STDL_ was contacted by the Swiss Federal Statistical Office (_OFS_) to determine in which extend it could be possible to complete the construction date of Swiss buildings based on the analysis of a temporal sequence of the Swiss federal maps produced by _swisstopo_. With an initial target of _80%_ of correct guesses, the goal of this research project was to demonstrate the possibility to reach such goal using a reliable validation metric.

This repository holds the developed scripts used to analyse the results of the detection made through the research project primary processing pipeline.

## Research Project Links

The following links give access to the codes related to the project :

* [Primary pipeline - Construction dates extraction using maps](https://github.com/swiss-territorial-data-lab/regbl-poc)
* [Secondary pipeline - Construction dates extraction without maps](https://github.com/swiss-territorial-data-lab/intYEARpolator)
* [Results and analysis tools for the primary pipeline (This repository)](https://github.com/swiss-territorial-data-lab/regbl-poc-analysis)

The following links give access to official documentations on the considered data :

* [RegBL : Swiss federal register of buildings and dwellings](https://www.bfs.admin.ch/bfs/en/home/registers/federal-register-buildings-dwellings.html)
* [Maps : Swiss national maps 1:25'000](https://shop.swisstopo.admin.ch/en/products/maps/national/lk25)

## regbl-poc

## Copyright and License

**regbl-poc** - Nils Hamel, Huriel Richel <br >
Copyright (c) 2020-2021 Republic and Canton of Geneva

This program is licensed under the terms of the GNU GPLv3. Documentation and illustrations are licensed under the terms of the CC BY 4.0.

## Dependencies

The _regbl-poc_ comes with the following package (Ubuntu 20.04 LTS) dependencies ([Instructions](DEPEND.md)) :

* build-essential
* libopencv-dev

The code documentation is built using Doxygen.

## Compilation

To build the project, including the sub-modules, use make with the following targets :

    $ make clean-all && make all

To rebuild the binaries without rebuilding sub-modules, use the make targets :

    $ make clean && make build

To only rebuild sub-modules, use the make command with the targets :

    $ make clean-module && make module

To generate the documentation of the project, use the targets :

    $ make clean-doc && make doc

and the targets :

    $ make clean-all-doc && make all-doc

to generate the documentation of the project and its sub-modules.
