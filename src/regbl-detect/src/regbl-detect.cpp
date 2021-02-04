/*
 *  regbl - detect
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

    # include "regbl-detect.hpp"

    /* temporary */
    cv::Mat regbl_mask;

/*
    source - Detection methods
 */

    bool regbl_detect_on_map( cv::Mat & regbl_map, int * const regbl_x, int * const regbl_y ) {

        /* detection cross pattern */
        static const int regbl_cross[29][2] = {

            { -3, +0 },
            { -2, -2 },
            { -2, -1 },
            { -2, +0 },
            { -2, +1 },
            { -2, +2 },
            { -1, -2 },
            { -1, -1 },
            { -1, +0 },
            { -1, +1 },
            { -1, +2 },
            { +0, -3 },
            { +0, -2 },
            { +0, -1 },
            { +0, +0 },
            { +0, +1 },
            { +0, +2 },
            { +0, +3 },
            { +1, -2 },
            { +1, -1 },
            { +1, +0 },
            { +1, +1 },
            { +1, +2 },
            { +2, -2 },
            { +2, -1 },
            { +2, +0 },
            { +2, +1 },
            { +2, +2 },
            { +3, +0 }

        };

        /* detection coordinates */
        int regbl_u( 0. );
        int regbl_v( 0. );
        
        /* parsing detection cross */
        for ( int regbl_i = 0; regbl_i < 29; regbl_i ++ ) {

            /* compute detection coordinates */
            regbl_u = ( * regbl_x ) + regbl_cross[regbl_i][0];
            regbl_v = ( * regbl_y ) + regbl_cross[regbl_i][1];

            /* check coordinates */
            if ( regbl_u < 0 ) continue;
            if ( regbl_v < 0 ) continue;

            /* check coordinates */
            if ( regbl_u >= regbl_map.cols ) continue;
            if ( regbl_v >= regbl_map.rows ) continue;

            /* formal detection */
            if ( regbl_map.at<uchar>( regbl_v, regbl_u ) == 0 ) {

                /* assign formal detection position */
                ( * regbl_x ) = regbl_u;
                ( * regbl_y ) = regbl_v;

                /* send results */
                return( true );

            }

        }

        /* send results */
        return( false );

    }

    void regbl_detect( cv::Mat & regbl_map, cv::Mat & regbl_track, std::string & regbl_export_egid, std::string & regbl_export_position, std::string & regbl_export_detect, std::string & regbl_year ) {

        /* input stream */
        std::ifstream regbl_input;

        /* output stream */
        std::ofstream regbl_output;

        /* egid value */
        std::string regbl_egid;

        /* position coordinates */
        int regbl_x( 0. );
        int regbl_y( 0. );

        /* detection statistic */
        int regbl_found( 0 );
        int regbl_total( 0 );

        /* cross color */
        cv::Scalar regbl_color;

        /* parsing egid directory */
        for ( const std::filesystem::directory_entry & regbl_file : std::filesystem::directory_iterator( regbl_export_egid ) ) {

            /* check for regular file */
            if ( regbl_file.is_regular_file() == true ) {

                /* extract egid */
                regbl_egid = regbl_file.path().filename();

                /* create input stream */
                regbl_input.open( regbl_export_position + "/" + regbl_year + "/" + regbl_egid, std::ifstream::in );

                /* check input stream */
                if ( regbl_input.is_open() == false ) {

                    /* display message */
                    std::cerr << "error : unable to access position file" << std::endl;

                    /* send message */
                    exit( 1 );

                }

                /* reset statistic */
                regbl_found = 0;
                regbl_total = 0;

                /* parsing position - need to bring back all position consideration - allows only single detection */
                if ( ( regbl_input >> regbl_x >> regbl_y ) && ( regbl_found == 0 ) ) {

                    /* detection on map */
                    if ( regbl_detect_on_map( regbl_map, & regbl_x, & regbl_y ) == true ) {

                        /* update statistic */
                        regbl_found ++;

                        /* update color */
                        regbl_color = cv::Scalar( 0, 255, 0, 255 );

                    } else {

                        /* update color */
                        regbl_color = cv::Scalar( 0, 0, 255, 255 );

                    }

                    /* mark detection on tacking map */
                    cv::line( regbl_track, cv::Point( regbl_x    , regbl_y - REGBL_DETECT_CROSS ), cv::Point( regbl_x    , regbl_y + REGBL_DETECT_CROSS ), regbl_color );
                    cv::line( regbl_track, cv::Point( regbl_x - REGBL_DETECT_CROSS, regbl_y     ), cv::Point( regbl_x + REGBL_DETECT_CROSS, regbl_y     ), regbl_color );

                    /* update total */
                    regbl_total ++;

                }

                /* check total consistency */
                if ( regbl_total == 0 ) {

                    /* display message */
                    std::cerr << "error : unable to import position from position file" << std::endl;

                    /* send message */
                    exit( 1 );

                } else {

                    /* experimental */
                    int regbl_size( lc_connect_get_size( regbl_map, regbl_mask, regbl_x, regbl_y, true ) );

                    /* create output stream */
                    regbl_output.open( regbl_export_detect + "/" + regbl_egid, std::ofstream::app );

                    /* check output stream */
                    if ( regbl_output.is_open() == false ) {

                        /* display message */
                        std::cerr << "error : unable to write in storage structure : detection file " << std::endl;

                        /* send message */
                        exit( 1 );

                    }

                    /* export detection result */
                    regbl_output << regbl_year << ( ( regbl_found > 0 ) ? " 1 " : " 0 " ) << regbl_x << " " << regbl_y << " " << regbl_size << std::endl;

                    /* delete output stream */
                    regbl_output.close();

                }

                /* delete input stream */
                regbl_input.close();

            }

        }

    }

