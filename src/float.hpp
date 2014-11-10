/* Droplet Collection - A trifling game.
 * Copyright (C) 2011  Akinori ABE
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INCLUDE_GUARD_588E9327_E8F6_46B6_AC1D_AEA80B5EAC5D
#define INCLUDE_GUARD_588E9327_E8F6_46B6_AC1D_AEA80B5EAC5D

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <cmath>

#define OC_FMIN 1e-6
#define OC_FMAX 10e10

#ifndef M_PI
#  define M_PI 3.14159265358979323846264338327950288
#endif

namespace DropletCollection
{
    /**
     * @brief Checks whether a floating-point value is 0.
     *
     * @param x A floating-point number.
     * @return  true if x is 0. Otherwise false is returned.
     */
    template <typename FloatT>
    static inline bool
    fiszero (const FloatT x)
    {
        return std::fabs(x) < OC_FMIN;
    }

    /**
     * @brief Checks whether 2 floating-point values are equal.
     *
     * @param lhs A floating-point number.
     * @param rhs A floating-point number.
     * @return    true if they are equal. Otherwise false is returned.
     */
    template <typename FloatT>
    static inline bool
    feq (const FloatT lhs,
         const FloatT rhs)
    {
        return fiszero(lhs - rhs);
    }

    /**
     * @brief Checks whether a floating-point value is less than another value.
     *
     * @param lhs A floating-point number.
     * @param rhs A floating-point number.
     * @return    true if lhs < rhs. Otherwise false is returned.
     */
    template <typename FloatT>
    static inline bool
    flt (const FloatT lhs,
         const FloatT rhs)
    {
        return lhs < rhs && !feq(lhs, rhs);
    }

    /**
     * @brief Checks whether a floating-point value is equal or less than another value.
     *
     * @param lhs A floating-point number.
     * @param rhs A floating-point number.
     * @return    true if lhs <= rhs. Otherwise false is returned.
     */
    template <typename FloatT>
    static inline bool
    fle (const FloatT lhs,
         const FloatT rhs)
    {
        return lhs < rhs || feq(lhs, rhs);
    }

    /**
     * @brief Checks whether a floating-point value is greater than another value.
     *
     * @param lhs A floating-point number.
     * @param rhs A floating-point number.
     * @return    true if lhs > rhs. Otherwise false is returned.
     */
    template <typename FloatT>
    static inline bool
    fgt (const FloatT lhs,
         const FloatT rhs)
    {
        return lhs > rhs && !feq(lhs, rhs);
    }

    /**
     * @brief Checks whether a floating-point value is equal or greater than another value.
     *
     * @param lhs A floating-point number.
     * @param rhs A floating-point number.
     * @return    true if lhs >= rhs. Otherwise false is returned.
     */
    template <typename FloatT>
    static inline bool
    fge (const FloatT lhs,
         const FloatT rhs)
    {
        return lhs > rhs || feq(lhs, rhs);
    }

    template <typename FloatT>
    static inline bool
    fbetw (const FloatT n,
           const FloatT min,
           const FloatT max)
    {
        return fgt(n, min) && flt(n, max);
    }

    template <typename FloatT>
    static inline bool
    fbetweq (const FloatT n,
             const FloatT min,
             const FloatT max)
    {
        return fge(n, min) && fle(n, max);
    }

    /**
     * @brief Checks whether a floating-point value is finite.
     *
     *  "finite" means that @c x is not NaN and between -OC_FMAX and OC_FMAX.
     * But this function can't check NaN if your @c cmath doesn't support
     * @c isfinite, @c isnan or @c fpclassify macro
     *
     * @param x A floating-point number.
     * @return  true if x is finite. Otherwise false is returned.
     */
    template <typename FloatT>
    static inline bool
    fisfinite (const FloatT x)
    {
#if defined(isfinite)
        return isfinite(x) && x > -OC_FMAX && x < OC_FMAX;
#elif defined(isnan)
        return isnan(x) && x > -OC_FMAX && x < OC_FMAX;
#elif defined(fpclassify)
        return fpclassify(x) != FP_NAN && x > -OC_FMAX && x < OC_FMAX;
#else
        return x > -OC_FMAX && x < OC_FMAX;
#endif
    }
}

#endif /* ! INCLUDE_GUARD_588E9327_E8F6_46B6_AC1D_AEA80B5EAC5D */
