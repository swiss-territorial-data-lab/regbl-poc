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
        //regbl_channel[0] = ( ( regbl_channel[0] - regbl_mean[0] ) / regbl_std[0] ) * 128 + regbl_mean[0];
        regbl_channel[0] = ( ( regbl_channel[0] - regbl_mean[0] ) / regbl_std[0] ) * 64 + regbl_mean[0];

        /* merge image channel */
        cv::merge( regbl_channel, regbl_in );

        /* convert image back to rgb */
        cv::cvtColor( regbl_in, regbl_in, cv::COLOR_YCrCb2BGR );

    }

/*
    source - processing methods
 */

    void regbl_process_extract_black( cv::Mat & regbl_in, cv::Mat & regbl_out, int const regbl_l2dist ) {

        /* initialise output matrix */
        regbl_out = cv::Mat( cv::Size( regbl_in.cols, regbl_in.rows ), CV_8UC1, cv::Scalar(255));

        /* L2-norm */
        double regbl_l2norm( 0 );

        /* parsing pixels */
        for ( int x = 0; x < regbl_in.cols; x ++ ) {

            /* parsing pixels */
            for ( int y = 0; y < regbl_in.rows; y ++ ) {

                /* compute L2-norm */
                regbl_l2norm = regbl_in.at<cv::Vec3b>(y,x)[0] * regbl_in.at<cv::Vec3b>(y,x)[0] +
                               regbl_in.at<cv::Vec3b>(y,x)[1] * regbl_in.at<cv::Vec3b>(y,x)[1] +
                               regbl_in.at<cv::Vec3b>(y,x)[2] * regbl_in.at<cv::Vec3b>(y,x)[2];

                /* check condition */
                if ( regbl_l2norm < ( regbl_l2dist * regbl_l2dist ) ) {

                    int flag( 1 );

                    for ( int i = 0; i < 2; i ++ )
                    for ( int j = i+1; j < 3; j ++ )
                        if(std::abs(regbl_in.at<cv::Vec3b>(y,x)[i]-regbl_in.at<cv::Vec3b>(y,x)[j]) > 16) flag = 0;

                    /* assign binary black */
                    if ( flag == 1 ) regbl_out.at<uchar>(y,x) = 0;

                }

            }

        }

    }

    int regbl_process_conway_iteration( cv::Mat & regbl_in, cv::Mat & regbl_out, int const regbl_gamevalue ) {

        /* initialise output matrix */
        regbl_out = cv::Mat( cv::Size( regbl_in.cols, regbl_in.rows ), CV_8UC1, cv::Scalar(255));        

        /* conway game rule counter */
        int regbl_count( 0 );

        /* updated cell count */
        int regbl_updated( 0 );

        /* parsing pixels */
        for ( int x = 1; x < ( regbl_in.cols - 1 ); x ++ ) {

            /* parsing pixels */
            for ( int y = 1; y < ( regbl_in.rows - 1 ); y ++ ) {

                /* reset counter */
                regbl_count = 0;

                /* conway region parser */
                for ( int u = x - 1; u < ( x + 2 ); u ++ ) {

                    /* conway region parser */
                    for ( int v = y - 1; v < ( y + 2 ); v ++ ) {

                        /* check region cell state */
                        if ( regbl_in.at<uchar>(v,u) < 128 ) {

                            /* update counter */
                            regbl_count ++;

                        }

                    }

                }

                /* cell living condition */
                if ( regbl_count > regbl_gamevalue ) {

                    /* assign binary black */
                    regbl_out.at<uchar>(y,x) = 0;

                    /* check state */
                    if ( regbl_in.at<uchar>(y,x) > 128 ) {

                        /* update counter */
                        regbl_updated ++;

                    }

                }

            }

        }

        /* return amount of updated cell */
        return( regbl_updated );

    }

    regbl_component_t regbl_extract_connected( cv::Mat & regbl_in ) {

        /* tracking matrix */
        cv::Mat regbl_track( cv::Size( regbl_in.cols, regbl_in.rows ), CV_8UC1, cv::Scalar(0));

        /* flood-track neighbour array */
        int regbl_next[4][2] = { { +0, -1 }, { +0, +1 }, { -1, +0 }, { +1, +0 } };

        /* coordinate push buffer */
        int regbl_u( 0 ), regbl_v( 0 );

        /* returned component array */
        regbl_component_t regbl_return;

        /* parsing pixels */
        for ( int x = 1; x < ( regbl_in.cols - 1 ); x ++ ) {

            /* parsing pixels */
            for ( int y = 1; y < ( regbl_in.rows - 1 ); y ++ ) {

                /* check pixel state */
                if ( ( regbl_in.at<uchar>(y,x) < 128 ) && ( regbl_track.at<uchar>(y,x) == 0 ) ) {

                    /* component coordinates array */
                    std::vector< std::vector< double > > regbl_component{ { double( x ), double( y ) } };

                    /* update tracking */
                    regbl_track.at<uchar>(y,x) = 255;

                    /* component coordinates head */
                    unsigned int regbl_head( 0 );

                    /* component pixels gathering */
                    while ( regbl_head < regbl_component.size() ) {

                        /* parsing connected neighbours */
                        for ( int regbl_i = 0; regbl_i < 4; regbl_i ++ ) {

                            /* compute pre-pushed coordinates */
                            regbl_u = regbl_component[regbl_head][0] + regbl_next[regbl_i][0];
                            regbl_v = regbl_component[regbl_head][1] + regbl_next[regbl_i][1];

                            /* check connectivity */
                            if ( ( regbl_in.at<uchar>(regbl_v,regbl_u) < 128 ) && ( regbl_track.at<uchar>(regbl_v,regbl_u) == 0 ) ) {

                                /* add pixel to component */
                                regbl_component.push_back( std::vector< double > { double( regbl_u ), double( regbl_v ) } );

                                /* update tracking */
                                regbl_track.at<uchar>(regbl_v,regbl_u) = 255;

                            }
                            
                        }

                        /* update head */
                        regbl_head ++;

                    }

                    /* high-pass filter */
                    if ( regbl_component.size() > 8 ) {

                        /* push component */
                        regbl_return.push_back( regbl_component );

                    }

                }

            }

        }

        /* return component */
        return( regbl_return );

    }

    void regbl_process_pca_filtering( cv::Mat & regbl_in, cv::Mat & regbl_out ) {

        /* initialise output matrix */
        regbl_out = cv::Mat( cv::Size( regbl_in.cols, regbl_in.rows ), CV_8UC1, cv::Scalar(255));

        /* tracking matrix */
        cv::Mat regbl_track( cv::Size( regbl_in.cols, regbl_in.rows ), CV_8UC1, cv::Scalar(0));

        /* flood-track neighbour array */
        int regbl_next[4][2] = { { +0, -1 }, { +0, +1 }, { -1, +0 }, { +1, +0 } };

        /* coordinate push buffer */
        int regbl_u( 0 ), regbl_v( 0 );

        /* parsing pixels */
        for ( int x = 1; x < ( regbl_in.cols - 1 ); x ++ ) {

            /* parsing pixels */
            for ( int y = 1; y < ( regbl_in.rows - 1 ); y ++ ) {

                /* check pixel state */
                if ( ( regbl_in.at<uchar>(y,x) < 128 ) && ( regbl_track.at<uchar>(y,x) == 0 ) ) {

                    /* component coordinates array */
                    std::vector< std::vector< double > > regbl_component{ { double( x ), double( y ) } };

                    /* update tracking */
                    regbl_track.at<uchar>(y,x) = 255;

                    /* component coordinates head */
                    unsigned int regbl_head( 0 );

                    /* component pixels gathering */
                    while ( regbl_head < regbl_component.size() ) {

                        /* parsing connected neighbours */
                        for ( int regbl_i = 0; regbl_i < 4; regbl_i ++ ) {

                            /* compute pre-pushed coordinates */
                            regbl_u = regbl_component[regbl_head][0] + regbl_next[regbl_i][0];
                            regbl_v = regbl_component[regbl_head][1] + regbl_next[regbl_i][1];

                            /* check connectivity */
                            if ( ( regbl_in.at<uchar>(regbl_v,regbl_u) < 128 ) && ( regbl_track.at<uchar>(regbl_v,regbl_u) == 0 ) ) {

                                /* add pixel to component */
                                regbl_component.push_back( std::vector< double > { double( regbl_u ), double( regbl_v ) } );

                                /* update tracking */
                                regbl_track.at<uchar>(regbl_v,regbl_u) = 255;

                            }
                            
                        }

                        /* update head */
                        regbl_head ++;

                    }

                    if ( regbl_component.size() > 16 ) {

                        for ( unsigned int i = 0; i < regbl_component.size(); i ++ ) {

                            regbl_out.at<uchar>(regbl_component[i][1],regbl_component[i][0]) = 127;

                        }

                    }

                }

            }

        }

    }

    void regbl_process_component_raster( regbl_component_t & regbl_in, cv::Mat & regbl_out, cv::Size regbl_size ) {

        /* initialise output matrix */
        regbl_out = cv::Mat( regbl_size, CV_8UC1, cv::Scalar(255));

        /* parsing component */
        for ( unsigned int regbl_i = 0; regbl_i < regbl_in.size(); regbl_i ++ ) {

            /* parsing pixel */
            for ( unsigned int regbl_j = 0; regbl_j < regbl_in[regbl_i].size(); regbl_j ++ ) {

                /* render pixel */
                regbl_out.at<uchar>( regbl_in[regbl_i][regbl_j][1], regbl_in[regbl_i][regbl_j][0] ) = 0;

            }

        }

    }