/*
    source - Main function
 */

    int main( int argc, char ** argv ) {

        /* storage structure path */
        char * regbl_storage_path( lc_read_string( argc, argv, "--storage", "-s" ) );

        /* path composition */
        std::string regbl_export_egid;
        std::string regbl_export_position;
        std::string regbl_export_detect;
        std::string regbl_export_frame;

        /* raster image */
        cv::Mat regbl_map;
        cv::Mat regbl_track;

        /* storage list */
        lc_list_t regbl_list;

        /* check path specification */
        if ( regbl_storage_path == NULL ) {

            /* displays message */
            std::cerr << "error : storage path specification" << std::endl;

            /* send message */
            return( 1 );

        }

        /* import storage list */
        regbl_list = lc_list_import( std::string( regbl_storage_path ) + "/regbl_list" );

        /* check storage list */
        if ( regbl_list.size() == 0 ) {

            /* display message */
            std::cerr << "error : unable to import storage list file" << std::endl;

            /* send message */
            return( 1 );

        }

        /* compose path */
        regbl_export_egid = std::string( regbl_storage_path ) + "/regbl_output/output_egid";

        /* check consistency */
        if ( std::filesystem::is_directory( regbl_export_egid ) == false ) {

            /* display message */
            std::cerr << "error : unable to locate egid directory" << std::endl;

            /* send message */
            return( 1 );

        }

        /* compose path */
        regbl_export_position = std::string( regbl_storage_path ) + "/regbl_output/output_position";

        /* check consistency */
        if ( std::filesystem::is_directory( regbl_export_position ) == false ) {

            /* display message */
            std::cerr << "error : unable to locate position directory" << std::endl;

            /* send message */
            return( 1 );

        }

        /* compose path */
        regbl_export_detect = std::string( regbl_storage_path ) + "/regbl_output/output_detect";

        /* check consistency */
        if ( std::filesystem::is_directory( regbl_export_detect ) == false ) {

            /* create directory */
            std::filesystem::create_directories( regbl_export_detect );

        } else {

            /* display message */
            std::cerr << "error : detect directory already processed" << std::endl;

            /* send message */
            return( 1 );

        }

        /* compose path */
        regbl_export_frame = std::string( regbl_storage_path ) + "/regbl_output/output_frame";

        /* check consistency */
        if ( std::filesystem::is_directory( regbl_export_frame ) == false ) {

            /* create directory */
            std::filesystem::create_directories( regbl_export_frame );

        } else {

            /* display message */
            std::cerr << "error : frame directory already processed" << std::endl;

            /* send message */
            return( 1 );

        }

        /* parsing storage list */
        for ( unsigned int regbl_parse = 0; regbl_parse < regbl_list.size(); regbl_parse ++ ) {

            /* display information */
            std::cout << "Processing year " << regbl_list[regbl_parse][0] << " ..." << std::endl;

            /* import year map */
            regbl_map = cv::imread( std::string( regbl_storage_path ) + "/regbl_frame/frame/" + regbl_list[regbl_parse][0] + ".tif", cv::IMREAD_GRAYSCALE );

            /* check year map importation */
            if ( regbl_map.empty() == true ) {

                /* display message */
                std::cerr << "error : unable to import map for year " << regbl_list[regbl_parse][0] << std::endl;

                /* send message */
                return( 1 );

            } else {

                /* check map and list consistency */
                if ( ( std::stoi( regbl_list[regbl_parse][5] ) != regbl_map.cols ) || ( std::stoi( regbl_list[regbl_parse][6] ) != regbl_map.rows ) ) {

                    /* display message */
                    std::cerr << "error : inconsistency between map size and storage list size" << std::endl;

                    /* send message */
                    return( 1 );

                }

                /* experimental */
                regbl_mask = cv::Mat( regbl_map.rows, regbl_map.cols, CV_8UC1, cv::Scalar( 0 ) );

                /* invert map y-axis - fit northing coordinates direction */
                cv::flip( regbl_map, regbl_map, 0 );

                /* create detection tracking map */
                regbl_track = cv::Mat::zeros( cv::Size( regbl_map.cols, regbl_map.rows ), CV_8UC4 );

                /* perform detection on the map */
                regbl_detect( regbl_map, regbl_track, regbl_export_egid, regbl_export_position, regbl_export_detect, regbl_list[regbl_parse][0] );

                /* invert tracking map y-axis */
                cv::flip( regbl_track, regbl_track, 0 );

                /* export map with detections */
                cv::imwrite( std::string( regbl_storage_path ) + "/regbl_output/output_frame/" + regbl_list[regbl_parse][0] + ".tif", regbl_track );

            }

        }

        /* send message */
        return( 0 );

    }

