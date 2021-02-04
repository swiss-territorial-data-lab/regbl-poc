## Overview

This program is used to deduce the construction date of the building based on the results of the detection process. For each building, it analysis its detection file to determine the most probable construction date of the building.

The deduction process is based on the following assumption : the building has to be detected on the most recent map until it desapear from an older map. The construction date defined between the most recent map on which the building is missing and the oldest map on which it still appear.

As detection of the presence of building is based on a simple pixel-based process, it is not able to detect if an old building was destructed replaced by the considered one. A surface variation criterion is added to detect the changes in the morphology of the building. In such a case, the map on which the morphology has changed is interpreted as a missing detection for the attribution of the deduced range.

In case the building is not detected on the most recent map, it is assumed to have been built more recently that the most recent map. In case the building is detected on each map without change in morphology, it is considered to have been built before the oldest map. In such case, the value _+32767_ and _-32768_ are used to indicate the missing boundaries.

On the following timeline of a chosen building, one can see how the deduction algorithm assoicate a construction range based on the disapearance of the building on the oldest maps :

<p align="center">
<img src="deduction-example-strict.png?raw=true" width="768">
<br />
<i>Example of a simple detection based on building disapearance on the oldest maps</i>
</p>

On the following image, the example of a building showing a change in its morphology is provided. This illustrate how the detection process interpretes this as a disapearance despite a building continues to be detected on the oldest maps :

<p align="center">
<img src="deduction-example-break.png?raw=true" width="768">
<br />
<i>Example of a simple detection based on building disapearance on the oldest maps</i>
</p>

This timelines are created using the [_tracker_](../../src/regbl-tracker) tool.

## Usage

The construction date program is used in the following way :

    $ ./regbl-detect -s .../main/directory/path

The only parameter is then the main storage directory path. The deduction process expects the detection file to be computed for each building by the detection process.
