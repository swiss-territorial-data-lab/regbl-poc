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
    source - Detection methods
 */

    bool regbl_detect_on_map( cv::Mat & regbl_map, double const regbl_x, double const regbl_y, double const regbl_size ) {

        /* detection cross pattern */
        static const int regbl_cross[5][2] = {

            { -1, +0 },
            { +1, +0 },
            { +0, +0 },
            { +0, -1 },
            { +0, +1 }

        };

        /* detection coordinates */
        double regbl_u( 0. );
        double regbl_v( 0. );
        
        /* parsing detection cross */
        for ( int regbl_i = 0; regbl_i < 5; regbl_i ++ ) {

            /* compute detection coordinates */
            regbl_u = regbl_x + regbl_cross[regbl_i][0] * regbl_size;
            regbl_v = regbl_y + regbl_cross[regbl_i][1] * regbl_size;

            /* check coordinates */
            if ( regbl_u < 0 ) continue;
            if ( regbl_v < 0 ) continue;

            /* check coordinates */
            if ( regbl_u >= regbl_map.cols ) continue;
            if ( regbl_v >= regbl_map.rows ) continue;

            /* formal detection */
            if ( regbl_map.at<uchar>( regbl_v, regbl_u ) < 127.5 ) {

                /* send results */
                return( true );

            }

        }

        /* send results */
        return( false );

    }

/*
    source - Processing methods
 */

    void regbl_detect( char const * const regbl_database, cv::Mat & regbl_map, cv::Mat & regbl_track, std::string & regbl_export, std::string & regbl_year, double const regbl_xmin, double const regbl_xmax, double const regbl_ymin, double const regbl_ymax ) {

        /* reading buffers */
        char regbl_head[REGBL_BUFFER] = { 0 };
        char regbl_line[REGBL_BUFFER] = { 0 };

        /* reading token */
        char regbl_token[REGBL_BUFFER] = { 0 };

        /* detection index */
        int regbl_EGID ( 0 );
        int regbl_GKODE( 0 );
        int regbl_GKODN( 0 );
        int regbl_GBAUJ( 0 );

        /* coordinates variable */
        double regbl_x( 0. );
        double regbl_y( 0. );

        /* exportation stream */
        std::ofstream regbl_output;

        /* create database stream */
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

        /* detect and check entry */
        if ( ( regbl_EGID  = regbl_detect_database_header( regbl_head, "EGID"  ) ) < 0 ) {

            /* display message */
            std::cerr << "error : unable to locate EGID in database" << std::endl;

            /* abort */
            exit( 1 );

        }

        /* detect and check entry */
        if ( ( regbl_GKODE = regbl_detect_database_header( regbl_head, "GKODE" ) ) < 0 ) {

            /* display message */
            std::cerr << "error : unable to locate GKODE in database" << std::endl;

            /* abort */
            exit( 1 );

        }

        /* detect and check entry */
        if ( ( regbl_GKODN = regbl_detect_database_header( regbl_head, "GKODN" ) ) < 0 ) {

            /* display message */
            std::cerr << "error : unable to locate GKODN in database" << std::endl;

            /* abort */
            exit( 1 );

        }

        /* detect and check entry */
        if ( ( regbl_GBAUJ = regbl_detect_database_header( regbl_head, "GBAUJ" ) ) < 0 ) {

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

                /* read EGID token */
                regbl_detect_database_entry( regbl_line, regbl_EGID, regbl_token );

                /* create output stream */
                regbl_output.open( regbl_export + "/" + std::string( regbl_token ), std::ofstream::out | std::ofstream::app );

                /* check stream */
                if ( regbl_output.is_open() == false ) {

                    /* display message */
                    std::cerr << "error : unable to write reference file" << std::endl;

                    /* abort */
                    exit( 1 );

                }

                /* detect on map */
                if ( regbl_detect_on_map( regbl_map, regbl_x, regbl_y, 3 ) == true ) {

                    /* export detection result */
                    regbl_output << regbl_year << " 1 " << regbl_x << " " << regbl_y << std::endl;

                    /* update traking map */
                    cv::line( regbl_track, cv::Point( regbl_x    , regbl_y - 3 ), cv::Point( regbl_x    , regbl_y + 3 ), cv::Scalar( 0, 255, 0, 255 ) );
                    cv::line( regbl_track, cv::Point( regbl_x - 3, regbl_y     ), cv::Point( regbl_x + 3, regbl_y     ), cv::Scalar( 0, 255, 0, 255 ) );

                } else {

                    /* export detection result */
                    regbl_output << regbl_year << " 0 " << regbl_x << " " << regbl_y <<  std::endl;

                    /* update traking map */
                    cv::line( regbl_track, cv::Point( regbl_x    , regbl_y - 3 ), cv::Point( regbl_x    , regbl_y + 3 ), cv::Scalar( 0, 0, 255, 255 ) );
                    cv::line( regbl_track, cv::Point( regbl_x - 3, regbl_y     ), cv::Point( regbl_x + 3, regbl_y     ), cv::Scalar( 0, 0, 255, 255 ) );

                }

                /* delete output stream */
                regbl_output.close();

            }

        }

        /* delete database stream */
        regbl_stream.close(); 

    }

    void regbl_detect_reference( char const * const regbl_database, std::string & regbl_export, double const regbl_xmin, double const regbl_xmax, double const regbl_ymin, double const regbl_ymax ) {

        /* reading buffers */
        char regbl_head[REGBL_BUFFER] = { 0 };
        char regbl_line[REGBL_BUFFER] = { 0 };

        /* reading token */
        char regbl_token[REGBL_BUFFER] = { 0 };

        /* detection index */
        int regbl_EGID ( 0 );
        int regbl_GKODE( 0 );
        int regbl_GKODN( 0 );
        int regbl_GBAUJ( 0 );

        /* coordinates variable */
        double regbl_x( 0. );
        double regbl_y( 0. );

        /* exportation stream */
        std::ofstream regbl_output;

        /* create database stream */
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

        /* detect and check entry */
        if ( ( regbl_EGID  = regbl_detect_database_header( regbl_head, "EGID"  ) ) < 0 ) {

            /* display message */
            std::cerr << "error : unable to locate EGID in database" << std::endl;

            /* abort */
            exit( 1 );

        }

        /* detect and check entry */
        if ( ( regbl_GKODE = regbl_detect_database_header( regbl_head, "GKODE" ) ) < 0 ) {

            /* display message */
            std::cerr << "error : unable to locate GKODE in database" << std::endl;

            /* abort */
            exit( 1 );

        }

        /* detect and check entry */
        if ( ( regbl_GKODN = regbl_detect_database_header( regbl_head, "GKODN" ) ) < 0 ) {

            /* display message */
            std::cerr << "error : unable to locate GKODN in database" << std::endl;

            /* abort */
            exit( 1 );

        }

        /* detect and check entry */
        if ( ( regbl_GBAUJ = regbl_detect_database_header( regbl_head, "GBAUJ" ) ) < 0 ) {

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

                /* read EGID token */
                regbl_detect_database_entry( regbl_line, regbl_EGID, regbl_token );

                /* create output stream */
                regbl_output.open( regbl_export + "/" + std::string( regbl_token ), std::ofstream::out );

                /* check stream */
                if ( regbl_output.is_open() == false ) {

                    /* display message */
                    std::cerr << "error : unable to write reference file" << std::endl;

                    /* abort */
                    exit( 1 );

                }

                /* read GBAUJ token */
                regbl_detect_database_entry( regbl_line, regbl_GBAUJ, regbl_token );                

                /* export reference */
                regbl_output << ( ( std::strlen( regbl_token ) > 0 ) ? regbl_token : "-1" ) << std::endl;

                /* delete output stream */
                regbl_output.close();

            }

        }

        /* delete database stream */
        regbl_stream.close(); 

    }

