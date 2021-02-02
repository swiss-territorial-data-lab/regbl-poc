/*
 *  regbl - deduce
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

    # include "regbl-deduce.hpp"

/*
    source - Processing methods
 */

    void regbl_deduce_detect( std::string regbl_detect, std::string regbl_deduce ) {

        /* input stream */
        std::ifstream regbl_input;

        /* output stream */
        std::ofstream regbl_output;

        /* input token */
        std::string regbl_date;
        std::string regbl_state;
        std::string regbl_void;

        /* input token */
        int regbl_size( 0 );

        /* memory token */
        std::string regbl_memory( "32767" );

        /* memory token */
        int regbl_push( -1 );

        /* state flags */
        bool regbl_flag( true );
        bool regbl_detected( false );

        /* create input stream */
        regbl_input.open( regbl_detect, std::ifstream::in );

        /* check consistency */
        if ( regbl_input.is_open() == false ) {

            /* display message */
            std::cerr << "error : unable to access detection file" << std::endl;

            /* send message */
            exit( 1 );

        }

        /* create output stream */
        regbl_output.open( regbl_deduce, std::ofstream::out );

        /* check consistency */
        if ( regbl_output.is_open() == false ) {

            /* display message */
            std::cerr << "error : unable to write in storage structure : deduction file" << std::endl;

            /* send message */
            exit( 1 );

        }

        /* parsing file */
        while ( ( regbl_flag == true ) && ( regbl_detected == false ) ) {

            /* import token */
            if ( regbl_input >> regbl_date >> regbl_state >> regbl_void >> regbl_void >> regbl_size ) {

                /* check flag state */
                if ( regbl_state == "0" ) {

                    /* export detected building date range boundary */
                    regbl_output << regbl_memory << " " << regbl_date;

                    /* update detection state */
                    regbl_detected = true;

                } else {

                    /* check pushed value */
                    if ( regbl_push > 0 ) {

                        /* apply morphologic test */
                        if ( regbl_ratio( regbl_push, regbl_size ) > REGBL_DEDUCE_RATIO ) {

                            /* export detected building date range boundary */
                            regbl_output << regbl_memory << " " << regbl_date;

                            /* update detection state */
                            regbl_detected = true;

                        }

                    }

                }

                /* push date */
                regbl_memory = regbl_date;

                /* push size */
                regbl_push = regbl_size;

            } else {

                /* update loop state */
                regbl_flag = false;

            }

        }

        /* check detection state */
        if ( regbl_detected == false ) {

            /* export detected building date range boundary */
            regbl_output << regbl_memory << " -32768";

        }

        /* delete output stream */
        regbl_output.close();

        /* delete stream */
        regbl_input.close();

    }

/*
    source - Main function
 */

    int main( int argc, char ** argv ) {

        /* storage structure path */
        char * regbl_storage_path( lc_read_string( argc, argv, "--storage", "-s" ) );

        /* egid value */
        std::string regbl_egid;

        /* path composition */
        std::string regbl_export_egid;
        std::string regbl_export_detect;
        std::string regbl_export_deduce;

        /* check path specification */
        if ( regbl_storage_path == NULL ) {

            /* displays message */
            std::cerr << "error : storage path specification" << std::endl;

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
        regbl_export_detect = std::string( regbl_storage_path ) + "/regbl_output/output_detect";

        /* check consistency */
        if ( std::filesystem::is_directory( regbl_export_egid ) == false ) {

            /* display message */
            std::cerr << "error : unable to locate detect directory" << std::endl;

            /* send message */
            return( 1 );

        }

        /* compose path */
        regbl_export_deduce = std::string( regbl_storage_path ) + "/regbl_output/output_deduce";

        /* check consistency */
        if ( std::filesystem::is_directory( regbl_export_deduce ) == false ) {

            /* create directory */
            std::filesystem::create_directories( regbl_export_deduce );

        } else {

            /* display message */
            std::cerr << "error : deduce directory already processed" << std::endl;

            /* send message */
            return( 1 );

        }

        /* parsing egid */
        for ( const std::filesystem::directory_entry & regbl_file : std::filesystem::directory_iterator( regbl_export_egid ) ) {

            /* check for regular file */
            if ( regbl_file.is_regular_file() == true ) {

                /* extract egid */
                regbl_egid = regbl_file.path().filename();

                /* deduction process */
                regbl_deduce_detect( regbl_export_detect + "/" + regbl_egid, regbl_export_deduce + "/" + regbl_egid );                

            }

        }

        /* send message */
        return( 0 );

    }

