## Overview

This tools is dedicated to source map processing to extract the building footprints. As the _RegBL_ completion project uses map to detect the construction date of buildings, the maps need to be cleaned up in order to limit as much as possible their content to the building footprints, leaving only the relevant information needed.

The process takes place in steps : considering the following map as an example :

<p align="center">
<img src="../../doc/image/map-basel-2005.jpg?raw=true" width="256">
&nbsp;
<img src="../../doc/image/map-basel-2005-crop.jpg?raw=true" width="256">
<br />
<i>Exemple of considered map : Basel in 2005 and closer view - Swisstopo, 1:25'000 national maps</i>
</p>

The first step consists in image equalization to compensate any bias in their digitization process. This step is optional and has to be specified through a flag to take place. The _exposure_ of the maps are then standardized in such a way.

The next steps consists in black pixel extraction. Each pixel of the input map is tested to determine whether or not it can be considered as black using specific threshold. As the building are drawn in black, extracting blacks pixel is a first way of separating the buildings from the rest of the symbology. The following result is obtained :

<p align="center">
<img src="../../doc/image/map-basel-2005-black.jpg?raw=true" width="256">
&nbsp;
<img src="../../doc/image/map-basel-2005-crop-black.jpg?raw=true" width="256">
<br />
<i>Results of the black extraction process</i>
</p>

As one can see on the result of the black extraction process, the building are still highly connected to other symbological elements and to each others in some cases. Having the building footprints well separated and well defined is an important point for subsequent processes responsible of construction data deduction. To achieve it, two steps are added. The first one uses a variation of the Conway game of live to implement a morphological operator able to disconnect pixel groups. The following image gives the results of this separation process along with the previous black extraction result on which it is based :

<p align="center">
<img src="../../doc/image/map-basel-2005-crop-black.jpg?raw=true" width="256">
&nbsp;
<img src="../../doc/image/map-basel-2005-crop-conway.jpg?raw=true" width="256">
<br />
<i>Results of the morphological operator (right) compare to the previous black extraction (left)</i>
</p>

As the morphological operator provide the desired result, it also shrinks the footprints of the elements. It allows to eliminate a lot of structure that are not building but it also reduces the footprint of the building, which can increase the amount work to perform by the subsequent processes. To solve this issue and obtain building footprints that are as close as possible to the original map, a controlled re-growing step is added. It uses a region threshold and the black extraction result to re-grow the buildings without going any further of their original definition. The following images gives a view of the final result along with the original map :

<p align="center">
<img src="../../doc/image/map-basel-2005-crop.jpg?raw=true" width="256">
&nbsp;
<img src="../../doc/image/map-basel-2005-crop-final.jpg?raw=true" width="256">
<br />
<i>Final result of the building footprint extraction (right) compared to the original map</i>
</p>

As one can see, as the Conway morphological operator is not able to get rid of all the non-building elements, such as large and bold texts, the re-growing final step also thicken them. The result of the building extraction then provides very good footprints but without being able to eliminate all the undesired elements.

## Usage

The segmentation tool can be used in the following way :

    $ ./regbl_segmentation -i .../source/map.tif -o .../result/export/map.tif [-e] [-i] [-s .../export/steps.tif]

The first and second parameters simple give the path of the source map to process and the location of the result to export. Any image format accepted by _opencv_ can be considered.

The equalization flag (_-e_), can be provided to ask the process to equalize the source image before to perform the processing steps. The inversion flag (_-i_) works the same for color inversion.

The last path that can be specified tells the process to export the map at each step of the process allowing to understand exactly what is the result of each step.