/*
    source - Database methods
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
            if ( ( regbl_line[regbl_head] == REGBL_DELIMITER ) || ( regbl_line[regbl_head] == '\0' ) ) {

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
            if ( ( regbl_line[regbl_head] == REGBL_DELIMITER ) || ( regbl_line[regbl_head] == '\0' ) ) {

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
    source - Main function
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
        cv::Mat regbl_track;

        /* path variable */
        std::string regbl_export;
        std::string regbl_reference;
        std::string regbl_frame;
        std::string regbl_database;

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

        /* create path */
        regbl_export = std::string( regbl_storage_path ) + "/regbl_output";

        /* parsing list */
        while ( regbl_list >> regbl_location ) {

            /* import subsequent list tokens */
            if ( regbl_list >> regbl_year >> regbl_xmin >> regbl_xmax >> regbl_ymin >> regbl_ymax ) {

                /* display information */
                std::cout << "Processing : " << regbl_location << " in " << regbl_year << std::endl;

                /* import image */
                regbl_map = cv::imread( regbl_storage_path + std::string("/regbl_frame/frame_") + regbl_location + std::string("/") + regbl_location + std::string("_") + regbl_year + std::string(".tif"), cv::IMREAD_GRAYSCALE );

                /* check image importation */
                if ( regbl_map.empty() == false ) {

                    /* invert map y-axis */
                    cv::flip( regbl_map, regbl_map, 0 );

                    /* create detection tracking map */
                    regbl_track = cv::Mat::zeros( cv::Size( regbl_map.cols, regbl_map.rows ), CV_8UC4 );

                    /* create path */
                    regbl_reference = regbl_export + "/output_reference/reference_" + regbl_location;

                    /* check directory */
                    if ( std::filesystem::is_directory( regbl_reference ) == false ) {

                        /* create directory */
                        std::filesystem::create_directories( regbl_reference );

                        /* process reference */
                        regbl_detect_reference( regbl_database_path, regbl_reference, std::stod( regbl_xmin, NULL ), std::stod( regbl_xmax, NULL ), std::stod( regbl_ymin, NULL ), std::stod( regbl_ymax, NULL ) );

                    }

                    /* create path */
                    regbl_database = regbl_export + "/output_database/database_" + regbl_location;

                    /* check directory */
                    if ( std::filesystem::is_directory( regbl_database ) == false ) {

                        /* create directory */
                        std::filesystem::create_directories( regbl_database );

                    }

                    /* process detection */
                    regbl_detect( regbl_database_path, regbl_map, regbl_track, regbl_database, regbl_year, std::stod( regbl_xmin, NULL ), std::stod( regbl_xmax, NULL ), std::stod( regbl_ymin, NULL ), std::stod( regbl_ymax, NULL ) );

                    /* create path */
                    regbl_frame = regbl_export + "/output_frame/frame_" + regbl_location;

                    /* check directory */
                    if ( std::filesystem::is_directory( regbl_frame ) == false ) {

                        /* create directory */
                        std::filesystem::create_directories( regbl_frame );

                    }

                    /* restore map y-axis */
                    cv::flip( regbl_track, regbl_track, 0 );

                    /* export map with detections */
                    cv::imwrite( regbl_frame + "/" + regbl_location + "_" + regbl_year + ".tif", regbl_track );

                } else {

                    /* display message */
                    std::cerr << "error : unable to access map" << std::endl;

                    /* abort */
                    return( 1 );

                }

            } else {

                /* display message */
                std::cerr << "error : unable to parse storage list" << std::endl;

                /* abort */
                return( 1 );

            }

        }

        /* delete list stream */
        regbl_list.close();

        /* send message */
        return( 0 );

    }

