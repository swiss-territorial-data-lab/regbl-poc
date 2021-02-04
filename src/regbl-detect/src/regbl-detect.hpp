/*
 *  regbl - detect
 *
 *      Nils Hamel - nils.hamel@alumni.epfl.ch
 *      Huriel Reichel
 *      Copyright (c) 2020 Republic and Canton of Geneva
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

    /*! \file   regbl-detect.hpp
     *  \author Nils Hamel <nils.hamel@alumni.epfl.ch>
     *  \author Huriel Reichel
     *
     *  regbl-detect
     */

    /*! \mainpage regbl-poc
     *
     *  \section license Copyright and License
     *
     *  **regbl** - Nils Hamel, Huriel Reichel <br >
     *  Copyright (c) 2020 Republic and Canton of Geneva
     *  
     *  This program is licensed under the terms of the GNU GPLv3. Documentation
     *  and illustrations are licensed under the terms of the CC BY 4.0.
     */

/*
    header - inclusion guard
 */

    # ifndef __REGBL_DETECT__
    # define __REGBL_DETECT__

/*
    header - internal includes
 */

/*
    header - external includes
 */

    # include <iostream>    
    # include <string>
    # include <fstream>
    # include <filesystem>
    # include <common-include.hpp>
    # include <opencv2/core/core.hpp>
    # include <opencv2/highgui/highgui.hpp>
    # include <opencv2/imgproc/imgproc.hpp>

/*
    header - preprocessor definitions
 */

    /* define detection cross size */
    # define REGBL_DETECT_CROSS ( 3 )

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

/*
    header - function prototypes
 */

    /*! \brief Detection methods
     *
     * This function is use to detect the presence or absence of a building on
     * the provided map. The map is expected to be pre-processed into a binary
     * image {0,255} on which buildings are specified in black.
     *
     * The detection is based on the provided position of the building. As the
     * position come from the RegBL database, it can be de-synchronised with the
     * building on the map. The function then uses a detection area to not only
     * relies on a single pixel to determine the presence of a building.
     *
     * The detection area is a simple small circle (hard-coded) around the 
     * position of the building.
     *
     * As the building is detected by the function, the detection pixel is set
     * in the provided position (provided as pointers). This allows the parent
     * process to know which pixel lead to the detection.   
     *
     * \param regbl_map Pre-processed map, single channel binary image
     * \param regbl_x   Position of the building, in pixels, (updated with the
     *                  detection position)
     * \param regbl_y   Position of the building, in pixels, (updated with the
     *                  detection position)
     */

    bool regbl_detect_on_map( cv::Mat & regbl_map, int * const regbl_x, int * const regbl_y );

    /*! \brief Detection methods
     *
     * This function performs the actual building detection on the current
     * provided map. It uses the map, expected to be a pre-processed binary
     * image {0,255}, and the building coordinates to decided whether or not
     * the buildings are there.
     *
     * Considering the provided map, corresponding to a specific year (slice of
     * the 3D raster), the function parses all the building files associate to
     * the map.
     *
     * For each building, the function uses the regbl_detect_on_map() function
     * to determine whether or not the considered building is there. It updates
     * the tracking overlay to graphically show each detection results (red
     * cross on missing building, green cross otherwise).
     *
     * As the building is detected, the function extracts the connected pixel
     * area based on the detection position.
     *
     * The function then update the detection file of each building by adding a
     * new line containing the year of the considered map, the detection result
     * (detected 1, 0 otherwise) and the formal detection position, in pixels.
     *
     * \param regbl_map             Pre-processed map, single channel binary image
     * \param regbl_track           Tracking overlay image, expected to be an RGBA image
     * \param regbl_export_egid     EGID files directory
     * \param regbl_export_position Building position files directory
     * \param regbl_export_detect   Building detection files directory
     * \param regbl_year            Year of the provided map (3D raster slice)
     */

    void regbl_detect( cv::Mat & regbl_map, cv::Mat & regbl_track, std::string & regbl_export_egid, std::string & regbl_export_position, std::string & regbl_export_detect, std::string & regbl_year );

    /*! \brief Main function
     *
     * This program is used to detect the presence or absence of building on
     * the provided pre-processed maps. The detection is a simple pixel-based
     * detection :
     *
     *      ./regbl_detect --storage/-s path of the main storage directory
     *
     * The program starts by reading the 3D raster descriptor file located in
     * the main storage directory. This allows it to get the list of the maps
     * that are available on this geographical area (3D raster slices).
     *
     * It then load each pre-processed map out of the expected sub-directory of
     * the main storage directory :
     *
     *     .../regbl_frame/frame/[year].tif
     *
     * After checking the map and descriptor consistency, the program prepares
     * the tracking map and the connected region exclusion map adapted to the
     * loaded map.
     *
     * It then performs the detection of the buildings on the current map. For
     * each map, the tracking map is exported at the end of the detection as an
     * overlay to allows it to be superimposed with the pre-processed or 
     * original maps to visually analyse what happened.
     *
     * The overlays are exported in the sub-directory :
     *
     *     .../regbl_output/output_frame/[year].tif
     *
     * which can then be used as overlays on maps.
     *
     * \param argc Standard parameter
     * \param argv Standard parameter
     *
     * \return Exit code
     */

    int main( int argc, char ** argv );

/*
    header - inclusion guard
 */

    # endif

