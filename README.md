## Overview

_Currently under development_

The _regbl-poc_ is a proof-of-concept for the detection of the construction dates
 of the swiss buildings contained in the [_OFS_](https://www.bfs.admin.ch/bfs/fr/home.html) official [RegBL](https://www.housing-stat.ch/fr/accueil.html) database.

## regbl-poc

## Copyright and License

**regbl-poc** - Nils Hamel, Huriel Reichel <br >
Copyright (c) 2020 Republic and Canton of Geneva

This program is licensed under the terms of the GNU GPLv3. Documentation and illustrations are licensed under the terms of the CC BY-NC-SA.

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
