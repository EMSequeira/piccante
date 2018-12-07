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

#ifndef PIC_GL_FILTERING_FILTER_DOWN_PP_HPP
#define PIC_GL_FILTERING_FILTER_DOWN_PP_HPP

#include "../../gl/filtering/filter.hpp"

namespace pic {

/**
 * @brief The FilterGLDownPP class
 */
class FilterGLDownPP: public FilterGL
{
protected:

    float threshold, *value;

    /**
     * @brief initShaders
     */
    void initShaders();

public:
    /**
     * @brief FilterGLDownPP
     * @param scale
     */
    FilterGLDownPP(float *value, float threshold);    

    /**
     * @brief update
     * @param value
     * @param threshold
     */
    void update(float *value, float threshold);

    /**
     * @brief OutputSize
     * @param imgIn
     * @param width
     * @param height
     * @param channels
     * @param frames
     */
    void OutputSize(ImageGLVec imgIn, int &width, int &height, int &channels, int &frames)
    {
        width       = imgIn[0]->width >> 1;
        height      = imgIn[0]->height >> 1;
        channels    = imgIn[0]->channels;
        frames      = imgIn[0]->frames;
    }
};

PIC_INLINE FilterGLDownPP::FilterGLDownPP(float *value, float threshold): FilterGL()
{
    value = NULL;

    initShaders();
    update(value, threshold);
}

PIC_INLINE void FilterGLDownPP::initShaders()
{
    fragment_source = MAKE_STRING
                      (
                          uniform sampler2D u_tex; \n
                          uniform vec4    value; \n
                          uniform float   threshold; \n
                          out     vec4    f_color; \n

    void main(void) { \n
        ivec2 coords = ivec2(gl_FragCoord.xy) * ivec2(2); \n
        vec3 color[4]; \n
        color[0] = texelFetch(u_tex, coords              , 0).xyz; \n
        color[1] = texelFetch(u_tex, coords + ivec2(1, 0), 0).xyz; \n
        color[2] = texelFetch(u_tex, coords + ivec2(0, 1), 0).xyz; \n
        color[3] = texelFetch(u_tex, coords + ivec2(1, 1), 0).xyz; \n

        int counter = 0;
        vec3 ret = vec3(0.0);

        for(int i=0; i<4; i++) {
            if(distance(color[i], value.xyz) > threshold) {
                ret += color[i];
                counter++;
            }
        }

        if(counter > 0) {
            ret /= vec3(counter);
        } else {
            ret = value.xyz;
        }

        f_color = vec4(ret.xyz, 1.0); \n
    }
                      );

    technique.init("330", vertex_source, fragment_source);

#ifdef PIC_DEBUG
    technique.printLog("FilterGLDownPP");
#endif

    technique.bind();
    technique.setAttributeIndex("a_position", 0);
    technique.setOutputFragmentShaderIndex("f_color", 0);
    technique.link();
    technique.unbind();
}

/**
 * @brief update
 * @param value
 * @param threshold
 */
PIC_INLINE void FilterGLDownPP::update(float *value, float threshold)
{
    if(value == NULL) {
        this->value = new float[4];

        for(int i = 0; i < 4; i++) {
            this->value[i] = 0.0f;
        }
    } else {
        this->value = value;
    }

    if(threshold > 0.0f) {
        this->threshold = threshold;
    } else {
        this->threshold = 1e-4f;
    }

    technique.bind();
    technique.setUniform1i("u_tex", 0);
    technique.setUniform1f("threshold", this->threshold);
    technique.setUniform4fv("value", this->value);
    technique.unbind();
}

} // end namespace pic

#endif /* PIC_GL_FILTERING_FILTER_DOWN_PP_HPP */
