/*
 *  regbl - segmentation
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

    # include "regbl-segmentation.hpp"

/*
    source - i/o methods
 */

    int regbl_io_state( cv::Mat & regbl_image, int regbl_state, std::string regbl_path ) {

        /* compose exportation path */
        std::string regbl_export( regbl_path + "/state-" + std::to_string( regbl_state ) + ".png" );

        /* export image */
        cv::imwrite( regbl_export.c_str(), regbl_image );

        /* update and return state */
        return( ++ regbl_state );

    }

/*
    source - tool methods
 */

    void regbl_tool_equalize( cv::Mat & regbl_in ) {

        /* channel sparation vector */
        std::vector< cv::Mat > regbl_channel;

        /* statistical quantities */
        cv::Scalar regbl_mean, regbl_std;

        /* convert image to yuv */
        cv::cvtColor( regbl_in, regbl_in, cv::COLOR_BGR2YCrCb );

        /* separate image channel */
        cv::split( regbl_in, regbl_channel );

        /* compute luminance mean and standard deviation */
        cv::meanStdDev( regbl_channel[0], regbl_mean, regbl_std );

        /* equalization */
        regbl_channel[0] = ( ( regbl_channel[0] - regbl_mean[0] ) / regbl_std[0] ) * 128 + regbl_mean[0];

        /* merge image channel */
        cv::merge( regbl_channel, regbl_in );

        /* convert image back to rgb */
        cv::cvtColor( regbl_in, regbl_in, cv::COLOR_YCrCb2BGR );

    }

/*
    source - processing methods
 */

    void regbl_process_extract_black( cv::Mat & regbl_in, cv::Mat & regbl_out, int const regbl_l2dist ) {

        /* gray proximity flag */
        bool regbl_gflag( false );

        /* initialise output matrix */
        regbl_out = cv::Mat( cv::Size( regbl_in.cols, regbl_in.rows ), CV_8UC1, cv::Scalar(255));

        /* L2-norm */
        double regbl_l2norm( 0 );

        /* parsing pixels */
        for ( int regbl_x = 0; regbl_x < regbl_in.cols; regbl_x ++ ) {

            /* parsing pixels */
            for ( int regbl_y = 0; regbl_y < regbl_in.rows; regbl_y ++ ) {

                /* compute L2-norm */
                regbl_l2norm = regbl_in.at<cv::Vec3b>(regbl_y,regbl_x)[0] * regbl_in.at<cv::Vec3b>(regbl_y,regbl_x)[0] +
                               regbl_in.at<cv::Vec3b>(regbl_y,regbl_x)[1] * regbl_in.at<cv::Vec3b>(regbl_y,regbl_x)[1] +
                               regbl_in.at<cv::Vec3b>(regbl_y,regbl_x)[2] * regbl_in.at<cv::Vec3b>(regbl_y,regbl_x)[2];

                /* check condition */
                if ( regbl_l2norm < ( regbl_l2dist * regbl_l2dist ) ) {

                    /* reset proximity flag */
                    regbl_gflag = true;

                    /* parsing components */
                    for ( int regbl_i = 0; regbl_i < 2; regbl_i ++ ) {

                        /* parsing components */
                        for ( int regbl_j = regbl_i + 1; regbl_j < 3; regbl_j ++ ) {

                            /* component proximity check */
                            if(std::abs(regbl_in.at<cv::Vec3b>(regbl_y,regbl_x)[regbl_i]-regbl_in.at<cv::Vec3b>(regbl_y,regbl_x)[regbl_j]) > 16) {

                                /* update proximity flag */
                                regbl_gflag = false;
                            }

                        }

                    }

                    /* check proximity detection results */
                    if ( regbl_gflag == true ) {

                        /* assign black pixel */
                        regbl_out.at<uchar>(regbl_y,regbl_x) = 0;

                    }

                }

            }

        }

    }

    void regbl_process_conway_iteration( cv::Mat & regbl_in, cv::Mat & regbl_out, int const regbl_kernel, float const regbl_gamevalue ) {

        /* initialise output matrix */
        regbl_out = cv::Mat( cv::Size( regbl_in.cols, regbl_in.rows ), CV_8UC1, cv::Scalar(255));        

        /* kernel boundaries */
        int regbl_lx( 0 );
        int regbl_ly( 0 );
        int regbl_hx( 0 );
        int regbl_hy( 0 );

        /* conway proportion */
        int regbl_count( 0 );
        int regbl_total( 0 );

        /* parsing pixels */
        for ( int regbl_x = 0; regbl_x < ( regbl_in.cols ); regbl_x ++ ) {

            /* parsing pixels */
            for ( int regbl_y = 0; regbl_y < ( regbl_in.rows ); regbl_y ++ ) {

                /* compute kernel boundaries */
                regbl_lx = regbl_x - regbl_kernel;
                regbl_ly = regbl_y - regbl_kernel;
                regbl_hx = regbl_x + regbl_kernel + 1;
                regbl_hy = regbl_y + regbl_kernel + 1;

                /* clamp kernel boundaries */
                regbl_lx = ( regbl_lx < 0 ) ? 0 : regbl_lx;
                regbl_ly = ( regbl_ly < 0 ) ? 0 : regbl_ly;

                /* clamp kernel boundaries */
                regbl_hx = ( regbl_hx > regbl_in.cols ) ? regbl_in.cols : regbl_hx;
                regbl_hy = ( regbl_hy > regbl_in.rows ) ? regbl_in.rows : regbl_hy;

                /* reset proportion */
                regbl_count = 0;
                regbl_total = 0;

                /* conway region parser */
                for ( int u = regbl_lx; u < regbl_hx; u ++ ) {

                    /* conway region parser */
                    for ( int v = regbl_ly; v < regbl_hy; v ++ ) {

                        /* check region cell state */
                        if ( regbl_in.at<uchar>(v,u) < 128 ) {

                            /* update counter */
                            regbl_count ++;

                        }

                        /* update total */
                        regbl_total ++;

                    }

                }

                /* cell living condition */
                if ( ( ( double ) regbl_count / regbl_total ) > regbl_gamevalue ) {

                    /* assign binary black */
                    regbl_out.at<uchar>(regbl_y,regbl_x) = 0;

                }

            }

        }

    }

