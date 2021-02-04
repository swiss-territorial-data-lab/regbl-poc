/*
 *  regbl - deduce
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

    /*! \file   regbl-deduce.hpp
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

    # ifndef __REGBL_DEDUCE__
    # define __REGBL_DEDUCE__

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

/*
    header - preprocessor definitions
 */

    /* define surface ratio threshold - @devs : magical parameter */
    # define REGBL_DEDUCE_RATIO ( 4.0 )

/*
    header - preprocessor macros
 */

    /* computation of ration always greater or equal to one */
    # define regbl_ratio(u,v) ( ( ( u ) > ( v ) ) ? ( u ) / ( v ) : ( v ) / ( u ) )

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
     * This function is responsible of analysis the detection file associated to
     * a specific building. Based on the content of the detection file, the
     * function deduce the most probable construction date of the building.
     *
     * The function starts by parsing the years on which the detection was made
     * and analysis the detection flag. It searches for the longest continuous
     * sequence of detection flags. As the last detection flag is seen, it uses
     * it as the upper range boundary of the construction date, the lower range
     * boundary being the map just before the last detection one.
     *
     * In addition, the function uses the surface of the connected area of the
     * building to detect change in the morphology of the building. If a jump of
     * the surface value between two detection, the function assumes that the
     * building was built just after the destruction of an older one.
     *
     * In case the building is not detected on the most recent map, it deduces
     * that the construction date is more recent than the most recent map. The
     * upper range boundary is then set to +32767.
     *
     * In case the building is detected on every map, without break, it deduces
     * the construction date to be more ancient than the oldest map. The lower
     * range boundary is set to -32768 in such a case.
     *
     * \param regbl_detect Building detection file path
     * \param regbl_deduce Deduction file exportation path
     */

    void regbl_deduce_detect( std::string regbl_detect, std::string regbl_deduce );

    /*! \brief Main function
     *
     * This program is used to perform the formal deduction of the building
     * construction date based on their detection file :
     *
     *     ./regbl-deduce --storage/-s path of the main storage directory
     *
     * The main function starts by listing all the available detection files for
     * the studied geographical area (covered by the 3D raster). For each file,
     * it applies the deduction process.
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

