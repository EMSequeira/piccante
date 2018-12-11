/*

PICCANTE
The hottest HDR imaging library!
http://vcg.isti.cnr.it/piccante

Copyright (C) 2014
Visual Computing Laboratory - ISTI CNR
http://vcg.isti.cnr.it
First author: Francesco Banterle

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.

*/

#ifndef PIC_FILTERING_FILTER_SAMPLER_2DADD_HPP
#define PIC_FILTERING_FILTER_SAMPLER_2DADD_HPP

#include "../filtering/filter.hpp"

#include "../image_samplers/image_sampler.hpp"
#include "../image_samplers/image_sampler_bilinear.hpp"

namespace pic {

/**
 * @brief The FilterSampler2DAdd class
 */
class FilterSampler2DAdd: public Filter
{
protected:
    bool bIsb;
    ImageSampler *isb;

    /**
     * @brief ProcessBBox
     * @param dst
     * @param src
     * @param box
     */
    void ProcessBBox(Image *dst, ImageVec src, BBox *box)
    {
        int channels = dst->channels;

        Image *src0 = src[0];
        Image *src1 = src[1];

        float *tmp_mem = new float[channels << 1];

        float *vOut  = &tmp_mem[0];
        float *vsrc0 = &tmp_mem[channels];

        float inv_height1f = 1.0f / float(box->height - 1);
        float inv_width1f = 1.0f / float(box->width - 1);

        for(int j = box->y0; j < box->y1; j++) {
            float y = float(j) * inv_height1f;

            for(int i = box->x0; i < box->x1; i++) {
                float x = float(i) * inv_width1f;

                float *tmp_dst  = (*dst )(i, j);

                isb->SampleImage(src0, x, y, vsrc0);
                isb->SampleImage(src1, x, y, vOut);

                for(int k = 0; k < channels; k++) {
                    tmp_dst[k] = vsrc0[k] + vOut[k];
                }
            }
        }

        delete[] tmp_mem;
    }

public:

    /**
     * @brief FilterSampler2DAdd
     * @param isb
     */
    FilterSampler2DAdd(ImageSampler *isb) : Filter()
    {
        this->minInputImages = 2;

        if(isb != NULL) {
            bIsb = false;
            this->isb = isb;
        } else {
            bIsb = true;
            this->isb = new ImageSamplerBilinear();
        }
    }

    ~FilterSampler2DAdd()
    {
        if(bIsb) {
            delete isb;
        }
    }

    /**
     * @brief update
     * @param isb
     */
    void update(ImageSampler *isb)
    {
        if((this->isb != NULL) && (bIsb)) {
            delete this->isb;
        }

        this->isb = isb;
        bIsb = false;
    }

    /**
     * @brief execute
     * @param imgIn
     * @param imgOut
     * @param isb
     * @return
     */
    static Image *execute(Image *imgIn, Image *imgOut, ImageSampler *isb)
    {
        FilterSampler2DAdd filter(isb);
        return filter.Process(Single(imgIn), imgOut);
    }
};

} // end namespace pic

#endif /* PIC_FILTERING_FILTER_SAMPLER_2DADD_HPP */

