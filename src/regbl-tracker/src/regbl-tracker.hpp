/*
 *  regbl - tracker
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

    /*! \file   regbl-tracker.hpp
     *  \author Nils Hamel <nils.hamel@alumni.epfl.ch>
     *  \author Huriel Reichel
     *
     *  regbl-tracker
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

    # ifndef __REGBL_TRACKER__
    # define __REGBL_TRACKER__

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

    /* define crops standard size */
    # define REGBL_TRACKER_CROPHALF ( 64 )
    # define REGBL_TRACKER_CROSHALF ( 32 )

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

    /*! ... */

    void regbl_tracker_crop( cv::Mat & regbl_crop, int const regbl_cnx, int const regbl_cny, int const regbl_detect );

    /*! ... */

    void regbl_tracker_building( cv::Mat & regbl_crop, int const regbl_cnx, int const regbl_cny, std::ifstream & regbl_stream );

    /*! ... */

    void regbl_tracker_surface( cv::Mat & regbl_crop, int const regbl_cnx, int const regbl_cny, double const regbl_surface );

    /*! ... */

    void regbl_tracker_detect( cv::Mat & regbl_crop, double const regbl_dx, double const regbl_dy, double const regbl_size, bool const regbl_detect );

    /*! ... */

    cv::Mat regbl_tracker_timeline( int const regbl_width, std::string regbl_year, int const regbl_detect, bool const regbl_lbound, bool const regbl_ubound );

    /*! ... */

    cv::Mat regbl_tracker_reference( int const regbl_width, std::string regbl_geid, std::string regbl_year, std::string regbl_udeduce, std::string regbl_ldeduce );

    /*! \brief Main function
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

