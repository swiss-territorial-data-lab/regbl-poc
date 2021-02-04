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

    # include "common-connect.hpp"

/*
    source - Connectivity methods
 */

    unsigned int lc_connect_get_size( cv::Mat & lc_image, cv::Mat & lc_mask, int const lc_x, int const lc_y, bool const lc_erase ) {

        /* compute connected area */
        lc_connect_t lc_area = lc_connect_get( lc_image, lc_mask, lc_x, lc_y, lc_erase );

        /* return connected part size */
        return( lc_area.size() );

    }

    lc_connect_t lc_connect_get( cv::Mat & lc_image, cv::Mat & lc_mask, int const lc_x, int const lc_y, bool const lc_erase ) {

        /* retruned structure */
        lc_connect_t lc_connect;

        /* array head */
        unsigned int lc_head( 0 );

        /* detection coordinates */
        int lc_u( 0 );
        int lc_v( 0 );

        /* connected neighbour */
        int lc_neigh[4][2] = { { +1, +0 }, { -1, +0 }, { +0, +1 }, { +0, -1 } };

        /* pushing first element */
        lc_connect.push_back( std::vector< int > ( 2 ) );

        /* assign first element */
        lc_connect.back()[0] = lc_x;
        lc_connect.back()[1] = lc_y;

        /* update mask */
        lc_mask.at<uchar>(lc_y, lc_x) = 255;

        /* follow connected pixels */
        while ( lc_head < lc_connect.size() ) {

            /* parsing neighbours */
            for ( unsigned int lc_i = 0; lc_i < 4; lc_i ++ ) {

                /* compose detection coordinates */
                lc_u = lc_connect[lc_head][0] + lc_neigh[lc_i][0];
                lc_v = lc_connect[lc_head][1] + lc_neigh[lc_i][1];

                /* check coordinates */
                if ( ( lc_u < 0 ) || ( lc_v < 0 ) ) continue;

                /* check coordinates */
                if ( ( lc_u >= lc_image.cols ) || ( lc_v >= lc_image.rows ) ) continue;

                /* check mask value */
                if ( lc_mask.at<uchar>( lc_v, lc_u ) == 0 ) {

                    /* check pixel value */
                    if ( lc_image.at<uchar>( lc_v, lc_u ) < 127.5 ) {

                        /* push connected element */
                        lc_connect.push_back( std::vector< int > ( 2 ) );

                        /* assign connected element */
                        lc_connect.back()[0] = lc_u;
                        lc_connect.back()[1] = lc_v;

                        /* update mask */
                        lc_mask.at<uchar>( lc_v, lc_u ) = 255;

                    }

                }

            }

            /* update head */
            lc_head ++;

        }

        /* check erase option */
        if ( lc_erase == true ) {

            /* parsing connected array */
            for ( unsigned int lc_i = 0; lc_i < lc_connect.size(); lc_i ++ ) {

                /* erase mask pixel */
                lc_mask.at<uchar>( lc_connect[lc_i][1], lc_connect[lc_i][0] ) = 0;

            }

        }

        /* return structure */
        return( lc_connect );

    }

