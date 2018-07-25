/*

PICCANTE
The hottest HDR imaging library!
http://piccantelib.net

Copyright (C) 2014
Visual Computing Laboratory - ISTI CNR
http://vcg.isti.cnr.it
First author: Francesco Banterle

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.

*/

//This means that OpenGL acceleration layer is disabled
#define PIC_DISABLE_OPENGL

//This means we do not use QT for I/O
#define PIC_DISABLE_QT

#include "piccante.hpp"

int main(int argc, char *argv[])
{
    if(argc == 2) {
        img0_str = argv[1];
    } else {
        img0_str = "../data/input/cornellbox_mat_pt.pfm";
    }
    printf("Reading an HDR file...");

    pic::Image img;
    img.Read(img0_str);

    printf("Ok\n");

    printf("Is it valid? ");
    if(img.isValid()) {
        printf("Ok\n");

        printf("Filtering the image with a Gaussian filter with sigma_s = 4.0...");

        pic::Image *output = pic::FilterRemoveInfNaN::Execute(&img, NULL);
        pic::Image *output2 = pic::FilterRemoveNuked::Execute(output, NULL, 0.95f);

        printf("Ok!\n");

        printf("Writing the file to disk...");
        bool bWritten = output2->Write("../data/output/filtered_nuked_removed.pfm");

        if(bWritten) {
            printf("Ok\n");
        } else {
            printf("Writing had some issues!\n");
        }
    } else {
        printf("No, the file is not valid!\n");
    }

    return 0;
}
