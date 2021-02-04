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

    /*! \file   regbl-segmentation.hpp
     *  \author Nils Hamel <nils.hamel@alumni.epfl.ch>
     *  \author Huriel Reichel
     *
     *  regbl-segmentation
     */

    /*! \mainpage regbl
     *
     *  \section license Copyright and License
     *
     *  **regbl** - Nils Hamel, Huriel Reichel <br >
     *  Copyright (c) 2020 Republic and Canton of Geneva
     *  
     *  This program is licensed under the terms of the GNU GPLv3. Documentation
     *  and illustrations are licensed under the terms of the CC BY 4.0.
     */

/*
    header - inclusion guard
 */

    # ifndef __REGBL_SEGMENTATION__
    # define __REGBL_SEGMENTATION__

/*
    header - internal includes
 */

/*
    header - external includes
 */

    # include <iostream>    
    # include <string>
    # include <common-include.hpp>
    # include <opencv2/core/core.hpp>
    # include <opencv2/highgui/highgui.hpp>
    # include <opencv2/imgproc/imgproc.hpp>

/*
    header - preprocessor definitions
 */

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

/*
    header - function prototypes
 */

    /*! \brief i/o methods
     *
     * This function allows to export the state of the processed image at each
     * step. A simple image, a state index and an exportation directory has to
     * be provided. The index is used to set a state number in the exportation
     * image name to keep them separated. The index is increased by the function
     * and returned.
     *
     * \param regbl_image Image to export
     * \param regbl_state State index value
     * \param regbl_path  Exportation directory path
     *
     * \return The function returns the provided state index increased by one.
     */

    int regbl_segmentation_io_state( cv::Mat & regbl_image, int regbl_state, std::string regbl_path );

    /*! \brief processing methods
     *
     * This function performs a standard equalization of the image. The process
     * takes place on the luma channel (Y of YUV). The function then starts by
     * converting the image from RGB to YUV.
     *
     * The luminance is standardiszed in the following way : considering M as
     * the channel mean value and S its standard deviation, the correction is as
     * follows :
     *
     *     Y' = ( ( Y - M ) / S ) * 128 + M
     *
     * The function then brings back the corrected YUV image in the RGB color
     * space. The whole process is made on the image passed as reference.
     *
     * \param regbl_in Image to be equalized.
     */

    void regbl_segmentation_tool_equalize( cv::Mat & regbl_in );

    /*! \brief processing methods
     *
     * This function allows to compute a binary image {0,255} that associate
     * a zero value to the pixel assumed to be black on the input image. The
     * computed binary image is returned as reference.
     *
     * The function starts by pre-selecting pixels that are sufficiently close
     * to zero. To do that, a simple L2 norm is used with a threshold value.
     * The pixel is pre-selected if on the input image, one has :
     *
     *     || ( R, G, B ) || < regbl_l2dist
     *
     * In such a case, the pixel color component are analyzed to check is they
     * are sufficiently gray (lack of color). This secondary selection is made
     * following the criterion :
     *
     *     max_{i,j,i!=j} | C_i - C-j | <= regbl_graylimit with C = R, G or B
     *
     * If the two previous criterion are verified, the pixel of the input image
     * is considered as black and the output image is updated accordingly.
     *
     * \param regbl_in        Input RGB image
     * \param regbl_out       Output binary image (single channel)
     * \param regbl_l2dist    L2 criterion threshold
     * \param regbl_graylimit Component similarity threshold
     */

    void regbl_segmentation_process_extract_black( cv::Mat & regbl_in, cv::Mat & regbl_out, int const regbl_l2dist, int const regbl_graylimit );

    /*! \brief processing methods
     *
     * This function implements a variation of the conway game of life as an
     * image processing step (morphological operator). The function expects a
     * single channel binary image {0,255} as input and return, by reference
     * a single channel binary image {0,255}.
     *
     * The process is implemented in the following way : each pixel of the input
     * is considered. The amount of zero-valued pixel in a area around the pixel
     * is computed and compared to the size, in pixels, of the area itself. The
     * following condition is applied :
     *
     *     black_pixel_count / total_pixel_count > regbl_gamevalue
     *
     * the corresponding pixel on the output image is set to zero (black) and
     * left at 255 otherwise. The output image has then the same size as the
     * input one.
     *
     * The area of search around each pixel is defined by plus and minus the
     * value of regbl_kernel.
     *
     * \param regbl_in        Input image, single channel binary image
     * \param regbl_out       Output image, single channel binary image
     * \param regbl_kernel    Half size, in pixels, of the searching area
     * \param regbl_gamevalue Conway variation game rule value (threshold)
     */

    void regbl_segmentation_process_conway_iteration( cv::Mat & regbl_in, cv::Mat & regbl_out, int const regbl_kernel, float const regbl_gamevalue );

    /*! \brief processing methods
     *
     * This function implements a building extraction procedure out of the
     * provided map, which has to be a single channel binary image {0,255} with 
     * potential building in black (0) and the background in white (255).
     *
     * Two input image are provided to the function : the cleaned map leaving
     * mostly the buildings and main structures (usually obtained after black
     * extraction and conway operator) and the source map containing all the
     * structures (usually obtained after black extraction only).
     *
     * The function starts by extracting connected black component out of the
     * cleaned input image. It then considers all black pixel of each component
     * with an area around each of them provided by plus and minus the value of
     * regbl_tolerence.
     *
     * The pixel of the output image, a single channel binary image {0,255} with
     * same size as the input images, are set based on the following criterion :
     *
     *     if the pixel is part of the connected component, the pixel is set to
     *     zero on the output image (the background being white).
     *     
     *     if the pixel is part of the area around a pixel of the connected 
     *     component, the function checks if the pixel is also black on the
     *     source image and set it on the output image in such case.
     *
     * This allows to regrows the building pre-extraction achieved using the
     * conway operator without taking risks. It also follows that this process
     * can not grows structure beyond the value provided by the regbl_tolerence
     * value.
     *
     * \param regbl_clean     Cleaned version of the source map
     * \param regbl_source    Binary (black and white) version of the source map
     * \param regbl_output    Output map
     * \param regbl_tolerence Size of the potential growing area
     */

    void regbl_segmentation_process_extract_building( cv::Mat & regbl_clean, cv::Mat & regbl_source, cv::Mat & regbl_output, int const regbl_tolerence );

    /*! \brief Main function
     *
     * This program allows to process a digitized map (developed and tested on
     * swisstopo national map) in order to extract only the buildings footprints
     * as pixels spots :
     * 
     *     ./regbl-segmentation --input/-i RGB source image
     *                          --output/-o building footprint image
     *                          --state/-s Processing step exportation
     *                          --invert/-v Image inversion flag
     *                          --equalize/-e Image equalization flag
     *
     * The source image, expected to be RGB is loaded in the first place and
     * inverted if the invert flag is set. The function also checks if the
     * equalization flag is set and equalize the RGB image accordingly. 
     *
     * The RGB image is then submitted to a black pixels extraction process that
     * uses two threshold to decide whether or not a pixel has to be considered
     * as black. The result is then a single channel binary image {0,255}.
     *
     * As building on maps are most of the time glued with other elements of the
     * map, such as roads, train tracks and other buildings, a conway based 
     * morphological operator is used to shrink the black structure, forcing
     * their separation.
     *
     * After this step, the building footprints are smaller than the original
     * buildings on the source map. A re-growing process is finally applied to
     * allow restoring building original size. This process uses the image
     * resulting of the black pixel extraction as a guide to avoid taking risk
     * on re-growing ghost structures.  
     *
     * The resulting image is then exported using the provided output path.
     *
     * The program allows to specify a directory path in which, when specified,
     * all the steps are exported as successive image. This allows to analyze in
     * detail the process to see what has gone wrong when needed.
     *
     * \param argc Standard parameter
     * \param argv Standard parameter
     *
     * \return Exit code.
     */

    int main( int argc, char ** argv );

/*
    header - inclusion guard
 */

    # endif

