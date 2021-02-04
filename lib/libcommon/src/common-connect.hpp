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

    # include <iostream>
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

    /*! \brief Connectivity methods
     *
     * This function is a simple front-end to the lc_connect_get() function. It
     * only uses the returned connected structure to extract it size, in pixels
     * count and returns it.
     *
     * lc_image and lc_mask : single channel image
     * lc_mask needs to be zero-valued
     *
     * \return Size, in pixels, of the connected area.
     */

    unsigned int lc_connect_get_size( cv::Mat & lc_image, cv::Mat & lc_mask, int const lc_x, int const lc_y, bool const lc_erase );

    /*! \brief Connectivity methods
     *
     * This function is used to extract a connected pixel area definition based
     * on the provided initial pixel. The detection takes place on the provided
     * image that is expected to be a single-channel binary image {0,255}. The
     * connected area are always understood as black area (0).
     *
     * The process implements a simple neighbour searh of black pixel starting
     * by the provided one, until the entire area is converted. The coordinates
     * of each found black pixel are pushed, in x, y order, to the connect
     * return structure, which is a simple vector of ints vector.
     *
     * The function uses a secondary image, also a single-channel binary image
     * with same size as the image one, that it uses to keep track of the
     * already selected pixels. A pixel can be selected as part of the connected
     * area only if it is set to zero on the secondary image. Each time a pixel
     * is selected, it is whitened on the secondary image, indicating its
     * selection.
     *
     * The secondary image is then updated by the function. The erase flag can
     * be set to true to ask the function to erase all the update it made on the
     * seconary image, to keep it in its original state.
     * 
     * \param lc_image Source image, single-channel binary image {0,255}
     * \param lc_mask  Tracking image, single-channel binary image {0,255}
     * \param lc_x     Initial pixel position
     * \param lc_y     Initial pixel position
     * \param lc_erase Trakcer image erasing flag
     *
     * \return Returns vector of int vector (n by 2) containing the selected
     * pixel coordinates.
     */

    lc_connect_t lc_connect_get( cv::Mat & lc_image, cv::Mat & lc_mask, int const lc_x, int const lc_y, bool const lc_erase );

/*
    header - inclusion guard
 */

    # endif

