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

    /*! ... */

    void regbl_bootstrap_extract( std::string regbl_GEB_path, std::string regbl_export_egid, std::string regbl_export_position, std::string regbl_export_reference, std::string regbl_export_surface, lc_list & regbl_list );

    /*! ... */

    void regbl_bootstrap_entries( std::string regbl_EIN_path, std::string regbl_export_egid, std::string regbl_export_position, lc_list & regbl_list );

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

    /*! ... */

    int main( int argc, char ** argv );

/*
    header - inclusion guard
 */

    # endif

