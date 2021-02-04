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

    # include "common-list.hpp"

/*
    source - Importation methods
 */

    lc_list_t lc_list_import( std::string const lc_path ) {

        /* returned list */
        lc_list_t lc_return;

        /* importation buffer */
        std::vector < std::string > lc_buffer(7);

        /* create stream */
        std::ifstream lc_stream( lc_path, std::ifstream::in );

        /* check stream */
        if ( lc_stream.is_open() == false ) {

            /* return empty list */
            return( lc_return );

        }

        /* parsing list file */
        while ( lc_stream >> lc_buffer[0] >> lc_buffer[1] >> lc_buffer[2] >> lc_buffer[3] >> lc_buffer[4] >> lc_buffer[5] >> lc_buffer[6] ) {

            /* push buffer to list */
            lc_return.push_back( lc_buffer );

        }

        /* delete stream */
        lc_stream.close();

        /* return list */
        return( lc_return );

    }

/*
    source - Conversion methods
 */

    double lc_list_metric_to_pixel( lc_list_t & lc_list, int const lc_index ) {

        /* range values */
        double lc_georange( std::stod( lc_list[lc_index][2] ) - std::stod( lc_list[lc_index][1] ) );
        double lc_pixrange( std::stod( lc_list[lc_index][5] ) );

        /* compute and return factor */
        return( lc_pixrange / lc_georange );

    }

