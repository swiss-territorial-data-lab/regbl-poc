## Overview

This repository is related to the _RegBL completion_ research project. The _STDL_ was contacted by the Swiss Federal Statistical Office (_OFS_) to determine in which extend it could be possible to complete the construction date of Swiss buildings based on the analysis of a temporal sequence of the Swiss federal maps produced by _swisstopo_. With an initial target of _80%_ of correct guesses, the goal of this research project was to demonstrate the possibility to reach such goal using a reliable validation metric.

This repository holds the primary pipeline codes used to process the _RegBL_ entries using the _swisstopo_ maps. It also comes with a specific tool used to create temporal representation of the situation of buildings using maps and _RegBL_ information.

## Research Project Links

The following links give access to the codes related to the project :

* [Primary pipeline - Construction dates extraction using maps (This repository)](https://github.com/swiss-territorial-data-lab/regbl-poc)
* [Secondary pipeline - Construction dates extraction without maps](https://github.com/swiss-territorial-data-lab/regbl-poc-intyearpolator)
* [Results and analysis tools for the primary pipeline](https://github.com/swiss-territorial-data-lab/regbl-poc-analysis)

The following links give access to official documentations on the considered data :

* [RegBL : Swiss federal register of buildings and dwellings](https://www.bfs.admin.ch/bfs/en/home/registers/federal-register-buildings-dwellings.html)
* [Maps : Swiss national maps 1:25'000](https://shop.swisstopo.admin.ch/en/products/maps/national/lk25)

## regbl-poc

In the first place, the primary pipeline is split into step-processes that are each responsible of achieving a specific part of the overall process. The following links gives access to the documentation of each of the step-processes :

* [Processing structure bootstrap](src/regbl-bootstrap)
* [Source map segmentation](src/regbl-segmentation)
* [Building detection](src/regbl-detect)
* [Construction date deduction](src/regbl-deduce)
* [Timelines composer](src/regbl-tracker)

The pipeline works by considering a main storage directory in which everything is stored, including source maps, segmented maps, intermediates results and final results. This directory has to be created in the first place :

    .../regbl_process

The geographical area on which the process has to take place need to be defined. It has to be a geographical region on which historical maps are available. For example, the _Bern_ area used during the research project is defined in terms of its _EPSG:2056_ boundaries :

    2596000 2602000 1197000 1203000

giving the _easting_ (longitude) and _northing_ (latitude) minimum and maximum values. A sequence of historical maps covering the selected area has to be gathered. In the case of this research project, we used the _swisstopo_ 1:25'000 national maps (_KOMB_). We highly recommend to use maps with the same scale and coming with a geographical ratio identical to the ratio of the geographical area (1:1 in terms of _easting_ and _northing_ pixel edge).

As an illustration, the following image gives a sample of the maps chosen to cover the _Bern_ area during this research project :

<p align="center">
<img src="doc/image/map-bern-timeline.jpg?raw=true" width="768">
<br />
<i>Sample of the considered _Bern_ maps (1993, 1998, 2004, 2010) - Swisstopo, 1:25'000 national maps</i>
</p>

As one can see, the maps are squared, as the selected geographical area (1:1 pixels edge). The original maps are placed in the following directory :

    .../regbl_process/regbl_frame/frame_original/[year].tif (2010.tif, 2004.tif ...)

The _TIFF_ format is mendatory, even the geotiff information are not considered.

The next step is to apply the [segmentation](src/regbl-segmentation) process on each of the selected map to clean them and extract the pixel footprint of the buildings. The following image gives and illustration of the obtained segmented (pre-processed) maps :

<p align="center">
<img src="doc/image/map-bern-timeline-segmented.jpg?raw=true" width="768">
<br />
<i>Result of the segmentation process applied on the previous maps</i>
</p>

The pre-processed maps have to be stored in the :

    .../regbl_process/regbl_frame/frame/[year].tif (2010.tif, 2004.tif ...)

directory. These segmented maps are the one used to performed the detection and deduction of the building construction date.

A last element is needed to fully initialise the main storage directory, that is the 3D raster descriptor file. It is a simple text file that gives, line by line, the year of the maps, its geographical boundaries and the width and height of the corresponding segmented map :

    2010 2596000 2602000 1197000 1203000 4800 4800
    2004 2596000 2602000 1197000 1203000 4800 4800
    1998 2596000 2602000 1197000 1203000 4800 4800
    1993 2596000 2602000 1197000 1203000 4800 4800
    ...

The entries has to be given in the decreasing temporal order. The file storing these information as to be named _regbl_list_ and place in the main storage directory.

As the main directory is prepared as described, the three successive processing steps can take place, starting with the _bootstrap_ one :

    ./regbl-bootstrap -s .../regbl_process -g .../RegBL/GEB.dsv -e .../RegBL/EIN.dsv

which reads the _RegBL_ _GEB_ and _EIN_ database to extract the relevant building information based on the defined geographical area. The detection process can then takes place :

    ./regbl-detect -s .../regbl_process

which performs the building detection on each segmented map (3D raster slices). The last processing steps can then be run as follows :

    ./regbl-deduce -s .../regbl_process

which performs the final deduction of the building construction temporal range. The results of this last step can be found in the :

    .../regbl_process/regbl_output/output_deduce

where a large amount of files, one per building, can be accessed to read the upper and lower boundaries of the deduced construction range. The files are all named after the _EGID_ of their building.

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
