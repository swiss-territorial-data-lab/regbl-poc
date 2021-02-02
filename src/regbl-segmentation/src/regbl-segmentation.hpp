/*
 *  regbl - segmentation
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

    /*! \file   regbl-segmentation.hpp
     *  \author Nils Hamel <nils.hamel@alumni.epfl.ch>
     *  \author Huriel Reichel
     *
     *  regbl-segmentation
     */

    /*! \mainpage regbl
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

    # ifndef __REGBL_SEGMENTATION__
    # define __REGBL_SEGMENTATION__

/*
    header - internal includes
 */

/*
    header - external includes
 */

    # include <iostream>    
    # include <string>
    # include <common-include.hpp>
    # include <opencv2/core/core.hpp>
    # include <opencv2/highgui/highgui.hpp>
    # include <opencv2/imgproc/imgproc.hpp>

/*
    header - preprocessor definitions
 */

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

    typedef std::vector< std::vector< std::vector< double > > > regbl_component_t;

/*
    header - structures
 */

/*
    header - function prototypes
 */

    int regbl_segmentation_io_state( cv::Mat & regbl_image, int regbl_state, std::string regbl_path );

    void regbl_segmentation_tool_equalize( cv::Mat & regbl_in );

    void regbl_segmentation_process_extract_black( cv::Mat & regbl_in, cv::Mat & regbl_out, int const regbl_l2dist );

    void regbl_segmentation_process_conway_iteration( cv::Mat & regbl_in, cv::Mat & regbl_out, int const regbl_kernel, float const regbl_gamevalue );

    void regbl_segmentation_process_extract_building( cv::Mat & regbl_clean, cv::Mat & regbl_source, cv::Mat & regbl_output, int const regbl_tolerence );

    int main( int argc, char ** argv );

/*
    header - inclusion guard
 */

    # endif

