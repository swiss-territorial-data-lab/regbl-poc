## Overview

This program is used to bootstrap the main storage directory used to perform building construction date base on maps on the defined geographical area.

It starts by creating the required sub-directories before to read the _RegBL_ database. In the first place, the _GEB_ database is read to extract building position, _EGID_, available construction dates and surfaces. These information are then dispatched in their respective sub-directories to ease their access by subsequent processes.

The _EIN_ database is then read to extract the available position of the building entries. Building can have no, one or more entries specified in the database. The _EGID_ is used as a link to append the position of the entries to the position of their respective building.

The program also filter the buildings using the definition of the geographical 3D raster used to perform the detection of the construction date. The 3D raster descriptor file is then needed in the main storage directory (*regbl_list* file).

## Program outputs

This program creates and populates the following sub-directories of the main storage directory :

    output_egid            Empty files name after the buildings EGID

    output_position/[year] Files named after buildings EGID containing their 
    _                      position and the position of their entries (when 
    _                      available)

    output_reference       Files named after buildings EGID that comes with a
    _                      construction date in the RegBL database

    output_surface         Files named after buildings EGID containing the
    _                      building surface (when available)

These directories can contains large amount of file, depending on the size and building population of the selected geographical area.

## Usage

The program is used in the following way :

    $ ./regbl-bootstrap -s .../main/directory/path -g .../GEB/DSV/file [-e .../EIN/DSV/file]

The first parameter has to provide the path of the main storage directory in which the 3D raster descriptor can be found (_regbl__list_ file). The directory is then filled with the extracted information and dispatched in their respective sub-directories.

The two last parameters have to give the path of the _RegBL_ _GEB_ and _EIN_ _DSV_ files. These file are read to extract the information about the buildings. Be sure to specify the path of the _Data_ _DSV_ files (not the _Readme_ ones).

Specifying the _EIN_ database is optional. If the _EIN_ database is not specified, the entries are not extracted, and then, not considered for subsequent building detection and construction date processes.
