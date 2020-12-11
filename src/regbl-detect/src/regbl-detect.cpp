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
    source - main function
 */

    int main( int argc, char ** argv ) {

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