/*
    source - filtering methods
 */

    regbl_component_t regbl_filter_pca( regbl_component_t & regbl_in, double const regbl_ratio ) {

        /* filtered connected components array */
        regbl_component_t filter;

        /* parsing components */
        for ( unsigned int i = 0; i < regbl_in.size() ; i ++ ) {

            /* prepare PCA matrix */
            cv::Mat regbl_pca( regbl_in[i].size(), 2, CV_64F );

            /* parsing components pixels */
            for ( unsigned int j = 0; j < regbl_in[i].size(); j ++ ) {

                /* compose PCA matrix */
                regbl_pca.at<double>(j,0) = regbl_in[i][j][0];
                regbl_pca.at<double>(j,1) = regbl_in[i][j][1];

            }

            /* compute PCA */
            cv::PCA pca_analysis( regbl_pca, cv::Mat(), cv::PCA::DATA_AS_ROW );

            /* apply filtering condition */
            if ( ( pca_analysis.eigenvalues.at<double>(1) / pca_analysis.eigenvalues.at<double>(0) ) > regbl_ratio ) {

                /* push unfiltered component */
                filter.push_back( regbl_in[i] );

            }

        }

        /* returned filtered array */
        return( filter );

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

        /* source image */
        cv::Mat regbl_source, regbl_binary, regbl_swap;

        /* state variable */
        int regbl_state( 1 );

        /* iteration parser */
        int regbl_flag( 0 );

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



        regbl_tool_equalize( regbl_source );

        /* check state specification */
        if ( regbl_state_path != NULL ) {

            /* export state */
            regbl_state = regbl_io_state( regbl_source, regbl_state, std::string( regbl_state_path ) );

        }


        /* black element extraction */
        //regbl_process_extract_black( regbl_source, regbl_binary, 96 );
        //regbl_process_extract_black( regbl_source, regbl_binary, 8 );
        regbl_process_extract_black( regbl_source, regbl_binary, 64 );

        /* check state specification */
        if ( regbl_state_path != NULL ) {

            /* export state */
            regbl_state = regbl_io_state( regbl_binary, regbl_state, std::string( regbl_state_path ) );

        }

        /* resize binary image */
        //cv::resize(regbl_binary, regbl_binary, cv::Size(), 1.5, 1.5, cv::INTER_NEAREST);
        cv::resize(regbl_binary, regbl_binary, cv::Size(), 2.0, 2.0, cv::INTER_NEAREST);

        /* check state specification */
        if ( regbl_state_path != NULL ) {

            /* export state */
            regbl_state = regbl_io_state( regbl_binary, regbl_state, std::string( regbl_state_path ) );

        }

        /* initialise conway state */
        regbl_flag = 1;

        /* apply conway */
        while ( regbl_flag > 0 ) {

            /* apply conway iteration and check results */
            if ( ( regbl_flag = regbl_process_conway_iteration( regbl_binary, regbl_swap, 4 ) ) > 0 ) {

                /* check state specification */
                if ( regbl_state_path != NULL ) {

                    /* export state */
                    regbl_state = regbl_io_state( regbl_swap, regbl_state, std::string( regbl_state_path ) );

                }

            }

            /* swap matrix */
            regbl_binary = regbl_swap;

        }

        /* extract connected component */
        regbl_component = regbl_extract_connected( regbl_binary );

        /* applying PCA filtering */
        regbl_component = regbl_filter_pca( regbl_component, 0.01 );

        /* render component on raster */
        regbl_process_component_raster( regbl_component, regbl_binary, cv::Size( regbl_binary.cols, regbl_binary.rows ) );

        /* check state specification */
        if ( regbl_state_path != NULL ) {

            /* export state */
            regbl_state = regbl_io_state( regbl_binary, regbl_state, std::string( regbl_state_path ) );

        }

        /* resize to original */
        cv::resize( regbl_binary, regbl_binary, cv::Size( regbl_source.cols, regbl_source.rows ), 0, 0, cv::INTER_NEAREST );

        /* export result image */
        cv::imwrite( regbl_output_path, regbl_binary );

        /* system message */
        return( 0 );

    }

