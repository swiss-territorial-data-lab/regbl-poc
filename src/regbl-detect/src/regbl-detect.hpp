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

    /* define string buffer length */
    # define REGBL_BUFFER 8192

    /* define delimiter */
    # define REGBL_DELIMITER '\t'

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

    /*! \brief Processing methods
     *
     *  This function performs the actual building detection on the current
     *  provided map. It uses the map, expected to be pre-processed, colors and
     *  the building coordinates to decided whether or not the building is
     *  there.
     *
     *  The function starts by reading the header of the DSV database before to
     *  search for the entries :
     *
     *      EGID, GKODE, GKODN, GBAUJ
     *
     *  which corresponds to the ID of the buildings, their coordinates and the
     *  construction date, when available.
     *
     *  As the entries are found in the header, the function starts parsing the
     *  provided RegBL DSV database segment file. It is parsed line by line,
     *  each one describing one building.
     *
     *  For each building, the function checks if its coordinates are on the map
     *  or not using the provided boundaries. In case the building is on the
     *  map, it's coordinates are converted from CH1903+/LV95 to the pixel
     *  coordinates of the map.
     *
     *  Using the pixel coordinates of the building and the map itself, the
     *  function determine if the building is on the map or not. To do so, the
     *  color of the underlying pixel at the position of the building is at its
     *  maximum value (255). In such a case, the detection succeed.
     *
     *  For each building, the function uses the provided exportation path to
     *  create or append to a file the result of the detection. It add a line to
     *  the file containing the year of the considered map a a zero or a one
     *  indicating that the building was not found or found, respectively.
     *
     *  The result are append in files in order to have the history of the
     *  detection for each building in a single file. The name of the file is
     *  set using the considered building unique ID (EGID).
     *
     *  In addition, the function also update the map itself to indicate the
     *  position of each detection and its results. In case the detection
     *  failed, a 64-valued pixel is set, a 192-valued pixel otherwise. This
     *  allows to export the map afterwards to have a graphical view of the
     *  detection.
     *
     *  The provided map is expected to be a OpenCV greyscale matrix containing
     *  only black (0) and white (255) pixel in uint8 (uchar) format. The white
     *  pixels are used to map building while black ones are used for the rest.
     *
     *  \param regbl_database Path of the RegBL DSV database segment file
     *  \param regbl_map      OpenCV matrix containing the map
     *  \param regbl_export   Directory in which files are exported
     *  \param regbl_year     String containing the year of the map
     *  \param regbl_xmin     Location boundaries coordinate : low E - EPSG:2056
     *  \param regbl_xmax     Location boundaries coordinate : high E - EPSG:2056
     *  \param regbl_ymin     Location boundaries coordinate : low N - EPSG:2056
     *  \param regbl_ymax     Location boundaries coordinate : high N - EPSG:2056
     */

    void regbl_detect( char const * const regbl_database, cv::Mat & regbl_map, std::string & regbl_export, std::string & regbl_year, double const regbl_xmin, double const regbl_xmax, double const regbl_ymin, double const regbl_ymax );

    /*! \brief Processing methods
     *
     *  This function is used to extract the reference information about the
     *  construction date of the building in the considered RegBL database
     *  segment.
     *
     *  The function starts by reading the header of the DSV database before to
     *  search for the entries :
     *
     *      EGID, GKODE, GKODN, GBAUJ
     *
     *  which corresponds to the ID of the buildings, their coordinates and the
     *  construction date, when available.
     *
     *  As the entries are found in the header, the DSV database is parsed line
     *  by line, each line corresponding to one building.
     *
     *  For each line, the function extracts the four entries and only consider
     *  the building that are within the current map (location). This filtering
     *  is made using the provided boundaries in CH1903+/LV95.
     *
     *  As the building is on the current processed map, its construction date
     *  is extracted and analysed. If a date is available, it is exported in
     *  the building file. If not, minus one is exported instead.
     *
     *  A exportation file is created for each building using its ID (EGID) to
     *  name it. It follows that this function creates a large amount of small
     *  files.
     *
     *  \param regbl_database Path of the RegBL DSV database segment file
     *  \param regbl_export   Directory in which files are exported
     *  \param regbl_xmin     Location boundaries coordinate : low E - EPSG:2056
     *  \param regbl_xmax     Location boundaries coordinate : high E - EPSG:2056
     *  \param regbl_ymin     Location boundaries coordinate : low N - EPSG:2056
     *  \param regbl_ymax     Location boundaries coordinate : high N - EPSG:2056
     */

    void regbl_detect_reference( char const * const regbl_database, std::string & regbl_export, double const regbl_xmin, double const regbl_xmax, double const regbl_ymin, double const regbl_ymax );

    /*! \brief Database methods
     *
     *  This function is used to compute the index of an DSV database entry
     *  based on its header name. The header line of the considered database
     *  has then to be provided. The DSV database is expected to use tabulation
     *  as separator.
     *
     *  Considering the following DSV database header :
     *
     *      head_0 \t head_1 \t head_2
     *
     *  and providing 'head_1' as entry target (entry name), the function
     *  returns with index one.
     *
     *  This function is used to analyse the DSV database header allowing to
     *  subsequently use the regbl_detect_database_entry() function to reach
     *  any entries with a simple index.
     *
     *  \param regbl_line   DSV database header line, using tabulation as separator
     *  \param regbl_target Name of the entry in the DSV header
     *
     *  \return Index of the entry detected in the header using its name.
     */

    int regbl_detect_database_header( char const * const regbl_line, char const * const regbl_target );

    /*! \brief Database methods
     *
     *  This function is used to search in the database lines the element of the
     *  table corresponding to the provided index. The line is expected to be
     *  part of a DSV file using tabulation as separators. The index is used to
     *  count the amount of element to jump over to reach the desired one.
     *
     *  For example, providing the following line :
     *
     *      entry_0 \t entry_1 \t entry_2
     *
     *  and one as index leads the function to select the 'entry_1' string. The
     *  string is returned through the \b regbl_token string.
     *
     *  \param regbl_line   DSV database line, using tabulation as separator
     *  \param regbl_target Index of the element to search
     *  \param regbl_token  String through which the found entry is returned
     */

    void regbl_detect_database_entry( char const * const regbl_line, int const regbl_target, char * const regbl_token );

    /*! \brief Main function
     *
     *  This program is used to detect the presence or absence of building on
     *  the provided pre-processed geotiff maps. The detection is simply based
     *  on color values, the pre-processing of the maps being in charge of the
     *  complex preparation task :
     *
     *      ./regbl_detect --database/-d [path of the RegBL DSV file]
     *                     --storage/-s [path of the storage directory]
     *
     *  The main function starts by reading the arguments and parameters to
     *  check them. In the provided storage directory, a file named 'regbl_list'
     *  is opened and is expected to give the location name, the year and the
     *  map boundaries coordinates, separated by spaces :
     *
     *      bern  2015 2596000 2602000 1197000 1203000
     *
     *  the coordinates are expected to be in the CH1903+/LV95 with min. and max
     *  of easting and northing. The list can contain as many entries as it is
     *  required.
     *
     *  The input pre-processed maps are expected to be located in the storage
     *  directory with path and name :
     *
     *      [storage_directoy]/regbl_frame/frame_[location]/[location]_[year].tif
     *
     *  with location and year as specified in the list.
     *
     *  Each map is expected to be a black and white image with building in
     *  white and the rest (nodata) in black. The image can be a color or simply
     *  greyscale image.
     *
     *  As the first map is loaded by the main function, the processing of the
     *  RegBL database DSV file starts. In the first place, only when a new
     *  location is considered, the database is parsed to find the construction
     *  date of each building in the base, when available.
     *
     *  The results of this process are exported in the storage directory in the
     *  following path :
     *
     *      [storage_directory]/regbl_output/output_reference/reference_[location]
     *
     *  each building construction date is stored in a file named after the
     *  building RegBL EGID. See :
     *
     *      regbl_detect_reference()
     *
     *  documentation for more information.
     *
     *  For each location and each of its available year, the detection process
     *  takes place. The RegBL database DSV file is parsed and the coordinates
     *  of each building is considered for two thing : the first one is to
     *  determine if the building is in the map boundaries; the second one is
     *  to detect on the map if the building appears (white pixels) or not.
     *
     *  For each building, the result of the detection is stored in the path :
     *
     *      [storage_directory]/regbl_output/output_database/database_[location]
     *
     *  Again, a file is created for each building using its EGID to name it. As
     *  the building is detected, the year and a one are exported, the year and
     *  a zero otherwise. See :
     *
     *      regbl_detect()
     *
     *  documentation for more information.
     *
     *  In addition, the detection process put a mark at each building position
     *  on the current processed map to indicates whether or not the building
     *  was found. The update map is then exported in the following directory :
     *
     *      [storage_directory]/regbl_output/output_frame/frame_[location]
     *
     *  and named :
     *
     *      [location]_[year].tif
     *
     *  This allows users to check graphically what append during the detection
     *  process.
     *
     *  The main function ends as all the maps specified in the list are fully
     *  processed and all results exported.
     *
     *  Notes on error management : if any file access, in both input and output
     *  modes fails, the programs displays and error in the standard error and
     *  stops sending 1 to the system.
     *
     *  \param argc Standard main parameter
     *  \param argv Standard main parameter
     *
     *  \return Returns standard system codes.
     */

    int main( int argc, char ** argv );

/*
    header - inclusion guard
 */

    # endif

