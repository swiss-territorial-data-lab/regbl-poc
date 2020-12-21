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

/*
    source - processing methods
 */

    void regbl_detect( char const * const regbl_database, char const * const regbl_storage, cv::Mat & regbl_map, std::string & regbl_location, std::string & regbl_year, double const regbl_xmin, double const regbl_xmax, double const regbl_ymin, double const regbl_ymax ) {

        /* reading buffers */
        char regbl_head[REGBL_BUFFER] = { 0 };
        char regbl_line[REGBL_BUFFER] = { 0 };

        /* detection index */
        int regbl_EGID ( 0 );
        int regbl_GKODE( 0 );
        int regbl_GKODN( 0 );
        int regbl_GBAUJ( 0 );

        /* reading token */
        char regbl_token[REGBL_BUFFER] = { 0 };

        /* coordinates variable */
        double regbl_x( 0. );
        double regbl_y( 0. );

        /* detection flag */
        int regbl_flag( 0 );

        /* exportation stream */
        std::ofstream regbl_export;

        /* database stream */
        std::ifstream regbl_stream( regbl_database, std::ifstream::in );

        /* check stream */
        if ( regbl_stream.is_open() == false ) {

            /* display message */
            std::cerr << "error : unable to open database" << std::endl;

            /* abort */
            exit( 1 );

        }

        /* import database header */
        regbl_stream.getline( regbl_head, REGBL_BUFFER );

        /* detect database entries */
        regbl_EGID  = regbl_detect_database_header( regbl_head, "EGID"  );
        regbl_GKODE = regbl_detect_database_header( regbl_head, "GKODE" );
        regbl_GKODN = regbl_detect_database_header( regbl_head, "GKODN" );
        regbl_GBAUJ = regbl_detect_database_header( regbl_head, "GBAUJ" );

        /* check entry */
        if ( regbl_EGID < 0 ) {

            /* display message */
            std::cerr << "error : unable to locate EGID in database" << std::endl;

            /* abort */
            exit( 1 );

        }

        /* check entry */
        if ( regbl_GKODE < 0 ) {

            /* display message */
            std::cerr << "error : unable to locate GKODE in database" << std::endl;

            /* abort */
            exit( 1 );

        }

        /* check entry */
        if ( regbl_GKODN < 0 ) {

            /* display message */
            std::cerr << "error : unable to locate GKODN in database" << std::endl;

            /* abort */
            exit( 1 );

        }

        /* check entry */
        if ( regbl_GBAUJ < 0 ) {

            /* display message */
            std::cerr << "error : unable to locate GBAUJ in database" << std::endl;

            /* abort */
            exit( 1 );

        }

        /* parsing database entries */
        while ( regbl_stream.getline( regbl_line, REGBL_BUFFER ) ) {

            /* read coordinates token */
            regbl_detect_database_entry( regbl_line, regbl_GKODE, regbl_token );

            /* convert token */
            regbl_x = std::atof( regbl_token );

            /* read coordinates token */
            regbl_detect_database_entry( regbl_line, regbl_GKODN, regbl_token );

            /* convert token */
            regbl_y = std::atof( regbl_token );

            /* check coordinates boundaries */
            if ( ( regbl_x >= regbl_xmin ) && ( regbl_x < regbl_xmax ) && ( regbl_y >= regbl_ymin ) && ( regbl_y < regbl_ymax ) ) {

                /* convert geographical coordinates to map pixel coordinates */
                regbl_x = ( ( regbl_x - regbl_xmin ) / ( regbl_xmax - regbl_xmin ) ) * regbl_map.rows;
                regbl_y = ( ( regbl_y - regbl_ymin ) / ( regbl_ymax - regbl_ymin ) ) * regbl_map.cols;

                /* detect on map */
                if ( regbl_map.at<uchar>( regbl_y, regbl_x ) > 127 ) {

                    /* update flag */
                    regbl_flag = 1;

                    /* mark on map */
                    regbl_map.at<uchar>( regbl_y, regbl_x ) = 192;

                } else {

                    /* update flag */
                    regbl_flag = 0;

                    /* mark on map */
                    regbl_map.at<uchar>( regbl_y, regbl_x ) = 64;

                }

                /* read coordinates token */
                regbl_detect_database_entry( regbl_line, regbl_EGID, regbl_token );

                /* create exportation stream */
                regbl_export.open( std::string( regbl_storage ) + "/regbl_output/output_database/database_" + regbl_location + "/" + regbl_location + "_" + std::string( regbl_token ), std::ofstream::out | std::ofstream::app );

                /* check stream */
                if ( regbl_export.is_open() == false ) {

                    /* display message */
                    std::cerr << "error : unable to access database output stream" << std::endl;

                    /* abort */
                    exit( 1 );

                }

                /* export dates */
                regbl_export << regbl_year << " " << regbl_flag << std::endl;

                /* delete stream */
                regbl_export.close();

                /* create exportation stream */
                regbl_export.open( std::string( regbl_storage ) + "/regbl_output/output_reference/reference_" + regbl_location + "/" + regbl_location + "_" + std::string( regbl_token ), std::ofstream::out );

                /* check stream */
                if ( regbl_export.is_open() == false ) {

                    /* display message */
                    std::cerr << "error : unable to access database output stream" << std::endl;

                    /* abort */
                    exit( 1 );

                }

                /* read coordinates token */
                regbl_detect_database_entry( regbl_line, regbl_GBAUJ, regbl_token );
                
                /* export dates */
                regbl_export << regbl_year << " " << regbl_token << std::endl;

                /* delete stream */
                regbl_export.close();

            }

        }

        /* delete input stream */
        regbl_stream.close(); 

    }