/*
    source - main function
 */

    int main( int argc, char ** argv ) {

        /* input image path */
        char * regbl_input_path( lc_read_string( argc, argv, "--input", "-i" ) );

        /* output image path */
        char * regbl_output_path( lc_read_string( argc, argv, "--output", "-o" ) );

        /* exportation path */
        char * regbl_state_path( lc_read_string( argc, argv, "--state", "-s" ) );

        /* resize factor */
        double regbl_factor( lc_read_double( argc, argv, "--factor", "-f", 0.5 ) );

        /* iteration limit */
        int regbl_kernel( lc_read_signed( argc, argv, "--kernel", "-l", 3 ) );

        /* black extraction L2 */
        int regbl_black( lc_read_signed( argc, argv, "--black", "-b", 64 ) );

        /* source image */
        cv::Mat regbl_source, regbl_binary, regbl_swap;

        /* state variable */
        int regbl_state( 1 );

        /* component array */
        regbl_component_t regbl_component;

        /* check consistency */
        if ( ( regbl_input_path == NULL ) || ( regbl_output_path == NULL ) ) {

            /* display message */
            std::cerr << "error : input/output files specification" << std::endl;

            /* abort */
            return( 0 );

        }

        /* import source image */
        regbl_source = cv::imread( regbl_input_path, cv::IMREAD_COLOR );

        /* check state specification */
        if ( regbl_state_path != NULL ) {

            /* export state */
            regbl_state = regbl_io_state( regbl_source, regbl_state, std::string( regbl_state_path ) );

        }

        /* check inversion */
        if ( lc_read_flag( argc, argv, "--invert", "-v" ) == true ) {

            /* invert image */
            regbl_source = cv::Scalar(255) - regbl_source;

            /* check state specification */
            if ( regbl_state_path != NULL ) {

                /* export state */
                regbl_state = regbl_io_state( regbl_source, regbl_state, std::string( regbl_state_path ) );

            }

        }

        /* check equalisation */
        if ( lc_read_flag( argc, argv, "--equalize", "-e" ) == true ) {

            /* equalize image */
            regbl_tool_equalize( regbl_source );

            /* check state specification */
            if ( regbl_state_path != NULL ) {

                /* export state */
                regbl_state = regbl_io_state( regbl_source, regbl_state, std::string( regbl_state_path ) );

            }

        }

        /* black element extraction */
        regbl_process_extract_black( regbl_source, regbl_binary, regbl_black );

        /* check state specification */
        if ( regbl_state_path != NULL ) {

            /* export state */
            regbl_state = regbl_io_state( regbl_binary, regbl_state, std::string( regbl_state_path ) );

        }

        /* apply conway iteration */
        regbl_process_conway_iteration( regbl_binary, regbl_swap, regbl_kernel, regbl_factor );

        /* check state specification */
        if ( regbl_state_path != NULL ) {

            /* export state */
            regbl_state = regbl_io_state( regbl_swap, regbl_state, std::string( regbl_state_path ) );

        }

        /* export result image */
        cv::imwrite( regbl_output_path, regbl_swap );

        /* system message */
        return( 0 );

    }

