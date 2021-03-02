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
    source - Rendering methods
 */

    void regbl_tracker_crop( cv::Mat & regbl_crop, int const regbl_cnx, int const regbl_cny, int const regbl_detect ) {

        /* color value */
        cv::Scalar regbl_color( ( regbl_detect == 0 ) ? cv::Scalar( 78, 66, 192 ) : cv::Scalar( 98, 142, 22 ) );

        /* draw edge overlay */
        cv::rectangle( regbl_crop, cv::Rect( 0, -1, regbl_crop.cols, regbl_crop.rows + 2 ), regbl_color, 1 );
        
        /* display central cross */
        cv::line( regbl_crop, cv::Point( regbl_cnx, 0 ), cv::Point(regbl_cnx, regbl_cny - REGBL_TRACKER_CROSHALF * 0.75 ), regbl_color );
        cv::line( regbl_crop, cv::Point( 0, regbl_cny ), cv::Point(regbl_cnx - REGBL_TRACKER_CROSHALF * 0.75, regbl_cny ), regbl_color );

        /* display central cross */
        cv::line( regbl_crop, cv::Point( regbl_cnx, regbl_crop.rows ), cv::Point(regbl_cnx, regbl_cny + REGBL_TRACKER_CROSHALF * 0.75 ), regbl_color );
        cv::line( regbl_crop, cv::Point( regbl_crop.cols, regbl_cny ), cv::Point(regbl_cnx + REGBL_TRACKER_CROSHALF * 0.75, regbl_cny ), regbl_color );


    }

    void regbl_tracker_building( cv::Mat & regbl_crop, int const regbl_cnx, int const regbl_cny, std::ifstream & regbl_stream ) {

        /* importation token */
        double regbl_cx( 0. );
        double regbl_cy( 0. );
        double regbl_ux( 0. );
        double regbl_uy( 0. );

        /* display central position */
        cv::circle( regbl_crop, cv::Point( regbl_cnx, regbl_cny ), 1, cv::Scalar( 224, 16, 224 ), cv::FILLED );

        /* clear stream */
        regbl_stream.clear();
        regbl_stream.seekg(0);

        /* import central position */
        regbl_stream >> regbl_cx >> regbl_cy;

        /* parsing remaining position */
        while ( regbl_stream >> regbl_ux >> regbl_uy ) {

            /* shift to center */
            regbl_ux -= regbl_cx;
            regbl_uy -= regbl_cy;

            /* round and compute position */
            regbl_ux = regbl_cnx + std::round( regbl_ux );
            regbl_uy = regbl_cny - std::round( regbl_uy );

            /* display entires position */
            cv::circle( regbl_crop, cv::Point( regbl_ux, regbl_uy ), 1, cv::Scalar( 224, 16, 224 ), cv::FILLED );

            /* draw linking line */
            cv::line( regbl_crop, cv::Point( regbl_cnx, regbl_cny ), cv::Point( regbl_ux, regbl_uy ), cv::Scalar( 224, 16, 224 ) );

        }

    }

    void regbl_tracker_surface( cv::Mat & regbl_crop, int const regbl_cnx, int const regbl_cny, double const regbl_surface ) {

        /* check surface value */
        if ( regbl_surface < 0 ) {

            /* abort drawing */
            return;

        }

        /* display building sythetic surface */
        cv::circle( regbl_crop, cv::Point( regbl_cnx, regbl_cny ), std::sqrt( regbl_surface / 3.1415926353 ), cv::Scalar( 224, 16, 224 ) );

    }

    void regbl_tracker_detect( cv::Mat & regbl_crop, double const regbl_dx, double const regbl_dy, double const regbl_size, bool const regbl_detect ) {

        /* check detection result */
        if ( regbl_detect == 0 ) {

            /* abort drawing */
            return;

        }

        /* display connected area size */
        cv::circle( regbl_crop, cv::Point( regbl_dx, regbl_dy ), std::sqrt( regbl_size / 3.1415926353 ), cv::Scalar( 98, 142, 22 ) );

        /* display detection cross */
        cv::line( regbl_crop, cv::Point( regbl_dx, regbl_dy - 3 ), cv::Point( regbl_dx, regbl_dy + 3 ), cv::Scalar( 98, 142, 22 ) );
        cv::line( regbl_crop, cv::Point( regbl_dx - 3, regbl_dy ), cv::Point( regbl_dx + 3, regbl_dy ), cv::Scalar( 98, 142, 22 ) );

    }

    cv::Mat regbl_tracker_timeline( int const regbl_width, std::string regbl_year, int const regbl_detect ) {

        /* text shift value */
        int regbl_shift( ( regbl_width - ( REGBL_TRACKER_CROPHALF * 2 ) ) / 2 );

        /* color value */
        cv::Scalar regbl_color( ( regbl_detect == 0 ) ? cv::Scalar( 78, 66, 192 ) : cv::Scalar( 98, 142, 22 ) );

        /* returned matrix */
        cv::Mat regbl_return( 18, regbl_width, CV_8UC3, regbl_color );

        /* display crop date */
        cv::putText( regbl_return, regbl_year, cv::Point( 40 + regbl_shift, 14 ), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar( 255, 255, 255 ) );

        /* return matrix */
        return( regbl_return );

    }

    cv::Mat regbl_tracker_detection( int const regbl_width, std::string regbl_year, std::string regbl_hbound ) {

        /* color value */
        cv::Scalar regbl_color;

        /* check detection result */
        if ( std::stoi( regbl_year ) >= std::stoi( regbl_hbound ) ) {

            /* assign color */
            regbl_color = cv::Scalar( 98, 142, 22 );

        } else {

            /* assign color */
            regbl_color = cv::Scalar( 78, 66, 192 );

        }

        /* returned matrix */
        cv::Mat regbl_return( 18, regbl_width, CV_8UC3, regbl_color );

        /* return matrix */
        return( regbl_return );

    }

    cv::Mat regbl_tracker_reference( int const regbl_width, std::string regbl_geid, std::string regbl_year, std::string regbl_udeduce, std::string regbl_ldeduce ) {

        /* color value */
        cv::Scalar regbl_color;

        /* check reference availability */
        if ( regbl_year == "NO_REF" ) {

                /* assign color */
                regbl_color = cv::Scalar( 64, 64, 64 );

        } else {

            /* check detection results */        
            if ( ( std::stoi( regbl_year ) > std::stoi( regbl_ldeduce ) ) && ( std::stoi( regbl_year ) <= std::stoi( regbl_udeduce ) ) ) {

                /* assign color */
                regbl_color = cv::Scalar( 98, 142, 22 );

            } else {

                /* assign color */
                regbl_color = cv::Scalar( 78, 66, 192 );


            }

        }

        /* returned matrix */
        cv::Mat regbl_return( 18, regbl_width, CV_8UC3, regbl_color );

        /* display reference text */
        cv::putText( regbl_return, regbl_geid + " " + regbl_ldeduce + "-" + regbl_udeduce + " " + regbl_year, cv::Point( 0, 14 ), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar( 255, 255, 255 ) );

        /* return matrix */
        return( regbl_return );

    }

