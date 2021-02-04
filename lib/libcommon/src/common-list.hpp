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

    /*! \file   common-list.hpp
     *  \author Nils Hamel <nils.hamel@alumni.epfl.ch>
     *  \author Huriel Reichel
     *
     *  regbl - common library - list
     */

/*
    header - inclusion guard
 */

    # ifndef __LC_LIST__
    # define __LC_LIST__

/*
    header - internal includes
 */

/*
    header - external includes
 */

    # include <iostream>
    # include <fstream>
    # include <string>
    # include <vector>

/*
    header - preprocessor definitions
 */

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

    typedef std::vector < std::vector< std::string > > lc_list_t;

/*
    header - structures
 */

/*
    header - function prototypes
 */

    /*! \brief Importation methods
     *
     * This function allows to import the information contained in the main
     * storage directory 3D raster descriptor file. Such file is used to store
     * the information needed to perform the building construction deduction
     * process.
     *
     * The descriptor file is a simple file containing lines which describes the
     * year, geographical boundaries and map pixel size of each slice of the
     * 3D raster.
     *
     * This function allows to simply import the content of the descriptor in a
     * vector of string vector, each line containing a slice description.
     *
     * \param lc_path Path of the 3D raster descriptor file
     *
     * \return Returns a vector of string vector containing the content of the
     * descriptor file in a n by 7 matrix, n being the amount of slices.
     */

    lc_list_t lc_list_import( std::string const lc_path );

    /*! \brief Conversion methods
     *
     * This function allows to compute the conversion factor to apply on metric
     * value to obtain their pixel counterpart on the map designated by a line
     * of the provdied 3D raster descriptor string matrix.
     *
     * \param lc_list  3D raster descriptor string matrix
     * \param lc_index Index of the slide in the 3D raster descriptor
     *
     * \return Returns the metric to pixel conversion factor of the designated
     * 3D raster slice. 
     */

    double lc_list_metric_to_pixel( lc_list_t & lc_list, int const lc_index );

/*
    header - inclusion guard
 */

    # endif

