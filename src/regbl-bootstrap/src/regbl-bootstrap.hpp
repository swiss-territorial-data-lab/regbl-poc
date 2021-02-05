/*
 *  regbl - bootstrap
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

    /*! \file   regbl-bootstrap.hpp
     *  \author Nils Hamel <nils.hamel@alumni.epfl.ch>
     *  \author Huriel Reichel
     *
     *  regbl-bootstrap
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

    # ifndef __REGBL_BOOTSTRAP__
    # define __REGBL_BOOTSTRAP__

/*
    header - internal includes
 */

/*
    header - external includes
 */

    # include <iostream>    
    # include <string>
    # include <vector>
    # include <fstream>
    # include <filesystem>
    # include <common-include.hpp>

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

    typedef std::vector< std::vector< std::string > > regbl_listbase;

/*
    header - structures
 */

/*
    header - function prototypes
 */

    /*! \brief Extraction methods
     *
     * This function is responsible of extracting the information of building in
     * the RegBL database provided through its path. The function reads the
     * database entries and filter out the building that are not on the studied
     * area. The filtering of the building based on their position is made using
     * the 3D raster descriptor of the studied area.
     *
     * For each building, the position (EPSG:2056, E, N), the EGID, the surface
     * (in square meters) and the construction date (when available) are
     * extracted. The different information are dispatched and stored to ease
     * accesses by subsequent processes.
     *
     * Both surface value and construction date can be missing for a building.
     * In such a case, an empty string is exported in the storage file to inform
     * the lack of information.
     *
     * \param regbl_GEB_path         Path of the RegBL GEB DSV file
     * \param regbl_export_egid      Exportation path for EGID files
     * \param regbl_export_position  Exportation path for positions
     * \param regbl_export_reference Exportation path for construction date
     * \param regbl_export_surface   Exportation path for surface values
     * \param regbl_list             3D raster descriptor
     */

    void regbl_bootstrap_extract( std::string regbl_GEB_path, std::string regbl_export_egid, std::string regbl_export_position, std::string regbl_export_reference, std::string regbl_export_surface, lc_list_t & regbl_list );

    /*! \brief Extraction methods
     *
     * This function is responsible of extracting the position of the building
     * entries, when available, and to link them to their respective building
     * using the EGID as link. A geographical filtering is performed to only
     * consider entries that appear on the 3D raster. This is done using the 3D
     * raster descriptor.
     *
     * This process parses then the RegBL EIN database to extract the entries   
     * position, taking into account that a building can have no, one or more
     * entries. The EGID is used to link them to the extraction of building
     * position.
     *
     * It follows that this function will complete the position files computed
     * by the function regbl_bootstrap_extract(), by appending the position of
     * the entries to the already computed position file.
     *
     * \param regbl_EIN_path         Path of the RegBL EIN DSV file
     * \param regbl_export_position  Exportation path for positions
     * \param regbl_list             3D raster descriptor
     */

    void regbl_bootstrap_entries( std::string regbl_EIN_path, std::string regbl_export_egid, std::string regbl_export_position, lc_list_t & regbl_list );

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
     * This program is used to bootstrap the processing storage structure of a
     * 3D raster processing for building construction deduction :
     *
     *     ./regbl-bootstrap --storage/-s Main storage path
     *                       --geb/-g RegBL GEB DSV file path
     *                       --ein/-e RegBL EIN DSV file path
     *
     * The first parameter gives the main storage path, that is the directory in
     * with all the processing steps data will be exported and gathered.
     *
     * The two path provided to the program have to point to the GEB and EIN DSV
     * file of the RegBL database. Be sure to specify the 'Data' one. The EIN
     * database can be omitted. In such a case, the entries of the building are
     * not extracted, and then, will not be used by any of the subsequent
     * processes.
     *
     * The program then initialises the main storage directory by creating all
     * the required sub-directories. It then starts reading the GEB database
     * entries to extract each building position, EGID, construction date and
     * surface values.
     *
     * This done, the program reads the EIN database to extract the available
     * building entries and links them to their respective building.
     *
     * A geographical filtering is performed base on the building position and
     * on the entries position themselves in order to keep only the building
     * that appear on each 2D slice of the 3D raster. This program then needs
     * the 3D raster descriptor to be available in the main storage path.
     *
     * \param argc Standard parameter
     * \param argv Standard parameter
     *
     * \return Exit code.
     */

    int main( int argc, char ** argv );

/*
    header - inclusion guard
 */

    # endif

