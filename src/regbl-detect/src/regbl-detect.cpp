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

    void regbl_detect( cv::Mat & regbl_image, std::string regbl_database, std::string regbl_date, std::string regbl_output ) {

        /* database connector */
        std::ifstream regbl_connect( regbl_database.c_str(), std::ifstream::in );

        /* output stream */
        std::ofstream regbl_stream( regbl_output.c_str(), std::ofstream::out | std::ofstream::app );

        /* i/o buffer */
        std::string regbl_buffer;

        /* position buffer */
        int regbl_x( 0 ), regbl_y( 0 );

        /* check output stream */
        if ( regbl_stream.is_open() ) {

            /* check connector */
            if ( regbl_connect.is_open() ) {

                /* reading database */
                while ( regbl_connect >> regbl_buffer ) {

                    /* extract position */
                    regbl_x = std::stoi( regbl_buffer );

                    /* reading database */
                    if ( regbl_connect >> regbl_buffer ) {

                        /* extract position */
                        regbl_y = std::stoi( regbl_buffer );

                        /* make detection */
                        if ( regbl_image.at<uchar>(regbl_y, regbl_x) < 255 ) {

                            regbl_stream << 1;

                        } else {

                            regbl_stream << 0;

                        }

                        regbl_stream << " ";

                    } else {

                        /* display message */
                        std::cerr << "error : unconsistent database" << std::endl;

                    }

                }

                /* finish output stream line */
                regbl_stream << regbl_date << std::endl;

                /* delete connector */
                regbl_connect.close();

            } else {

                /* display message */
                std::cerr << "error : unable to access database" << std::endl;

            }

            /* delete output stream */
            regbl_stream.close();

        } else {

            /* display message */
            std::cerr << "error : unable to create output stream" << std::endl;

        }

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

        /* database input */
        char * regbl_database_path( lc_read_string( argc, argv, "--database", "-d" ) );

        /* reading buffers */
        char regbl_head[REGBL_BUFFER] = { 0 };
        char regbl_line[REGBL_BUFFER] = { 0 };

        /* detection index */
        int regbl_EGID ( 0 );
        int regbl_GKODE( 0 );
        int regbl_GKODN( 0 );
        int regbl_GBAUJ( 0 );

        /* database stream */
        std::ifstream regbl_stream( regbl_database_path, std::ifstream::in );

        /* check stream */
        if ( regbl_stream.is_open() == false ) {

            /* display message */
            std::cerr << "error : unable to open database" << std::endl;

            /* abort */
            return( 0 );

        }

        /* import database header */
        regbl_stream.getline( regbl_head, REGBL_BUFFER );

        /* detect database entries */
        regbl_EGID  = regbl_detect_database_header( regbl_head, "EGID"  );
        regbl_GKODE = regbl_detect_database_header( regbl_head, "GKODE" );
        regbl_GKODN = regbl_detect_database_header( regbl_head, "GKODN" );
        regbl_GBAUJ = regbl_detect_database_header( regbl_head, "GBAUJ" );

        /* parsing database entries */
        while ( regbl_stream.getline( regbl_line, REGBL_BUFFER ) ) {

            //fprintf( stderr, "%s\n", regbl_line );

            char token[REGBL_BUFFER];

            regbl_detect_database_entry( regbl_line, regbl_GKODE, token );

            double coordx( std::atof( token ) );

            regbl_detect_database_entry( regbl_line, regbl_GKODN, token );

            double coordy( std::atof( token ) );

            fprintf( stderr, "%f %f\n", coordx, coordy );

        }

        /* delete input stream */
        regbl_stream.close();

        /* send message */
        return( 0 );

    }

    int main_( int argc, char ** argv ) {

        /* database input */
        char * regbl_database_path( lc_read_string( argc, argv, "--database", "-d" ) );

        /* source image directory */
        char * regbl_frame_path( lc_read_string( argc, argv, "--frame", "-f" ) );

        /* exportation file */
        char * regbl_output_path( lc_read_string( argc, argv, "--output", "-o" ) );

        /* source image */
        cv::Mat regbl_image;

        /* filename */
        std::string regbl_filename;

        /* check consistency */
        if ( ( regbl_database_path == NULL ) || ( regbl_frame_path == NULL ) || ( regbl_output_path == NULL ) ) {

            /* display message */
            std::cerr << "error : input/output files specification" << std::endl;

            /* abort */
            return( 0 );

        }

        /* parsing source image */
        for ( const auto & regbl_frame : std::filesystem::directory_iterator( regbl_frame_path ) ) {

            /* extract filename */
            regbl_filename = std::filesystem::path( regbl_frame.path() ).filename();

            /* display progression */
            std::cout << "Processing " << regbl_filename << " ..." << std::endl;

            /* import image */
            regbl_image = cv::imread( regbl_frame.path(), cv::IMREAD_GRAYSCALE );

            /* perform detection */
            regbl_detect( regbl_image, std::string( regbl_database_path ), regbl_filename.substr( 21, 4 ), std::string( regbl_output_path ) );

        }

        /* system message */
        return( 0 );

    }
