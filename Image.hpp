/*
 *  HDRMerge - HDR exposure merging software.
 *  Copyright 2012 Javier Celaya
 *  jcelaya@gmail.com
 *
 *  This file is part of HDRMerge.
 *
 *  HDRMerge is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  HDRMerge is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with HDRMerge. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <memory>
#include "Array2D.hpp"


namespace hdrmerge {

class RawParameters;

class Image : public Array2D<uint16_t> {
public:
    static const int scaleSteps = 6;

    Image() : Array2D<uint16_t>() {}
    Image(uint16_t * rawImage, const RawParameters & params) {
        buildImage(rawImage, params);
    }
    Image(const Image & copy) = delete;
    Image & operator=(const Image & copy) = delete;
    Image(Image && move) {
        (*this) = std::move(move);
    }
    Image & operator=(Image && move);

    bool good() const {
        return width > 0;
    }
    double exposureAt(size_t x, size_t y) const {
        return (*this)(x, y) * relExp;
    }
    bool isSaturated(size_t x, size_t y) const {
        return (*this)(x, y) >= satThreshold;
    }
    bool isSaturatedAround(size_t x, size_t y) const;
    double getRelativeExposure() const {
        return relExp;
    }
    size_t alignWith(const Image & r);
    void releaseAlignData() {
        scaled.reset();
    }
    void relativeExposure(const Image & nextImage);
    bool operator<(const Image & r) {
        return brightness > r.brightness;
    }

private:
    std::unique_ptr<Array2D<uint16_t>[]> scaled;
    uint16_t max;
    uint16_t satThreshold;
    double brightness;
    double relExp;
    double halfLightPercent;

    void subtractBlack(const RawParameters & params);
    void buildImage(uint16_t * rawImage, const RawParameters & params);
    void preScale();
};

} // namespace hdrmerge

#endif // _IMAGE_H_
