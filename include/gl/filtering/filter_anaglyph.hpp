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

#ifndef PIC_GL_FILTERING_FILTER_ANAGLYPH_HPP
#define PIC_GL_FILTERING_FILTER_ANAGLYPH_HPP

#include "../../base.hpp"

#include "../../gl/filtering/filter.hpp"

namespace pic {

/**
 * @brief The FilterGLAnaglyph class
 */
class FilterGLAnaglyph: public FilterGL
{
protected:

    /**
     * @brief initShaders
     */
    void initShaders();

public:
    /**
     * @brief FilterGLAnaglyph
     */
    FilterGLAnaglyph();
};

PIC_INLINE FilterGLAnaglyph::FilterGLAnaglyph(): FilterGL()
{
    initShaders();
}

PIC_INLINE void FilterGLAnaglyph::initShaders()
{
    fragment_source = MAKE_STRING
                      (
                          uniform sampler2D u_texL; \n
                          uniform sampler2D u_texR; \n
                          out     vec4      f_color; \n

    void main(void) {
        \n
        ivec2 coords = ivec2(gl_FragCoord.xy);
        \n
        vec3  colL = texelFetch(u_texL, coords, 0).xyz;
        \n
        vec3  colR = texelFetch(u_texR, coords, 0).xyz;
        \n
        vec3  colA = vec3(colL.x, colR.y, colR.z);
        colA = pow(colA, vec3(0.45));
        f_color = vec4(colA, 1.0);
        \n
    }
                      );

    technique.initStandard("330", vertex_source, fragment_source, "FilterGLAnaglyph");

    technique.bind();
    technique.setUniform1i("u_texL", 0);
    technique.setUniform1i("u_texR", 1);
    technique.unbind();
}

} // end namespace pic

#endif /* PIC_GL_FILTERING_FILTER_ANAGLYPH_HPP */

