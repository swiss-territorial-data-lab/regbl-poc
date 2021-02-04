## Overview

This program allows to compute synthetic representation of the situation of a specific building according to the considered 3D raster slices (maps). The representation also shows the results of the detection and deduction process to provide a full and comprehenisve view of the building situation.

The following image shows and example of such representation for a chosen building on the city of _Basel_ :

<p align="center">
<img src="../../doc/image/deduction-example-strict.png?raw=true" width="768">
<br />
<i>Example of a computed timeline for a Basel building</i>
</p>

The two main elements of the timeline are the two succession of crops, centred on the desired building, showing the evolution of its situation across the time. The top timeline uses the original maps to create the crops while the bottom one uses the pre-processed maps. This allows to determine in which extend the pre-processing of the map can be responisble of mistakes.

On each crop, a series of pink elements are rendered. The pink small crosses indicates the position of the building along with the position of its entries, when available. The entries are linked to the position of the building using a pink line. The position of the building is the center of the representation. A pink circle is also used to show the surface of the building provided by the _RegBL_ database, when available.

Two other green element can be seen in case a building was detected : the green cross shows the format position of the building detection that can slightly differs from the position of the building itself. The green circle is used to represent the surface of the connected pixel area of the building.

The top indicator gives the building _EGID_, the deduced construction date range and the construction date provided by the _RegBL_ database, when available.

The middle indicator gives the dates of each crop while the bottom inidicator shows the lifespace of the building based on its deduced construction date range.

The top indicator background is set to green if the deduced construction date range is consistent with the _RegBL_ construction date, and red otherwise. If the _RegBL_ provides no construction date, the background is left grey.

## Usage

The tracker prorgam is used in the following way :

    $ ./regbl-tracker -s .../main/directory/path -g 441411 -e .../exportation/directory

The first parameter has to provide the main storage directory path while the second has to give the _EGID_ of the desired building. The _EGID_ has then to corresponds to a building that is available on the geographical area covered by the 3D raster of the main storage directory.

The last parameter has to give a path to a directory in which the timeline is exported as an image using the _EGID_ to name it. The _PNG_ format is used.

The tracker expects a fully processed main storage directory to be able to compute the timeline, including detection and deduction process steps.
