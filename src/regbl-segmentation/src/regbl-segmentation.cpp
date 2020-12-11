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

    int regbl_io_state( cv::Mat & regbl_image, int regbl_state, std::string & regbl_path ) {

        /* compose exportation path */
        std::string regbl_export( regbl_path + "/state" + std::to_string( regbl_state ) + ".png" );

        /* export image */
        cv::imwrite( regbl_export.c_str(), regbl_image );

        /* update and return state */
        return( ++ regbl_state );

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

                    /* assign binary black */
                    regbl_out.at<uchar>(y,x) = 0;

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

    void regbl_process_pca_filtering( cv::Mat & regbl_in, cv::Mat & regbl_out ) {

        /* initialise output matrix */
        regbl_out = cv::Mat( cv::Size( regbl_in.cols, regbl_in.rows ), CV_8UC1, cv::Scalar(255));

        /* tracking matrix */
        cv::Mat regbl_track( cv::Size( regbl_in.cols, regbl_in.rows ), CV_8UC1, cv::Scalar(0));

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
                    int regbl_head( 0 );

                    /* component pixels gathering */
                    while ( regbl_head < regbl_component.size() ) {

                        /* check connectivitregbl_component[regbl_head][1] */
                        if ( ( regbl_in.at<uchar>(regbl_component[regbl_head][1]+1,regbl_component[regbl_head][0]) < 128 ) && ( regbl_track.at<uchar>(regbl_component[regbl_head][1]+1,regbl_component[regbl_head][0]) == 0 ) ) {

                            /* add piregbl_component[regbl_head][0]el to component */
                            regbl_component.push_back( std::vector< double > { double( regbl_component[regbl_head][0] ), double( regbl_component[regbl_head][1] + 1 ) } );

                            /* update tracking */
                            regbl_track.at<uchar>(regbl_component[regbl_head][1]+1,regbl_component[regbl_head][0]) = 255;

                        }

                        /* check connectivitregbl_component[regbl_head][1] */
                        if ( ( regbl_in.at<uchar>(regbl_component[regbl_head][1]-1,regbl_component[regbl_head][0]) < 128 ) && ( regbl_track.at<uchar>(regbl_component[regbl_head][1]-1,regbl_component[regbl_head][0]) == 0 ) ) {

                            /* add piregbl_component[regbl_head][0]el to component */
                            regbl_component.push_back( std::vector< double > { double( regbl_component[regbl_head][0] ), double( regbl_component[regbl_head][1] - 1 ) } );

                            /* update tracking */
                            regbl_track.at<uchar>(regbl_component[regbl_head][1]-1,regbl_component[regbl_head][0]) = 255;

                        }

                        /* check connectivitregbl_component[regbl_head][1] */
                        if ( ( regbl_in.at<uchar>(regbl_component[regbl_head][1],regbl_component[regbl_head][0]+1) < 128 ) && ( regbl_track.at<uchar>(regbl_component[regbl_head][1],regbl_component[regbl_head][0]+1) == 0 ) ) {

                            /* add piregbl_component[regbl_head][0]el to component */
                            regbl_component.push_back( std::vector< double > { double( regbl_component[regbl_head][0] + 1 ), double( regbl_component[regbl_head][1] ) } );

                            /* update tracking */
                            regbl_track.at<uchar>(regbl_component[regbl_head][1],regbl_component[regbl_head][0]+1) = 255;

                        }

                        /* check connectivitregbl_component[regbl_head][1] */
                        if ( ( regbl_in.at<uchar>(regbl_component[regbl_head][1],regbl_component[regbl_head][0]-1) < 128 ) && ( regbl_track.at<uchar>(regbl_component[regbl_head][1],regbl_component[regbl_head][0]-1) == 0 ) ) {

                            /* add piregbl_component[regbl_head][0]el to component */
                            regbl_component.push_back( std::vector< double > { double( regbl_component[regbl_head][0] - 1 ), double( regbl_component[regbl_head][1] ) } );

                            /* update tracking */
                            regbl_track.at<uchar>(regbl_component[regbl_head][1],regbl_component[regbl_head][0]-1) = 255;

                        }

                        /* update head */
                        regbl_head ++;

                    }

                    if ( regbl_component.size() > 16 ) {

                        for ( int i = 0; i < regbl_component.size(); i ++ ) {

                            regbl_out.at<uchar>(regbl_component[i][1],regbl_component[i][0]) = 127;

                        }

                    }

                }

            }

        }

    }

//cv::Mat tmp;
//while(std::getline(file, numStream))
//{
//    ...
//    cv::Mat m = cv::Mat(line).t();  // we need a row-vec
//    tmp.push_back(m);
//}
//cv::PCA pca(tmp, cv::Mat(), CV_PCA_DATA_AS_ROW, 2);


/*
    source - main function
 */

    int main( int argc, char ** argv ) {

        /* source image */
        cv::Mat regbl_source, regbl_binary, regbl_conway;

        /* state exportation path */
        std::string regbl_export( argv[2] );

        /* state variable */
        int regbl_state = 1;

        /* import source image */
        regbl_source = cv::imread( argv[1], cv::IMREAD_COLOR );

        regbl_state = regbl_io_state( regbl_source, regbl_state, regbl_export );

        regbl_process_extract_black( regbl_source, regbl_binary, 96 );

        cv::resize(regbl_binary, regbl_binary, cv::Size(), 1.5, 1.5, cv::INTER_NEAREST);

        regbl_state = regbl_io_state( regbl_binary, regbl_state, regbl_export );

        int i = 1;

        while ( i > 0 ) {

            if ( ( i = regbl_process_conway_iteration( regbl_binary, regbl_conway, 4 ) ) > 0 ) {

                regbl_state = regbl_io_state( regbl_conway, regbl_state, regbl_export );

            }

            regbl_binary = regbl_conway;

        }

        regbl_process_pca_filtering( regbl_binary, regbl_conway );

        regbl_state = regbl_io_state( regbl_conway, regbl_state, regbl_export );

        return( 0 );

    }

