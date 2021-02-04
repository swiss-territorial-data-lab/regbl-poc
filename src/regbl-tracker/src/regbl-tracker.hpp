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

    /*! \brief Rendering methods
     * 
     * Thus function draws the edge overlay and the centered cross focusing on
     * the position of the building. The color of the elements is set to green
     * if the building was detected on the map, in red otherwise.
     *
     * \param regbl_crop   Building map crop
     * \param regbl_cnx    Building pixel position
     * \param regbl_cny    Building pixel position
     * \param regbl_detect Building detection flag for the considered map
     */

    void regbl_tracker_crop( cv::Mat & regbl_crop, int const regbl_cnx, int const regbl_cny, int const regbl_detect );

    /*! \brief Rendering methods
     * 
     * This function draws the position of the building and the position of its
     * entries when available. The position of the building is shown by a cross
     * as the entries. Lines are also rendering to link the entries position to
     * the position of the building.
     *
     * As these information are extracted from the RegBL database, they are
     * rendered in pink.
     *
     * \param regbl_crop   Building map crop
     * \param regbl_cnx    Building pixel position
     * \param regbl_cny    Building pixel position
     * \param regbl_stream Building position file path
     */

    void regbl_tracker_building( cv::Mat & regbl_crop, int const regbl_cnx, int const regbl_cny, std::ifstream & regbl_stream );

    /*! \brief Rendering methods
     * 
     * This function renders a circle with surface corresponding to the building
     * surface as found in the RegBL database. Nothing is drawn if the surface
     * information is missing in the database.
     *
     * As the surface value is extracted from the RegBL database, it is rendered 
     * in pink.
     *
     * \param regbl_crop    Building map crop
     * \param regbl_cnx     Building pixel position
     * \param regbl_cny     Building pixel position
     * \param regbl_surface Building surface value, in squared pixels
     */

    void regbl_tracker_surface( cv::Mat & regbl_crop, int const regbl_cnx, int const regbl_cny, double const regbl_surface );

    /*! \brief Rendering methods
     * 
     * As the building detection process is made using small area around the
     * position of the building, the detection position can slightly differs
     * from the position of the building. If the building was not detected on
     * the cropped map, nothing is done.
     *
     * This function then draws a cross at the formal detection position, used
     * by the detection process. In addition, it also shows the surface of the
     * connected area detected using the detection position.
     *
     * As these information are coming from the detection process, they are
     * rendered in green.
     *
     * \param regbl_crop   Building map crop
     * \param regbl_dx     Building detection position
     * \param regbl_dy     Building detection position
     * \param regbl_size   Surface of the connected component, in squared pixels
     * \param regbl_detect Building detection flag for the considered map
     */

    void regbl_tracker_detect( cv::Mat & regbl_crop, double const regbl_dx, double const regbl_dy, double const regbl_size, bool const regbl_detect );

    /*! \brief Rendering methods
     * 
     * This function is responsible of creating the date indicator associated to
     * each crop of the available maps. The date indicator as the same width as
     * the crop itself.
     *
     * The background of the indicator is set to green if the building was
     * detected on the corresponding map, red otherwise. The date is written
     * centred in white.
     *
     * The computed indicator is returned as opencv matrix.
     *
     * \param regbl_width  Width, in pixels, of the cropped map
     * \param regbl_year   Yeap of the crop map
     * \param regbl_detect Building detection flag for the considered map
     *
     * \return Returns the computed indicator as opencv matrix.
     */

    cv::Mat regbl_tracker_timeline( int const regbl_width, std::string regbl_year, int const regbl_detect );

    /*! \brief Rendering methods
     * 
     * This function is used to create an indicator for each map crops to show
     * if the deduced construction date is above or below the year of the crop
     * map.
     *
     * In case the deduced building construction date is more recent that the
     * current crop map year, the indicator is simply a red image with same size
     * as the crop itself, green otherwise.
     *
     * This allows to show, on the overall timeline, the deduced lifespan of the
     * considered building.
     *
     * The computed indicator is returned as opencv matrix.
     *
     * \param regbl_width  Width, in pixels, of the cropped map
     * \param regbl_year   Yeap of the crop map
     * \param regbl_hbound Upper boundary of the deduced construction range
     *
     * \return Returns the computed indicator as opencv matrix.
     */

    cv::Mat regbl_tracker_detection( int const regbl_width, std::string regbl_year, std::string regbl_hbound );

    /*! \brief Rendering methods
     * 
     * This function creates a overall top indicator that indicates the EGID of
     * the considered building along with its deduced construction range and
     * the construction date found in the RegBL, when available.
     *
     * The information are written in white while the background is set to green
     * if the deduced construction range is consistent with the RegBL date, in
     * case it is available. In case it is not consistent, the background is set
     * to red.
     *
     * In case the construction date is not available in the RegBL, 'NO_REF' is
     * written and the background is set to grey.
     *
     * The computed indicator is returned as opencv matrix.
     *
     * \param regbl_width   Width, in pixels, of the cropped map
     * \param regbl_geid    Building EGID
     * \param regbl_year    RegBL construction date, when available
     * \param regbl_udeduce Deduced construction date upper range boundary
     * \param regbl_udeduce Deduced construction date lower range boundary
     *
     * \return Returns the computed indicator as opencv matrix.
     */

    cv::Mat regbl_tracker_reference( int const regbl_width, std::string regbl_geid, std::string regbl_year, std::string regbl_udeduce, std::string regbl_ldeduce );

    /*! \brief Main function
     *
     * This program is used to create a timeline focusing on one building and
     * uses maps to create a succession of crops centred around the building
     * showing its situation on each map (3D raster slices) :
     *
     *     ./regbl_tracker --storage/-s path of the main storage directory
     *                     --egid/-g building EGID
     *                     --export/-e Exportation directory path
     *
     * The main function starts by extracting crops, centred around the desired
     * building, from the pre-processed maps and the original one.
     *
     * In parallel, the program creates three indicator, on top, between the
     * pre-processed and original map crops succession and on bottom of the
     * computed timeline showing the EGID of the building, the deduced date
     * range of the construction, the construction date found in the RegBL
     * database (when available), the year of each crop and the deduced lifespan
     * of the considered building.
     *
     * On the crops themselves, the program shows the building centre, its
     * entries position (linked by a line to the position of the building) and
     * a circle representing the surface of the building. All these elements are
     * shown in pink, as extracted from the database.
     *
     * In addition, the formal position of the building detection, when
     * detected, and the surface of the connected pixel area of the building
     * footprint through a circle centred on the detection position are shown.
     * As these information are obtained through the detection process, they are
     * shown in green (when detected).
     *
     * The composed timeline image is then exported using the provided directory
     * path and uses the EGID of the building to name the image file. The PNG
     * format is considered to export the timeline.
     *
     * \param argc Standard parameter
     * \param argv Standard parameter
     *
     *  \return Exit code
     */

    int main( int argc, char ** argv );

/*
    header - inclusion guard
 */

    # endif

