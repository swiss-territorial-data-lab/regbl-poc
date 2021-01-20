/*
 *  regbl - bootstrap
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

    # include "regbl-bootstrap.hpp"

/*
    source - Extraction function
 */

    void regbl_bootstrap_extract( std::string regbl_GEB_path, std::string regbl_export_egid, std::string regbl_export_position, std::string regbl_export_reference, lc_list & regbl_list ) {

        /* reading buffers */
        char regbl_head[REGBL_BUFFER] = { 0 };
        char regbl_line[REGBL_BUFFER] = { 0 };

        /* reading token */
        char regbl_token[REGBL_BUFFER] = { 0 };
        char regbl_rdate[REGBL_BUFFER] = { 0 };

        /* detection index */
        int regbl_EGID ( 0 );
        int regbl_GKODE( 0 );
        int regbl_GKODN( 0 );
        int regbl_GBAUJ( 0 );

        /* coordinates variable */
        double regbl_x( 0. );
        double regbl_u( 0. );
        double regbl_y( 0. );
        double regbl_v( 0. );

        /* transfer matrix */
        std::vector < std::vector< double > > regbl_transfer;

        /* exportation stream */
        std::ofstream regbl_output;

        /* create database stream */
        std::ifstream regbl_stream( regbl_GEB_path, std::ifstream::in );

        /* check stream */
        if ( regbl_stream.is_open() == false ) {

            /* display message */
            std::cerr << "error : unable to open GEB database" << std::endl;

            /* abort */
            exit( 1 );

        }

        /* import database header */
        regbl_stream.getline( regbl_head, REGBL_BUFFER );

        /* detect and check entry */
        if ( ( regbl_EGID  = regbl_detect_database_header( regbl_head, "EGID"  ) ) < 0 ) {

            /* display message */
            std::cerr << "error : unable to locate EGID in GEB database" << std::endl;

            /* abort */
            exit( 1 );

        }

        /* detect and check entry */
        if ( ( regbl_GKODE = regbl_detect_database_header( regbl_head, "GKODE" ) ) < 0 ) {

            /* display message */
            std::cerr << "error : unable to locate GKODE in GEB database" << std::endl;

            /* abort */
            exit( 1 );

        }

        /* detect and check entry */
        if ( ( regbl_GKODN = regbl_detect_database_header( regbl_head, "GKODN" ) ) < 0 ) {

            /* display message */
            std::cerr << "error : unable to locate GKODN in GEB database" << std::endl;

            /* abort */
            exit( 1 );

        }

        /* detect and check entry */
        if ( ( regbl_GBAUJ = regbl_detect_database_header( regbl_head, "GBAUJ" ) ) < 0 ) {

            /* display message */
            std::cerr << "error : unable to locate GBAUJ in GEB database" << std::endl;

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

            /* read egid token */
            regbl_detect_database_entry( regbl_line, regbl_EGID, regbl_token );

            /* read reference date */
            regbl_detect_database_entry( regbl_line, regbl_GBAUJ, regbl_rdate );

            /* reset transfer matrix */
            regbl_transfer.clear();

            /* parsing storage list */
            for ( unsigned int regbl_parse = 0; regbl_parse < regbl_list.size(); regbl_parse ++ ) {

                /* convert coordinate according to current map */
                regbl_u = ( ( regbl_x - std::stod( regbl_list[regbl_parse][1], nullptr ) ) / ( std::stod( regbl_list[regbl_parse][2], nullptr ) - std::stod( regbl_list[regbl_parse][1], nullptr ) ) ) * std::stod( regbl_list[regbl_parse][5], nullptr );
                regbl_v = ( ( regbl_y - std::stod( regbl_list[regbl_parse][3], nullptr ) ) / ( std::stod( regbl_list[regbl_parse][4], nullptr ) - std::stod( regbl_list[regbl_parse][3], nullptr ) ) ) * std::stod( regbl_list[regbl_parse][6], nullptr );

                /* check coordinates */
                if ( ( regbl_u >= 0. ) && ( regbl_u < std::stod( regbl_list[regbl_parse][5], nullptr ) ) && ( regbl_v >= 0. ) && ( regbl_u < std::stod( regbl_list[regbl_parse][6], nullptr ) ) ) {

                    /* add matrix rows */
                    regbl_transfer.push_back( std::vector < double > (2) );

                    /* push coordinates */
                    regbl_transfer.back()[0] = regbl_u;
                    regbl_transfer.back()[1] = regbl_v;

                }

            }

            /* check transfer matrix */
            if ( regbl_transfer.size() != regbl_list.size() ) {

                /* detect partial selection */
                if ( regbl_transfer.size() > 0 ) {

                    /* display warning */
                    std::cerr << "warning : rejected building (" << regbl_token << ") as partially appearing on maps timeline" << std::endl;

                }

            } else {

                /* create egid stream */
                regbl_output.open( regbl_export_egid + "/" + regbl_token, std::ofstream::out );

                /* chech egid stream */
                if ( regbl_output.is_open() == false ) {

                    /* display message */
                    std::cerr << "error : unable to write in storage structure : egid file" << std::endl;

                    /* send message */
                    exit( 1 );

                }

                /* delete egid stream */
                regbl_output.close();

                /* parsing storage list */
                for ( unsigned int regbl_parse = 0; regbl_parse < regbl_list.size(); regbl_parse ++ ) {

                    /* create output stream */
                    regbl_output.open( regbl_export_position + "/" + regbl_token + "_" + regbl_list[regbl_parse][0], std::ofstream::out );

                    /* check output stream */
                    if ( regbl_output.is_open() == false ) {

                        /* display message */
                        std::cerr << "error : unable to write in storage structure : position file" << std::endl;

                        /* send message */
                        exit( 1 );

                    }

                    /* export position */
                    regbl_output << std::fixed << std::setprecision(3) << regbl_transfer[regbl_parse][0] << " " << regbl_transfer[regbl_parse][1] << std::endl;

                    /* delete output stream */
                    regbl_output.close();

                }

                /* check reference date */
                if ( std::strlen( regbl_rdate ) > 0 ) {
                
                    /* create reference stream */
                    regbl_output.open( regbl_export_reference + "/" + regbl_token, std::ofstream::out );

                    /* check reference stream */
                    if ( regbl_output.is_open() == false ) {

                        /* display message */
                        std::cerr << "error : unable to write in storage structure : reference file" << std::endl;

                        /* send message */
                        exit( 1 );

                    }

                    /* export reference date */
                    regbl_output << regbl_rdate << std::endl;

                    /* delete reference stream */
                    regbl_output.close();

                }

            }

        }

        /* delete database stream */
        regbl_stream.close(); 

    }

    void regbl_bootstrap_entries( std::string regbl_EIN_path, std::string regbl_export_egid, std::string regbl_export_position, lc_list & regbl_list ) {

        /* reading buffers */
        char regbl_head[REGBL_BUFFER] = { 0 };
        char regbl_line[REGBL_BUFFER] = { 0 };

        /* reading token */
        char regbl_token[REGBL_BUFFER] = { 0 };

        /* detection index */
        int regbl_EGID ( 0 );
        int regbl_DKODE( 0 );
        int regbl_DKODN( 0 );

        /* coordinates variable */
        double regbl_x( 0. );
        double regbl_u( 0. );
        double regbl_y( 0. );
        double regbl_v( 0. );

        /* transfer matrix */
        std::vector < std::vector< double > > regbl_transfer;

        /* exportation stream */
        std::ofstream regbl_output;

        /* create database stream */
        std::ifstream regbl_stream( regbl_EIN_path, std::ifstream::in );

        /* check stream */
        if ( regbl_stream.is_open() == false ) {

            /* display message */
            std::cerr << "error : unable to open EIN database" << std::endl;

            /* abort */
            exit( 1 );

        }

        /* import database header */
        regbl_stream.getline( regbl_head, REGBL_BUFFER );

        /* detect and check entry */
        if ( ( regbl_EGID  = regbl_detect_database_header( regbl_head, "EGID"  ) ) < 0 ) {

            /* display message */
            std::cerr << "error : unable to locate EGID in EIN database" << std::endl;

            /* abort */
            exit( 1 );

        }

        /* detect and check entry */
        if ( ( regbl_DKODE = regbl_detect_database_header( regbl_head, "DKODE" ) ) < 0 ) {

            /* display message */
            std::cerr << "error : unable to locate DKODE in EIN database" << std::endl;

            /* abort */
            exit( 1 );

        }

        /* detect and check entry */
        if ( ( regbl_DKODN = regbl_detect_database_header( regbl_head, "DKODN" ) ) < 0 ) {

            /* display message */
            std::cerr << "error : unable to locate DKODN in EIN database" << std::endl;

            /* abort */
            exit( 1 );

        }

        /* parsing database entries */
        while ( regbl_stream.getline( regbl_line, REGBL_BUFFER ) ) {

            /* read coordinates token */
            regbl_detect_database_entry( regbl_line, regbl_DKODE, regbl_token );

            /* check token state */
            if ( std::strlen( regbl_token ) > 0 ) {

                /* convert token */
                regbl_x = std::atof( regbl_token );

                /* read coordinates token */
                regbl_detect_database_entry( regbl_line, regbl_DKODN, regbl_token );

                /* check token state */
                if ( std::strlen( regbl_token ) > 0 ) {

                    /* convert token */
                    regbl_y = std::atof( regbl_token );

                    /* read egid token */
                    regbl_detect_database_entry( regbl_line, regbl_EGID, regbl_token );

                    /* check egid selection */
                    if ( std::filesystem::is_regular_file( regbl_export_egid + "/" + regbl_token ) == true ) {

                        /* reset transfer matrix */
                        regbl_transfer.clear();

                        /* parsing storage list */
                        for ( unsigned int regbl_parse = 0; regbl_parse < regbl_list.size(); regbl_parse ++ ) {

                            /* convert coordinate according to current map */
                            regbl_u = ( ( regbl_x - std::stod( regbl_list[regbl_parse][1], nullptr ) ) / ( std::stod( regbl_list[regbl_parse][2], nullptr ) - std::stod( regbl_list[regbl_parse][1], nullptr ) ) ) * std::stod( regbl_list[regbl_parse][5], nullptr );
                            regbl_v = ( ( regbl_y - std::stod( regbl_list[regbl_parse][3], nullptr ) ) / ( std::stod( regbl_list[regbl_parse][4], nullptr ) - std::stod( regbl_list[regbl_parse][3], nullptr ) ) ) * std::stod( regbl_list[regbl_parse][6], nullptr );

                            /* check coordinates */
                            if ( ( regbl_u >= 0. ) && ( regbl_u < std::stod( regbl_list[regbl_parse][5], nullptr ) ) && ( regbl_v >= 0. ) && ( regbl_u < std::stod( regbl_list[regbl_parse][6], nullptr ) ) ) {

                                /* add matrix rows */
                                regbl_transfer.push_back( std::vector < double > (2) );

                                /* push coordinates */
                                regbl_transfer.back()[0] = regbl_u;
                                regbl_transfer.back()[1] = regbl_v;

                            }

                        }

                        /* check selection */
                        if ( regbl_transfer.size() == regbl_list.size() ) {

                            /* parsing storage list */
                            for ( unsigned int regbl_parse = 0; regbl_parse < regbl_list.size(); regbl_parse ++ ) {

                                /* create output stream */
                                regbl_output.open( regbl_export_position + "/" + regbl_token + "_" + regbl_list[regbl_parse][0], std::ofstream::app );

                                /* check output stream */
                                if ( regbl_output.is_open() == false ) {

                                    /* display message */
                                    std::cerr << "error : unable to write in storage structure : position file" << std::endl;

                                    /* send message */
                                    exit( 1 );

                                }

                                /* export position */
                                regbl_output << std::fixed << std::setprecision(3) << regbl_transfer[regbl_parse][0] << " " << regbl_transfer[regbl_parse][1] << std::endl;

                                /* delete output stream */
                                regbl_output.close();

                            }

                        }

                    }

                }

            }

        }

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

        /* GEB database path */
        char * regbl_GEB_path( lc_read_string( argc, argv, "--geb", "-g" ) );

        /* EIN database path */
        char * regbl_EIN_path( lc_read_string( argc, argv, "--ein", "-e" ) );

        /* path composition */
        std::string regbl_export_reference;
        std::string regbl_export_egid;
        std::string regbl_export_position;

        /* storage list */
        lc_list regbl_list;

        /* check path specification */
        if ( regbl_storage_path == NULL ) {

            /* displays message */
            std::cerr << "error : storage path specification" << std::endl;

            /* send message */
            return( 1 );

        }

        /* check path specification */
        if ( regbl_GEB_path == NULL ) {

            /* displays message */
            std::cerr << "error : GEB database path specification" << std::endl;

            /* send message */
            return( 1 );

        }

        /* check path specification */
        if ( regbl_EIN_path == NULL ) {

            /* displays message */
            std::cerr << "error : EIN database path specification" << std::endl;

            /* send message */
            return( 1 );

        }

        /* import list */
        regbl_list = lc_list_import( std::string( regbl_storage_path ) + "/regbl_list" );

        /* check list */
        if ( regbl_list.size() == 0 ) {

            /* display message */
            std::cerr << "error : unable to import storage list file" << std::endl;

            /* send message */
            return( 1 );

        }

        /* compose path */
        regbl_export_egid = std::string( regbl_storage_path ) + "/regbl_output/output_egid";

        /* check directory */
        if ( std::filesystem::is_directory( regbl_export_egid ) == false ) {

            /* create directory */
            std::filesystem::create_directories( regbl_export_egid );

        }

        /* compose path */
        regbl_export_position = std::string( regbl_storage_path ) + "/regbl_output/output_position";

        /* check directory */
        if ( std::filesystem::is_directory( regbl_export_position ) == false ) {

            /* create directory */
            std::filesystem::create_directories( regbl_export_position );

        }  

        /* compose path */
        regbl_export_reference = std::string( regbl_storage_path ) + "/regbl_output/output_reference";

        /* check directory */
        if ( std::filesystem::is_directory( regbl_export_reference ) == false ) {

            /* create directory */
            std::filesystem::create_directories( regbl_export_reference );

        }      

        /* create main extraction - position, egid, reference */
        regbl_bootstrap_extract( regbl_GEB_path, regbl_export_egid, regbl_export_position, regbl_export_reference, regbl_list );

        /* create secondary extraction - entries position */
        regbl_bootstrap_entries( regbl_EIN_path, regbl_export_egid, regbl_export_position, regbl_list );

        /* send message */
        return( 0 );

    }

