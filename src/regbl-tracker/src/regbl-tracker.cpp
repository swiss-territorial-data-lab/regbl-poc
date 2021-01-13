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

    # include "regbl-tracker.hpp"

/*
    source - Processing methods
 */

    void regbl_tracker_crop( cv::Mat & regbl_crop, int const regbl_cnx, int const regbl_cny, std::string & regbl_date, int const regbl_detect ) {

        /* color value */
        cv::Scalar regbl_color;

        /* text shift value */
        int regbl_shift( ( regbl_crop.cols - ( REGBL_TRACKER_CROPHALF * 2 ) ) / 2 );

        /* check detection result */
        if ( regbl_detect == 0 ) {

            /* assign color */
            regbl_color = cv::Scalar( 78, 66, 192 );

        } else {

            /* assign color */
            regbl_color = cv::Scalar( 98, 142, 22 );

        }

        /* draw edge overlay */
        cv::rectangle( regbl_crop, cv::Rect( 0, 0, regbl_crop.cols, regbl_crop.rows ), regbl_color, 1 );

        /* draw text background */
        cv::rectangle( regbl_crop, cv::Rect( 0, 0, regbl_crop.cols, 18 ), regbl_color, cv::FILLED );

        /* display crop date */
        cv::putText( regbl_crop, regbl_date, cv::Point( 40 + regbl_shift, 14 ), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar( 255, 255, 255 ) );

        /* display central cross */
        cv::line( regbl_crop, cv::Point( regbl_cnx, regbl_cny + 2 ), cv::Point(regbl_cnx, regbl_cny + REGBL_TRACKER_CROSHALF ), regbl_color );
        cv::line( regbl_crop, cv::Point( regbl_cnx, regbl_cny - 2 ), cv::Point(regbl_cnx, regbl_cny - REGBL_TRACKER_CROSHALF ), regbl_color );
        cv::line( regbl_crop, cv::Point( regbl_cnx + 2, regbl_cny ), cv::Point(regbl_cnx + REGBL_TRACKER_CROSHALF, regbl_cny ), regbl_color );
        cv::line( regbl_crop, cv::Point( regbl_cnx - 2, regbl_cny ), cv::Point(regbl_cnx - REGBL_TRACKER_CROSHALF, regbl_cny ), regbl_color );

    }

    cv::Mat regbl_tracker_reference( int const regbl_width, std::string regbl_geid, std::string & regbl_truth ) {

        /* returned matrix */
        cv::Mat regbl_return( 18, regbl_width, CV_8UC3, cv::Scalar( 0, 0, 0 ) );

        /* display reference text */
        cv::putText( regbl_return, "GEID " + regbl_geid + " - DATE " + regbl_truth, cv::Point( 0, 14 ), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar( 255, 255, 255 ) );

        /* retrun matrix */
        return( regbl_return );

    }