/*
    source - Main function
 */

    int main( int argc, char ** argv ) {

        /* storage structure path */
        char * regbl_storage_path( lc_read_string( argc, argv, "--storage", "-s" ) );

        /* building identifier (EGID) */
        char * regbl_building_id( lc_read_string( argc, argv, "--egid", "-g" ) );

        /* exportation directory */
        char * regbl_output_path( lc_read_string( argc, argv, "--export", "-e" ) );

        /* path composition */
        std::string regbl_export_position;
        std::string regbl_export_detect;
        std::string regbl_export_reference;
        std::string regbl_export_surface;
        std::string regbl_export_deduce;

        /* importation token */
        std::string regbl_year;
        std::string regbl_ryear;
        std::string regbl_udeduce;
        std::string regbl_ldeduce;
        std::string regbl_void;

        /* importation token */
        int regbl_area( -1 );
        int regbl_size( -1 );
        int regbl_flag( -1 );

        /* importation token */
        double regbl_posx( 0. );
        double regbl_posy( 0. );
        double regbl_detx( 0. );
        double regbl_dety( 0. );

        /* factor value */
        double regbl_factor( 0. );

        /* cropping coordinates */
        int regbl_clx( 0 );
        int regbl_chx( 0 );
        int regbl_cly( 0 );
        int regbl_chy( 0 );

        /* crop center coordinates */
        int regbl_cnx( 0 );
        int regbl_cny( 0 );

        /* parsing index */
        unsigned int regbl_index( 0 );

        /* raster matrix */
        cv::Mat regbl_load;
        cv::Mat regbl_crop;

        /* combined matrix */
        cv::Mat regbl_ftln;
        cv::Mat regbl_stln;
        cv::Mat regbl_alin;
        cv::Mat regbl_adet;
        cv::Mat regbl_aref;

        /* storage list */
        lc_list_t regbl_list;

        /* building database stream */
        std::ifstream regbl_input;
        std::ifstream regbl_position;
        std::ifstream regbl_reference;
        std::ifstream regbl_surface;
        std::ifstream regbl_deduce;

        /* check path specification */
        if ( regbl_storage_path == NULL ) {

            /* display message */
            std::cerr << "error : storage path specification" << std::endl;

            /* send message */
            return( 1 );

        }

        /* check path specification */
        if ( regbl_building_id == NULL ) {

            /* display message */
            std::cerr << "error : building EGID specification" << std::endl;

            /* send message */
            return( 1 );

        }

        /* check path specification */
        if ( regbl_output_path == NULL ) {

            /* display message */
            std::cerr << "error : exportation path specification" << std::endl;

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

            /* display message */
            std::cerr << "error : unable to locate detection directory" << std::endl;

            /* send message */
            return( 1 );

        }

        /* compose path */
        regbl_export_reference = std::string( regbl_storage_path ) + "/regbl_output/output_reference";

        /* check consistency */
        if ( std::filesystem::is_directory( regbl_export_reference ) == false ) {

            /* display message */
            std::cerr << "error : unable to locate reference directory" << std::endl;

            /* send message */
            return( 1 );

        }

        /* compose path */
        regbl_export_deduce = std::string( regbl_storage_path ) + "/regbl_output/output_deduce";

        /* check consistency */
        if ( std::filesystem::is_directory( regbl_export_deduce ) == false ) {

            /* display message */
            std::cerr << "error : unable to locate deduction directory" << std::endl;

            /* send message */
            return( 1 );

        }

        /* create reference stream */
        regbl_reference.open( regbl_export_reference + "/" + std::string( regbl_building_id ), std::ifstream::in );

        /* check reference stream */
        if ( regbl_reference.is_open() == false ) {

            /* assign no reference flag */
            regbl_ryear = "NO_REF";

        } else {

            /* import reference date */
            regbl_reference >> regbl_ryear;

            /* delete reference stream */
            regbl_reference.close();

        }

        /* create deduction stream */
        regbl_deduce.open( regbl_export_deduce + "/" + std::string( regbl_building_id ), std::ifstream::in );

        /* check deduction stream */
        if ( regbl_deduce.is_open() == false ) {

            /* display message */
            std::cerr << "error : unable to import building deduced range" << std::endl;

            /* send message */
            return( 1 );

        } else {

            /* import deduced range */
            regbl_deduce >> regbl_udeduce >> regbl_ldeduce;

            /* delete deduction stream */
            regbl_deduce.close();

        }

        /* create surface stream */
        regbl_surface.open( std::string( regbl_storage_path ) + "/regbl_output/output_surface/" + std::string( regbl_building_id ), std::ifstream::in );

        /* check input stream */
        if ( regbl_surface.is_open() == false ) {

            /* display warning */
            std::cerr << "warning : building surface file not found : surface not displayed" << std::endl;

        } else {

            /* import surface value */
            regbl_surface >> regbl_area;

            /* delete surface stream */
            regbl_surface.close();

        }

        /* create input stream */
        regbl_input.open( regbl_export_detect + "/" + std::string( regbl_building_id ), std::ifstream::in );

        /* check input stream */
        if ( regbl_input.is_open() == false ) {

            /* display message */
            std::cerr << "error : unable to locate deduction file" << std::endl;

            /* send message */
            return( 1 );

        }

        /* input stream parsing */
        while ( regbl_input >> regbl_year >> regbl_flag >> regbl_detx >> regbl_dety >> regbl_size ) {

            /* create position stream */
            regbl_position.open( regbl_export_position + "/" + regbl_year + "/" + std::string( regbl_building_id ), std::ifstream::in );

            /* check position stream */
            if ( regbl_position.is_open() == false ) {

                /* display message */
                std::cerr << "error : unable to access position file" << std::endl;

                /* send message */
                return( 1 );

            }

            /* import position */
            if ( regbl_position >> regbl_posx >> regbl_posy ) {

                /* compute metric factor */
                regbl_factor = lc_list_metric_to_pixel( regbl_list, regbl_index );

                /* invert y coordinate */
                regbl_posy = std::stod( regbl_list[regbl_index][6] ) - regbl_posy - 1;
                regbl_dety = std::stod( regbl_list[regbl_index][6] ) - regbl_dety - 1;

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
                if ( regbl_chx > std::stod( regbl_list[regbl_index][5] ) ) {

                    /* update crop coordinates */
                    regbl_chx = std::stod( regbl_list[regbl_index][5] );

                }

                /* edge detection */
                if ( regbl_chy > std::stod( regbl_list[regbl_index][6] ) ) {

                    /* update crop coordinates */
                    regbl_chy = std::stod( regbl_list[regbl_index][6] );

                }
                
                /* import original map */
                regbl_load = cv::imread( std::string( regbl_storage_path ) + "/regbl_frame/frame_original/" + regbl_year + ".tif", cv::IMREAD_COLOR );

                /* check importation */
                if ( regbl_load.empty() == true ) {

                    /* display message */
                    std::cerr << "error : unable to import original map" << std::endl;

                    /* send message */
                    return( 1 );

                }

                /* crop original map */
                regbl_crop = regbl_load( cv::Rect( regbl_clx, regbl_cly, regbl_chx - regbl_clx, regbl_chy - regbl_cly ) );

                /* process crop */
                regbl_tracker_crop( regbl_crop, regbl_cnx, regbl_cny, regbl_flag );

                /* draw building and entries */
                regbl_tracker_building( regbl_crop, regbl_cnx, regbl_cny, regbl_position );

                /* draw building surface */
                regbl_tracker_surface( regbl_crop, regbl_cnx, regbl_cny, regbl_area * regbl_factor );

                /* display detection information */
                regbl_tracker_detect( regbl_crop, regbl_cnx + ( regbl_detx - regbl_posx ), regbl_cny + ( regbl_dety - regbl_posy ), regbl_size, regbl_flag );

                /* check accumulator state */
                if ( regbl_ftln.empty() == true ) {

                    /* bootstrap accumulator */
                    regbl_ftln = regbl_crop;

                } else {

                    /* update accumulator */
                    cv::hconcat( regbl_crop, regbl_ftln, regbl_ftln );

                }

                /* import segmented map */
                regbl_load = cv::imread( std::string( regbl_storage_path ) + "/regbl_frame/frame/" + regbl_year + ".tif", cv::IMREAD_COLOR );

                /* check importation */
                if ( regbl_load.empty() == true ) {

                    /* display message */
                    std::cerr << "error : unable to import segmented map" << std::endl;

                    /* send message */
                    return( 1 );

                }

                /* crop segmented map */
                regbl_crop = regbl_load( cv::Rect( regbl_clx, regbl_cly, regbl_chx - regbl_clx, regbl_chy - regbl_cly ) );

                /* process crop */
                regbl_tracker_crop( regbl_crop, regbl_cnx, regbl_cny, regbl_flag );

                /* draw building and entries */
                regbl_tracker_building( regbl_crop, regbl_cnx, regbl_cny, regbl_position );

                /* draw building surface */
                regbl_tracker_surface( regbl_crop, regbl_cnx, regbl_cny, regbl_area * regbl_factor );

                /* display detection information */
                regbl_tracker_detect( regbl_crop, regbl_cnx + ( regbl_detx - regbl_posx ), regbl_cny + ( regbl_dety - regbl_posy ), regbl_size, regbl_flag );

                /* check accumulator state */
                if ( regbl_stln.empty() == true ) {

                    /* bootstrap accumulator */
                    regbl_stln = regbl_crop;

                } else {

                    /* update accumulator */
                    cv::hconcat( regbl_crop, regbl_stln, regbl_stln );

                }

                /* create timeline */
                regbl_crop = regbl_tracker_timeline( regbl_crop.cols, regbl_year, regbl_flag );

                /* check accumulator state */
                if ( regbl_alin.empty() == true ) {

                    /* bootstrap accumulator */
                    regbl_alin = regbl_crop;

                } else {

                    /* update accumulator */
                    cv::hconcat( regbl_crop, regbl_alin, regbl_alin );

                }

                /* create detection zone */
                regbl_crop = regbl_tracker_detection( regbl_crop.cols, regbl_year, regbl_udeduce );

                /* check accumulator state */
                if ( regbl_adet.empty() == true ) {

                    /* bootstrap accumulator */
                    regbl_adet = regbl_crop;

                } else {

                    /* update accumulator */
                    cv::hconcat( regbl_crop, regbl_adet, regbl_adet );

                }


            } else {

                /* display message */
                std::cerr << "error : unable to import building position from file" << std::endl;

                /* send message */
                return( 1 );

            }

            /* close position stream */
            regbl_position.close();

            /* update index */
            regbl_index ++;

        }

        /* delete input stream */
        regbl_input.close();

        /* compose reference bar */
        regbl_aref = regbl_tracker_reference( regbl_ftln.cols, std::string( regbl_building_id ), regbl_ryear, regbl_udeduce, regbl_ldeduce );

        /* compose single representation */
        cv::vconcat( regbl_aref, regbl_ftln, regbl_aref );
        cv::vconcat( regbl_aref, regbl_alin, regbl_aref );
        cv::vconcat( regbl_aref, regbl_stln, regbl_aref );
        cv::vconcat( regbl_aref, regbl_adet, regbl_aref );

        /* export result */
        cv::imwrite( std::string( regbl_output_path ) + "/" + std::string( regbl_building_id ) + ".png", regbl_aref );

        /* send message */
        return( 0 );

    }

