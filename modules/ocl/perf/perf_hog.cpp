/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2010-2012, Multicoreware, Inc., all rights reserved.
// Copyright (C) 2010-2012, Advanced Micro Devices, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// @Authors
//    Fangfang Bai, fangfang@multicorewareinc.com
//    Jin Ma,       jin@multicorewareinc.com
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other oclMaterials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors as is and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/
#include "perf_precomp.hpp"

using namespace perf;

///////////// HOG////////////////////////

PERF_TEST(HOGFixture, HOG)
{
    Mat src = imread(getDataPath("gpu/hog/road.png"), cv::IMREAD_GRAYSCALE);
    ASSERT_TRUE(!src.empty()) << "can't open input image road.png";

    const std::string impl = getSelectedImpl();
    std::vector<cv::Rect> found_locations;
    declare.in(src).time(5);

    if (impl == "plain")
    {
        cv::HOGDescriptor hog;
        hog.setSVMDetector(hog.getDefaultPeopleDetector());

        TEST_CYCLE() hog.detectMultiScale(src, found_locations);

        SANITY_CHECK(found_locations, 1 + DBL_EPSILON);
    }
    else if (impl == "ocl")
    {
        cv::ocl::HOGDescriptor ocl_hog;
        ocl_hog.setSVMDetector(ocl_hog.getDefaultPeopleDetector());
        ocl::oclMat oclSrc(src);

        TEST_CYCLE() ocl_hog.detectMultiScale(oclSrc, found_locations);

        SANITY_CHECK(found_locations, 1 + DBL_EPSILON);
    }
#ifdef HAVE_OPENCV_GPU
    else if (impl == "gpu")
        CV_TEST_FAIL_NO_IMPL();
#endif
    else
        CV_TEST_FAIL_NO_IMPL();
}