/*
    source - database methods
 */

    int regbl_detect_database_header( char const * const regbl_line, char const * const regbl_target ) {

        /* parsing flag */
        bool regbl_parse( true );

        /* segmentation index */
        int regbl_tail( 0 );
        int regbl_head( 0 );

        /* token variable */
        char regbl_token[REGBL_BUFFER] = { 0 };

        /* detection index */
        int regbl_index( 0 );

        /* parsing characters */
        while ( regbl_parse == true ) {

            /* delimiter detection */
            if ( ( regbl_line[regbl_head] == '\t' ) || ( regbl_line[regbl_head] == '\0' ) ) {

                /* extract token */
                memcpy( regbl_token, regbl_line + regbl_tail, regbl_head - regbl_tail );

                /* adding termination char */
                regbl_token[regbl_head - regbl_tail] = '\0';

                /* target detection */
                if ( strcmp( regbl_token, regbl_target ) == 0 ) {

                    /* return index */
                    return( regbl_index );

                } else {

                    /* update index */
                    regbl_index ++;

                    /* push new tail */
                    regbl_tail = regbl_head + 1;

                    /* detect end of string */
                    if ( regbl_line[regbl_head] == '\0' ) {

                        /* update parser flag */
                        regbl_parse = false;

                    }

                }

            }

            /* update head */
            regbl_head ++;

        }

        /* send not found code */
        return( -1 );

    }

    void regbl_detect_database_entry( char const * const regbl_line, int const regbl_target, char * const regbl_token ) {

        /* parsing flag */
        bool regbl_parse( true );

        /* segmentation index */
        int regbl_tail( 0 );
        int regbl_head( 0 );

        /* detection index */
        int regbl_index( 0 );

        /* parsing characters */
        while ( regbl_parse == true ) {

            /* delimiter detection */
            if ( ( regbl_line[regbl_head] == '\t' ) || ( regbl_line[regbl_head] == '\0' ) ) {

                /* check index */
                if ( ( regbl_index ++ ) == regbl_target ) {

                    /* extract entity */
                    memcpy( regbl_token, regbl_line + regbl_tail, regbl_head - regbl_tail );

                    /* adding termination char */
                    regbl_token[regbl_head - regbl_tail] = '\0';

                    /* abort search */
                    return;

                }

                /* push new tail */
                regbl_tail = regbl_head + 1;

                /* detect end of string */
                if ( regbl_line[regbl_head] == '\0' ) {

                    /* update parser flag */
                    regbl_parse = false;

                }

            }

            /* update head */
            regbl_head ++;

        }

    }