/*
    source - Main function
 */

    int main( int argc, char ** argv ) {

        /* storage structure path */
        char * regbl_storage_path( lc_read_string( argc, argv, "--storage", "-s" ) );

        /* location name */
        char * regbl_location_name( lc_read_string( argc, argv, "--location", "-l" ) );

        /* building identifier (GEID) */
        char * regbl_building_id( lc_read_string( argc, argv, "--geid", "-g" ) );

        /* exportation directory */
        char * regbl_output_path( lc_read_string( argc, argv, "--export", "-e" ) );

        /* building database stream */
        std::ifstream regbl_building;

        /* building reference stream */
        std::ifstream regbl_reference;

        /* importation token */
        std::string regbl_date;
        std::string regbl_truth;

        /* importation token */
        int regbl_detect( -1 );

        /* importation token */
        float regbl_posx( 0 );
        float regbl_posy( 0 );

        /* cropping coordinates */
        int regbl_clx( 0 );
        int regbl_chx( 0 );
        int regbl_cly( 0 );
        int regbl_chy( 0 );

        /* crop center coordinates */
        int regbl_cnx( REGBL_TRACKER_CROPHALF );
        int regbl_cny( REGBL_TRACKER_CROPHALF );

        /* raster matrix */
        cv::Mat regbl_load;
        cv::Mat regbl_crop;

        /* combined matrix */
        cv::Mat regbl_aprc;
        cv::Mat regbl_aorg;
        cv::Mat regbl_aref;

        /* check consistency */
        if ( std::filesystem::is_directory( std::string( regbl_storage_path ) + "/regbl_frame/frame_" + std::string( regbl_location_name ) ) == false ) {

            /* display message */
            std::cerr << "error : could not locate the specified location processed maps directory" << std::endl;

            /* send message */
            return( 1 );

        }

        /* check consistency */
        if ( std::filesystem::is_directory( std::string( regbl_storage_path ) + "/regbl_frame/frame_" + std::string( regbl_location_name ) + "-original" ) == false ) {

            /* display message */
            std::cerr << "error : could not locate the specified location original maps directory" << std::endl;

            /* send message */
            return( 1 );

        }

        /* create reference stream */
        regbl_reference.open( std::string( regbl_storage_path ) + "/regbl_output/output_reference/reference_" + std::string( regbl_location_name ) + "/" + regbl_building_id, std::ios::in ); 

        /* check reference stream */
        if ( regbl_reference.is_open() == false ) {

            /* display message */
            std::cerr << "error : unable to access the building reference file in output directory" << std::endl;

            /* send message */
            return( 1 );

        }

        /* import reference construction date */
        regbl_reference >> regbl_truth;

        /* delete reference stream */
        regbl_reference.close();

        /* create building stream */
        regbl_building.open( std::string( regbl_storage_path ) + "/regbl_output/output_database/database_" + std::string( regbl_location_name ) + "/" + regbl_building_id, std::ios::in );

        /* check building stream */
        if ( regbl_building.is_open() == false ) {

            /* display message */
            std::cerr << "error : unable to access the building database file in output directory" << std::endl;

            /* send message */
            return( 1 );

        }

        /* parsing building detection database */
        while ( regbl_building >> regbl_date >> regbl_detect >> regbl_posx >> regbl_posy ) {

            /* import map */
            regbl_load = cv::imread( std::string( regbl_storage_path ) + "/regbl_frame/frame_" + std::string( regbl_location_name ) + "/" + std::string( regbl_location_name ) + "_" + regbl_date + ".tif", cv::IMREAD_COLOR );

            /* check image importation */
            if ( regbl_load.empty() == true ) {

                /* display message */
                std::cerr << "error : unable to load processed map : " << regbl_date << std::endl;

                /* send message */
                return( 1 );

            }

            /* invert y coordinate */
            regbl_posy = regbl_load.rows - regbl_posy - 1;

            /* compose cropping coordinates */
            regbl_clx = std::round( regbl_posx ) - REGBL_TRACKER_CROPHALF;
            regbl_chx = std::round( regbl_posx ) + REGBL_TRACKER_CROPHALF;
            regbl_cly = std::round( regbl_posy ) - REGBL_TRACKER_CROPHALF;
            regbl_chy = std::round( regbl_posy ) + REGBL_TRACKER_CROPHALF;

            /* assume center position */
            regbl_cnx = REGBL_TRACKER_CROPHALF;
            regbl_cny = REGBL_TRACKER_CROPHALF;

            /* edge detection */
            if ( regbl_clx < 0 ) {

                /* update center position */
                regbl_cnx += regbl_clx;

                /* update crop coordinate */
                regbl_clx = 0;

            }

            /* edge detection */
            if ( regbl_cly < 0 ) {

                /* update center position */
                regbl_cny += regbl_cly;

                /* update crop coordinate */
                regbl_cly = 0;

            }

            /* edge detection */
            if ( regbl_chx > regbl_load.cols ) {

                /* update crop coordinates */
                regbl_chx = regbl_load.cols;

            }

            /* edge detection */
            if ( regbl_chy > regbl_load.rows ) {

                /* update crop coordinates */
                regbl_chy = regbl_load.rows;

            }

            /* crop map */
            regbl_crop = regbl_load( cv::Rect( regbl_clx, regbl_cly, regbl_chx - regbl_clx, regbl_chy - regbl_cly ) );

            /* process crop */
            regbl_tracker_crop( regbl_crop, regbl_cnx, regbl_cny, regbl_date, regbl_detect );

            /* check accumulator state */
            if ( regbl_aprc.empty() == true ) {

                /* bootstrap accumulator */
                regbl_aprc = regbl_crop;

            } else {

                /* update accumulator */
                cv::hconcat( regbl_crop, regbl_aprc, regbl_aprc );

            }

            /* import map */
            regbl_load = cv::imread( std::string( regbl_storage_path ) + "/regbl_frame/frame_" + std::string( regbl_location_name ) + "-original/" + std::string( regbl_location_name ) + "_" + regbl_date + ".tif", cv::IMREAD_COLOR );

            /* check image importation */
            if ( regbl_load.empty() == true ) {

                /* display message */
                std::cerr << "error : unable to load original map : " << regbl_date << std::endl;

                /* send message */
                return( 1 );

            }

            /* crop map */
            regbl_crop = regbl_load( cv::Rect( regbl_clx, regbl_cly, regbl_chx - regbl_clx, regbl_chy - regbl_cly ) );

            /* process crop */
            regbl_tracker_crop( regbl_crop, regbl_cnx, regbl_cny, regbl_date, regbl_detect );

            /* check accumulator state */
            if ( regbl_aorg.empty() == true ) {

                /* bootstrap accumulator */
                regbl_aorg = regbl_crop;

            } else {

                /* update accumulator */
                cv::hconcat( regbl_crop, regbl_aorg, regbl_aorg );

            }

        }

        /* compose reference bar */
        regbl_aref = regbl_tracker_reference( regbl_aprc.cols, std::string( regbl_building_id ), regbl_truth );

        /* compose single representation */
        cv::vconcat( regbl_aprc, regbl_aorg, regbl_aprc );
        cv::vconcat( regbl_aprc, regbl_aref, regbl_aprc );

        /* export result */
        cv::imwrite( std::string( regbl_output_path ) + "/" + std::string( regbl_building_id ) + ".jpg", regbl_aprc );

        /* delete building stream */
        regbl_building.close();

        /* send message */
        return( 0 );

    }

