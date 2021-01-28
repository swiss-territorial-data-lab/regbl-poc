/*
 *  regbl - common library
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

    /*! \file   common-connect.hpp
     *  \author Nils Hamel <nils.hamel@alumni.epfl.ch>
     *  \author Huriel Reichel
     *
     *  regbl - common library - connect
     */

/*
    header - inclusion guard
 */

    # ifndef __LC_CONNECT__
    # define __LC_CONNECT__

/*
    header - internal includes
 */

/*
    header - external includes
 */

    # include <vector>
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

    typedef std::vector < std::vector < int > > lc_connect_t;

/*
    header - structures
 */

/*
    header - function prototypes
 */

    /*! \brief ...
     *
     * lc_image and lc_mask : single channel image
     * lc_mask needs to be zero-valued
     */

    unsigned int lc_connect_get_size( cv::Mat & lc_image, cv::Mat & lc_mask, int const lc_x, int const lc_y, bool const lc_erase );

    /*! \brief ...
     *
     * lc_image and lc_mask : single channel image
     * lc_mask needs to be zero-valued
     */

    lc_connect_t lc_connect_get( cv::Mat & lc_image, cv::Mat & lc_mask, int const lc_x, int const lc_y, bool const lc_erase );

/*
    header - inclusion guard
 */

    # endif