/*
    source - main function
 */

    int main( int argc, char ** argv ) {

        /* storage structure path */
        char * regbl_storage_path( lc_read_string( argc, argv, "--storage", "-s" ) );

        /* database input */
        char * regbl_database_path( lc_read_string( argc, argv, "--database", "-d" ) );

        /* storage list stream */
        std::ifstream regbl_list;

        /* list token */
        std::string regbl_location;
        std::string regbl_year;
        std::string regbl_xmin;
        std::string regbl_xmax;
        std::string regbl_ymin;
        std::string regbl_ymax;

        /* raster image */
        cv::Mat regbl_map;

        /* check path specification */
        if ( regbl_storage_path == NULL ) {

            /* display message */
            std::cerr << "error : storage path specification" << std::endl;

            /* abort */
            return( 1 );

        }

        /* check path specification */
        if ( regbl_database_path == NULL ) {

            /* display message */
            std::cerr << "error : database path specification" << std::endl;

            /* abort */
            return( 1 );

        }

        /* create list stream */
        regbl_list.open( regbl_storage_path + std::string( "/regbl_list" ), std::ifstream::in );

        /* check list stream */
        if ( regbl_list.is_open() == false ) {

            /* display message */
            std::cerr << "error : unable to access storage list" << std::endl;

            /* abort */
            return( 1 );

        }

        /* create directory */
        std::filesystem::create_directories( std::string( regbl_storage_path ) + "/regbl_output/output_frame" );

        /* parsing list */
        while ( regbl_list >> regbl_location ) {

            /* import subsequent tokens */
            if ( regbl_list >> regbl_year >> regbl_xmin >> regbl_xmax >> regbl_ymin >> regbl_ymax ) {

                std::cerr << regbl_location << " "
                          << regbl_year << " " 
                          << regbl_xmin << " " 
                          << regbl_xmax << " " 
                          << regbl_ymin << " " 
                          << regbl_ymax << std::endl;

                /* import image */
                regbl_map = cv::imread( regbl_storage_path + std::string("/regbl_frame/frame_") + regbl_location + std::string("/") + regbl_location + std::string("_") + regbl_year + std::string(".tif"), cv::IMREAD_GRAYSCALE );

                /* invert map y-axis */
                cv::flip( regbl_map, regbl_map, 0 );

                /* check image importation */
                if ( regbl_map.empty() == false ) {

                    /* create directory */
                    std::filesystem::create_directories( std::string( regbl_storage_path ) + "/regbl_output/output_frame/frame_" + regbl_location );

                    /* create directory */
                    std::filesystem::create_directories( std::string( regbl_storage_path ) + "/regbl_output/output_database/database_" + regbl_location );

                    /* create directory */
                    std::filesystem::create_directories( std::string( regbl_storage_path ) + "/regbl_output/output_reference/reference_" + regbl_location );

                    /* processing database on current map */
                    regbl_detect( regbl_database_path, regbl_storage_path, regbl_map, regbl_location, regbl_year, std::stod( regbl_xmin, NULL ), std::stod( regbl_xmax, NULL ), std::stod( regbl_ymin, NULL ), std::stod( regbl_ymax, NULL ) );

                    /* restore map y-axis */
                    cv::flip( regbl_map, regbl_map, 0 );

                    /* export map with detection marks */
                    cv::imwrite( std::string( regbl_storage_path ) + "/regbl_output/output_frame/frame_" + regbl_location + "/" + regbl_location + "_" + regbl_year + ".tif", regbl_map );

                } else {

                    /* display message */
                    std::cerr << "error : unable to access map" << std::endl;

                    /* abort */
                    return( 1 );

                }

            }

        }

        /* delete list stream */
        regbl_list.close();

        /* send message */
        return( 0 );

    }

