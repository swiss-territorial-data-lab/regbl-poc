## Overview

This repository is related to the _RegBL (RBD/GWR/RegBL) completion_ research project. The _STDL_ was contacted by the Swiss Federal Statistical Office (_OFS_) to determine in which extend it could be possible to complete the construction date of Swiss buildings based on the analysis of a temporal sequence of the Swiss federal maps produced by _swisstopo_. With an initial target of _80%_ of correct guesses, the goal of this research project was to demonstrate the possibility to reach such goal using a reliable validation metric.

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

* [Source map segmentation](src/regbl-segmentation)
* [Processing structure bootstrap](src/regbl-bootstrap)
* [Building detection](src/regbl-detect)
* [Construction date deduction](src/regbl-deduce)
* [Timelines composer](src/regbl-tracker)

### Preparation

The pipeline works by considering a main storage directory in which everything is stored, including source maps, segmented maps, intermediate results and final results. This directory has to be created in the first place :

    .../regbl_process

The geographical area on which the process has to take place need to be defined. It has to be a geographical region on which historical maps are available. For example, the _Bern_ area used during the research project is defined in terms of its boundaries (_EPSG:2056_), giving the _easting_ (longitude) and _northing_ (latitude) minimum and maximum values :

    2596000 2602000 1197000 1203000

A sequence of historical maps, available in _geotiff_, covering the selected area has to be gathered. In the case of this research project, we used the _swisstopo_ 1:25'000 national maps (_KOMB_). We highly recommend to use maps with the same scale and coming with a geographical ratio identical to the ratio of the geographical area (1:1 in terms of _easting_ and _northing_ pixel geographical edges). In addition, the [segmentation](src/regbl-segmentation) process was designed to work on the swisstopo maps. Some adaptation could be required to work on other scales.

The source maps have to be cropped according to the defined geographical area. We recommend using the [_GDAL tool_](https://gdal.org/programs/gdalwarp.html) to perform the geographical crop. For the _Bern_ example, one can use :

    $ gdalwarp -of gtiff -te 2596000 1197000 2602000 1203000 source_map.tif [year].tif

The original cropped maps have to be placed in the following directory (the _TIFF_ format is mendatory) :

    .../regbl_process/regbl_frame/frame_original/[year].tif (2010.tif, 2004.tif ...)

As an illustration, the following image gives a sample of the cropped maps chosen to cover the _Bern_ area during this research project. As one can see, the maps are square, as the selected geographical area (1:1 pixel edges) :

<p align="center">
<img src="doc/image/map-bern-timeline.jpg?raw=true" width="768">
<br />
<i>Sample of the considered maps of Bern (1993, 1998, 2004, 2010) - swisstopo, 1:25'000 national maps</i>
</p>

The next step is to apply the [segmentation](src/regbl-segmentation) process on each of the selected map to clean them and to extract the pixel footprint of the buildings. The maps have to be processed one by one using the [segmentation](src/regbl-segmentation) tool. The pre-processed maps have to be stored in the :

    .../regbl_process/regbl_frame/frame/[year].tif (2010.tif, 2004.tif ...)

directory and named after their year. These segmented maps are the one used to performed the detection and deduction of the building construction date. The following image gives and illustration of the obtained segmented (pre-processed) maps :

<p align="center">
<img src="doc/image/map-bern-timeline-segmented.jpg?raw=true" width="768">
<br />
<i>Result of the segmentation step-process applied on the previous maps</i>
</p>

A last element is needed to fully initialise the main storage directory, that is the 3D raster descriptor file. It is a simple text file that gives, line by line, the year of the maps, its geographical boundaries and the width and height (in pixels) of the corresponding segmented map :

    2010 2596000 2602000 1197000 1203000 4800 4800
    2004 2596000 2602000 1197000 1203000 4800 4800
    1998 2596000 2602000 1197000 1203000 4800 4800
    1993 2596000 2602000 1197000 1203000 4800 4800
    ...

The entries have to be given in the decreasing temporal order. The file storing these information as to be named _regbl_list_ and place in the main storage directory.

### Processing

As the main directory is prepared as described, the three successive processing steps can take place, starting with the [_bootstrap_]((src/regbl-bootstrap)) one :

    $ ./regbl-bootstrap -s .../regbl_process -g .../RegBL/GEB.dsv -e .../RegBL/EIN.dsv

which reads the _RegBL_ _GEB_ and _EIN_ databases to extract the relevant buildings information based on the defined geographical area through the 3D raster descriptor. Note that specifying the _EIN_ database is not mandatory. In case the _EIN_ database is not provided, the entries of the buildings are not considered for the deduction of their construction date.

The [_detection_](src/regbl-detect) process can then takes place :

    $ ./regbl-detect -s .../regbl_process

which performs the building detection on each segmented map (3D raster slices). The [_deduction_](src/regbl-deduce) process can then takes place :

    $ ./regbl-deduce -s .../regbl_process

which performs the final deduction of the building construction temporal range. The results of this last step can be found in the :

    .../regbl_process/regbl_output/output_deduce

where a large amount of files, one per building, can be accessed to read the upper and lower boundaries of the deduced construction range. The files are all named after the _EGID_ of their building.

### Analysis

This repository also offers a way to represent the situation of a building across the 3D raster along with information on the detection and deduction processes. Based on a given _EGID_, the [_tracker_](src/regbl-tracker) can be used to compute the representation :

    $ ./regbl-tracker -s .../regbl_process -g 1269938 -e .../export/directory

The representation is a _PNG_ image named after the building _EGID_. The following image gives an example of such representation :

<p align="center">
<img src="doc/image/deduction-example-strict.png?raw=true" width="768">
<br />
<i>Example of building situation and results representation</i>
</p>

The scripts proposed by the [results and analysis tool]((https://github.com/swiss-territorial-data-lab/regbl-poc-analysis)) can also be considered to analysis the results more in details.

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
