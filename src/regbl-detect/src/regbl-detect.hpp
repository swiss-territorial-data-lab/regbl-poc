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

    void regbl_detect( char const * const regbl_database, cv::Mat & regbl_map, std::string & regbl_export, std::string & regbl_year, double const regbl_xmin, double const regbl_xmax, double const regbl_ymin, double const regbl_ymax );

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

