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

    void regbl_deduce_detect( std::string regbl_path, std::string & regbl_export ) {

        /* input stream */
        std::ifstream regbl_input( regbl_path, std::ios::in );

        /* output stream */
        std::ofstream regbl_output;

        /* file tokens */
        std::string regbl_date;
        std::string regbl_state;
        std::string regbl_void;

        /* memory token */
        std::string regbl_memory( "2020" );

        /* loop state */
        bool regbl_flag( true );

        /* detection state */
        bool regbl_initial( false );
        bool regbl_detected( false );

        /* check consistency */
        if ( regbl_input.is_open() == false ) {

            /* display message */
            std::cerr << "error : unable to access database building file" << std::endl;

            /* send message */
            exit( 1 );

        }

        /* parsing file */
        while ( regbl_flag ) {

            /* import token */
            if ( regbl_input >> regbl_date >> regbl_state >> regbl_void >> regbl_void ) {

                /* check flag state */
                if ( regbl_state == "0" ) {

                    /* check initial state */
                    if ( regbl_initial == true ) {

                        /* create output stream */
                        regbl_output.open( regbl_export + "/" + std::string( std::filesystem::path( regbl_path ).filename() ), std::ios::out );

                        /* check consistency */
                        if ( regbl_output.is_open() == false ) {

                            /* display message */
                            std::cerr << "error : unable to access database building exportation file" << std::endl;

                            /* send message */
                            exit( 1 );

                        }

                        /* export detected building date range boundary */
                        regbl_output << regbl_memory << " " << regbl_date;

                        /* delete output stream */
                        regbl_output.close();

                        /* update loop state */
                        regbl_flag = false;

                        /* update detection state */
                        regbl_detected = true;

                    }

                } else {

                    /* update initial state */
                    regbl_initial = true;

                }

                /* push date */
                regbl_memory = regbl_date;

            } else {

                /* update loop state */
                regbl_flag = false;

            }

        }

        /* check detection state */
        if ( regbl_detected == false ) {

            /* create output stream */
            regbl_output.open( regbl_export + "/" + std::string( std::filesystem::path( regbl_path ).filename() ), std::ios::out );

            /* check consistency */
            if ( regbl_output.is_open() == false ) {

                /* display message */
                std::cerr << "error : unable to access database building exportation file" << std::endl;

                /* send message */
                exit( 1 );

            }

            /* export detected building date range boundary */
            regbl_output << regbl_memory << " 0";

            /* delete output stream */
            regbl_output.close();

        }

        /* delete stream */
        regbl_input.close();

    }

/*
    source - Main function
 */

    int main( int argc, char ** argv ) {

        /* storage structure path */
        char * regbl_storage_path( lc_read_string( argc, argv, "--storage", "-s" ) );

        /* location name */
        std::string regbl_location;

        /* path variable */
        std::string regbl_database;
        std::string regbl_reference;
        std::string regbl_export;
        std::string regbl_export_location;

        /* compose path */
        regbl_database = std::string( regbl_storage_path ) + "/regbl_output/output_database";

        /* check consistency */
        if ( std::filesystem::is_directory( regbl_database ) == false ) {

            /* display message */
            std::cerr << "error : database directory missing (unprocessed data)" << std::endl;

            /* send message */
            return( 1 );

        }

        /* compose path */
        regbl_reference = std::string( regbl_storage_path ) + "/regbl_output/output_reference";

        /* check consistency */
        if ( std::filesystem::is_directory( regbl_reference ) == false ) {

            /* display message */
            std::cerr << "error : reference directory missing (unprocessed data)" << std::endl;

            /* send message */
            return( 1 );

        }

        /* compose path */
        regbl_export = std::string( regbl_storage_path ) + "/regbl_output/output_deduce";

        /* check consistency */
        if ( std::filesystem::is_directory( regbl_export ) == true ) {

            /* display message */
            std::cerr << "error : structure already processed" << std::endl;

            /* send message */
            return( 1 );

        }

        /* create directory */
        std::filesystem::create_directories( regbl_export );

        /* parsing location database */
        for ( const std::filesystem::directory_entry & regbl_locs : std::filesystem::directory_iterator( regbl_database ) ) {

            /* check for directory only */
            if ( regbl_locs.is_directory() == true ) {

                /* extract location name */
                regbl_location = std::string( regbl_locs.path() ).substr( std::string( regbl_locs.path() ).rfind("_") + 1 );

                //std::cerr << regbl_locs.path() << " " << regbl_location << std::endl;
                
                /* parsing file in location database */
                for ( const std::filesystem::directory_entry & regbl_file : std::filesystem::directory_iterator( regbl_locs.path() ) ) {

                    /* compose path */
                    regbl_export_location = regbl_export + "/deduce_" + regbl_location;

                    /* create directory */
                    std::filesystem::create_directories( regbl_export_location );

                    //std::cerr << regbl_file << std::endl;

                    /* detection process */
                    regbl_deduce_detect( regbl_file.path(), regbl_export_location );

                }


            }

        }

        /* send message */
        return( 0 );

    }
