## Overview

This program is responsible of building detection on the 3D raster slices. It then performs on a bootstraped main storage directory containing the pre-processed maps.

The detection is based on the pre-processed map by looking at a small region around the position of the building provided by the _RegBL_ database. This region-based detection is made to partially solve the problem of missalignment of the building position on the maps.

For each building position, a tracker map is updated accroding to the result of the detection. The tracker map is a transparent overlay on which red and green crosses are drawn to inform of the detection or not of each builing, using their position. The following image gives an example of the overlay superimposed on an orginal map and its pre-processed counter part :

<p align="center">
<img src="../../doc/image/map-basel-2005-tracker-original.jpg?raw=true" width="256">
&nbsp;
<img src="../../doc/image/map-basel-2005-tracker-segmented.jpg?raw=true" width="256">
<br />
<i>Detection tracker overlay superimposed on original map (left) and pre-processed map (right) </i>
</p>

An overlay is exported for each slice of the 3D raster.

## Usage

The detection process is used in the following way :

    ./regbl-detect -s .../main/directory/path

The only parameter is then the main storage directory path. The detection process expects the main storage directory to be bootstraped and filled with the pre-processed maps.
