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

#ifndef INCLUDE_GUARD_6454D372_B900_4DBE_9073_762B529B07D3
#define INCLUDE_GUARD_6454D372_B900_4DBE_9073_762B529B07D3

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <cmath>
#include <ostream>
#include <glibmm.h>
#include "error.hpp"
#include "float.hpp"

namespace DropletCollection
{
    template <typename FloatT>
    class Vector
    {
        public:
            inline  Vector ();
            inline  Vector (const FloatT& x,
                            const FloatT& y);
            inline  Vector (const Vector<FloatT>& other);
            inline ~Vector ();

            template <typename OtherT>
            inline operator Vector<OtherT> () const;
            inline Vector<FloatT>& operator =       (const Vector<FloatT>& rhs);
            inline Vector<FloatT>& operator +=      (const FloatT& rhs);
            inline Vector<FloatT>& operator +=      (const Vector<FloatT>& rhs);
            inline Vector<FloatT>& operator -=      (const FloatT& rhs);
            inline Vector<FloatT>& operator -=      (const Vector<FloatT>& rhs);
            inline Vector<FloatT>& operator *=      (const FloatT& rhs);
            inline Vector<FloatT>& operator *=      (const Vector<FloatT>& rhs);
            inline Vector<FloatT>& operator /=      (const FloatT& rhs);
            inline Vector<FloatT>& operator /=      (const Vector<FloatT>& rhs);
            inline Vector<FloatT>  operator +       () const;
            inline Vector<FloatT>  operator +       (const FloatT& rhs) const;
            inline Vector<FloatT>  operator +       (const Vector<FloatT>& rhs) const;
            inline Vector<FloatT>  operator -       () const;
            inline Vector<FloatT>  operator -       (const FloatT& rhs) const;
            inline Vector<FloatT>  operator -       (const Vector<FloatT>& rhs) const;
            inline Vector<FloatT>  operator *       (const FloatT& rhs) const;
            inline Vector<FloatT>  operator *       (const Vector<FloatT>& rhs) const;
            inline Vector<FloatT>  operator /       (const FloatT& rhs) const;
            inline Vector<FloatT>  operator /       (const Vector<FloatT>& rhs) const;

            inline FloatT          get_x            () const;
            inline Vector<FloatT>& set_x            (const FloatT& x);
            inline FloatT          get_y            () const;
            inline Vector<FloatT>& set_y            (const FloatT& y);
            inline Vector<FloatT>& set_xy           (const FloatT& x,
                                                     const FloatT& y);
            inline FloatT          get_size2        () const;
            inline FloatT          get_size         () const;
            inline Vector<FloatT>& set_size         (const FloatT& size);
            inline FloatT          get_arg          () const;
            inline Vector<FloatT>& set_arg          (const FloatT& arg);
            inline Vector<FloatT>& set_polar        (const FloatT& size,
                                                     const FloatT& arg);

            inline bool            is_zero          () const;
            inline bool            is_unit          () const;
            inline Vector<FloatT>  get_unit         () const;
            inline Vector<FloatT>& set_unit         ();
            inline Vector<FloatT>  get_rotated      (const FloatT& arg) const;
            inline Vector<FloatT>& set_rotated      (const FloatT& arg);
            inline Vector<FloatT>  get_rotated_p90  () const;
            inline Vector<FloatT>  get_rotated_n90  () const;
            Vector<FloatT>         nob_decompose    (const Vector<FloatT>& vec) const;
            Vector<FloatT>         nob_compose      (const Vector<FloatT>& vec) const;
            Vector<FloatT>         wall_reflection  (const Vector<FloatT>& norm,
                                                     double                rest) const;
            FloatT                 distance2        (const Vector<FloatT>& other) const;
            FloatT                 distance         (const Vector<FloatT>& other) const;
            FloatT                 distance_to_line (const Vector<FloatT>& orientation,
                                                     const Vector<FloatT>& transit) const;
            Vector<FloatT>         vector_to_line   (const Vector<FloatT>& orientation,
                                                     const Vector<FloatT>& transit) const;

            static inline FloatT   inner_product    (const Vector<FloatT>& lhs,
                                                     const Vector<FloatT>& rhs);
            static inline FloatT   exterior_product (const Vector<FloatT>& lhs,
                                                     const Vector<FloatT>& rhs);
            static Vector<FloatT>  intersection     (const Vector<FloatT>& orientation1,
                                                     const Vector<FloatT>& transit1,
                                                     const Vector<FloatT>& orientation2,
                                                     const Vector<FloatT>& transit2);

        private:
            FloatT m_x;
            FloatT m_y;
    };

    template <typename FloatT>
    inline
    Vector<FloatT>::Vector ()
        : m_x(0), m_y(0)
    {
    }

    template <typename FloatT>
    inline
    Vector<FloatT>::Vector (const FloatT& x,
                            const FloatT& y)
        : m_x(x), m_y(y)
    {
    }

    template <typename FloatT>
    inline
    Vector<FloatT>::Vector (const Vector<FloatT>& other)
        : m_x(other.m_x), m_y(other.m_y)
    {
    }

    template <typename FloatT>
    inline
    Vector<FloatT>::~Vector ()
    {
        // Nothing to do
    }

    /////  Operators  /////////////////////////////////////////////////////////

    template <typename FloatT>
    template <typename OtherT>
    inline
    Vector<FloatT>::operator Vector<OtherT> () const
    {
        return Vector<OtherT>(m_x, m_y);
    }

    template <typename FloatT>
    inline Vector<FloatT>&
    Vector<FloatT>::operator = (const Vector<FloatT>& rhs)
    {
        m_x = rhs.get_x();
        m_y = rhs.get_y();
        return *this;
    }

    template <typename FloatT>
    inline Vector<FloatT>&
    Vector<FloatT>::operator += (const FloatT& rhs)
    {
        m_x += rhs;
        m_y += rhs;
        return *this;
    }

    template <typename FloatT>
    inline Vector<FloatT>&
    Vector<FloatT>::operator += (const Vector<FloatT>& rhs)
    {
        m_x += rhs.get_x();
        m_y += rhs.get_y();
        return *this;
    }

    template <typename FloatT>
    inline Vector<FloatT>&
    Vector<FloatT>::operator -= (const FloatT& rhs)
    {
        m_x -= rhs;
        m_y -= rhs;
        return *this;
    }

    template <typename FloatT>
    inline Vector<FloatT>&
    Vector<FloatT>::operator -= (const Vector<FloatT>& rhs)
    {
        m_x -= rhs.get_x();
        m_y -= rhs.get_y();
        return *this;
    }

    template <typename FloatT>
    inline Vector<FloatT>&
    Vector<FloatT>::operator *= (const FloatT& rhs)
    {
        m_x *= rhs;
        m_y *= rhs;
        return *this;
    }

    template <typename FloatT>
    inline Vector<FloatT>&
    Vector<FloatT>::operator *= (const Vector<FloatT>& rhs)
    {
        m_x *= rhs.get_x();
        m_y *= rhs.get_y();
        return *this;
    }

    template <typename FloatT>
    inline Vector<FloatT>&
    Vector<FloatT>::operator /= (const FloatT& rhs)
    {
        m_x /= rhs;
        m_y /= rhs;
        return *this;
    }

    template <typename FloatT>
    inline Vector<FloatT>&
    Vector<FloatT>::operator /= (const Vector<FloatT>& rhs)
    {
        m_x /= rhs.get_x();
        m_y /= rhs.get_y();
        return *this;
    }

    template <typename FloatT>
    inline Vector<FloatT>
    Vector<FloatT>::operator + () const
    {
        return *this;
    }

    template <typename FloatT>
    inline Vector<FloatT>
    Vector<FloatT>::operator + (const FloatT& rhs) const
    {
        return Vector<FloatT>(m_x + rhs, m_y + rhs);
    }

    template <typename FloatT>
    inline Vector<FloatT>
    Vector<FloatT>::operator + (const Vector<FloatT>& rhs) const
    {
        return Vector<FloatT>(m_x + rhs.get_x(), m_y + rhs.get_y());
    }

    template <typename FloatT>
    inline Vector<FloatT>
    Vector<FloatT>::operator - () const
    {
        return Vector<FloatT>(-m_x, -m_y);
    }

    template <typename FloatT>
    inline Vector<FloatT>
    Vector<FloatT>::operator - (const FloatT& rhs) const
    {
        return Vector<FloatT>(m_x - rhs, m_y - rhs);
    }

    template <typename FloatT>
    inline Vector<FloatT>
    Vector<FloatT>::operator - (const Vector<FloatT>& rhs) const
    {
        return Vector<FloatT>(m_x - rhs.get_x(), m_y - rhs.get_y());
    }

    template <typename FloatT>
    inline Vector<FloatT>
    Vector<FloatT>::operator * (const FloatT& rhs) const
    {
        return Vector<FloatT>(m_x * rhs, m_y * rhs);
    }

    template <typename FloatT>
    inline Vector<FloatT>
    Vector<FloatT>::operator * (const Vector<FloatT>& rhs) const
    {
        return Vector<FloatT>(m_x * rhs.get_x(), m_y * rhs.get_y());
    }

    template <typename FloatT>
    inline Vector<FloatT>
    Vector<FloatT>::operator / (const FloatT& rhs) const
    {
        return Vector<FloatT>(m_x / rhs, m_y / rhs);
    }

    template <typename FloatT>
    inline Vector<FloatT>
    Vector<FloatT>::operator / (const Vector<FloatT>& rhs) const
    {
        return Vector<FloatT>(m_x / rhs.get_x(), m_y / rhs.get_y());
    }

    /////  Exterior Operators  ////////////////////////////////////////////////

    /** @memberof Vector<Float> */
    template <typename LhsT, typename RhsT>
    static inline Vector<RhsT>
    operator + (const LhsT&         lhs,
                const Vector<RhsT>& rhs)
    {
        return Vector<RhsT>(lhs + rhs.get_x(), lhs + rhs.get_y());
    }

    /** @memberof Vector<Float> */
    template <typename LhsT, typename RhsT>
    static inline Vector<RhsT>
    operator - (const LhsT&         lhs,
                const Vector<RhsT>& rhs)
    {
        return Vector<RhsT>(lhs - rhs.get_x(), lhs - rhs.get_y());
    }

    /** @memberof Vector<Float> */
    template <typename LhsT, typename RhsT>
    static inline Vector<RhsT>
    operator * (const LhsT&         lhs,
                const Vector<RhsT>& rhs)
    {
        return Vector<RhsT>(lhs * rhs.get_x(), lhs * rhs.get_y());
    }

    /** @memberof Vector<Float> */
    template <typename LhsT, typename RhsT>
    static inline Vector<RhsT>
    operator / (const LhsT&         lhs,
                const Vector<RhsT>& rhs)
    {
        return Vector<RhsT>(lhs / rhs.get_x(), lhs / rhs.get_y());
    }

    /** @memberof Vector<Float> */
    template <typename charT, typename traitsT, typename FloatT>
    static inline std::basic_ostream<charT, traitsT>&
    operator << (std::basic_ostream<charT, traitsT>& lhs,
                 const Vector<FloatT>&               rhs)
    {
        lhs << "[" << rhs.get_x() << ", " << rhs.get_y() << "]";
        return lhs;
    }

    /////  Getters and Setters  ///////////////////////////////////////////////

    template <typename FloatT>
    inline FloatT
    Vector<FloatT>::get_x () const
    {
        return m_x;
    }

    template <typename FloatT>
    inline Vector<FloatT>&
    Vector<FloatT>::set_x (const FloatT& x)
    {
        m_x = x;
        return *this;
    }

    template <typename FloatT>
    inline FloatT
    Vector<FloatT>::get_y () const
    {
        return m_y;
    }

    template <typename FloatT>
    inline Vector<FloatT>&
    Vector<FloatT>::set_y (const FloatT& y)
    {
        m_y = y;
        return *this;
    }

    template <typename FloatT>
    inline Vector<FloatT>&
    Vector<FloatT>::set_xy (const FloatT& x,
                            const FloatT& y)
    {
        return ((*this) = Vector<FloatT>(x, y));
    }

    template <typename FloatT>
    inline FloatT
    Vector<FloatT>::get_size2 () const
    {
        return inner_product(*this, *this);
    }

    template <typename FloatT>
    inline FloatT
    Vector<FloatT>::get_size () const
    {
        return std::sqrt(get_size2());
    }

    template <typename FloatT>
    inline Vector<FloatT>&
    Vector<FloatT>::set_size (const FloatT& size)
    {
        dc_warn_if_fail(!is_zero());
        return ((*this) *= (size / get_size()));
    }

    template <typename FloatT>
    inline FloatT
    Vector<FloatT>::get_arg () const
    {
        return std::atan2(m_y, m_x);
    }

    template <typename FloatT>
    inline Vector<FloatT>&
    Vector<FloatT>::set_arg (const FloatT& arg)
    {
        return set_polar(get_size(), arg);
    }

    template <typename FloatT>
    inline Vector<FloatT>&
    Vector<FloatT>::set_polar (const FloatT& size,
                               const FloatT& arg)
    {
        return ((*this) = size * Vector<FloatT>(std::cos(arg), std::sin(arg)));
    }

    /////  Mathematical Methods  //////////////////////////////////////////////

    template <typename FloatT>
    inline bool
    Vector<FloatT>::is_zero () const
    {
        return feq<FloatT>(get_size2(), 0);
    }

    template <typename FloatT>
    inline bool
    Vector<FloatT>::is_unit () const
    {
        return feq<FloatT>(get_size2(), 1);
    }

    template <typename FloatT>
    inline Vector<FloatT>
    Vector<FloatT>::get_unit () const
    {
        return Vector(*this).set_unit();
    }

    template <typename FloatT>
    inline Vector<FloatT>&
    Vector<FloatT>::set_unit ()
    {
        return set_size(1);
    }

    template <typename FloatT>
    inline Vector<FloatT>
    Vector<FloatT>::get_rotated (const FloatT& arg) const
    {
        const Vector<FloatT> v(std::cos(arg), -std::sin(arg));
        return Vector(inner_product(*this, v),
                      inner_product(*this, v.get_rotated_p90()));
    }

    template <typename FloatT>
    inline Vector<FloatT>&
    Vector<FloatT>::set_rotated (const FloatT& arg)
    {
        return ((*this) = get_rotated(arg));
    }

    template <typename FloatT>
    inline Vector<FloatT>
    Vector<FloatT>::get_rotated_p90 () const
    {
        return Vector<FloatT>(-m_y, m_x);
    }

    template <typename FloatT>
    inline Vector<FloatT>
    Vector<FloatT>::get_rotated_n90 () const
    {
        return Vector<FloatT>(m_y, -m_x);
    }

    template <typename FloatT>
    Vector<FloatT>
    Vector<FloatT>::nob_decompose (const Vector<FloatT>& vec) const
    {
        dc_warn_if_fail(is_unit());
        return Vector(inner_product(vec, *this),
                      exterior_product(vec, *this));
    }

    template <typename FloatT>
    Vector<FloatT>
    Vector<FloatT>::nob_compose (const Vector<FloatT>& vec) const
    {
        dc_warn_if_fail(is_unit());
        return vec.m_x * (*this) + vec.m_y * get_rotated_n90();
    }

    /**
     * @brief 壁面で非弾性衝突した直後の速度を計算する．
     */
    template <typename FloatT>
    Vector<FloatT>
    Vector<FloatT>::wall_reflection (const Vector<FloatT>& norm,
                                     const double          rest) const
    {
        Vector<double> v = norm.nob_decompose(*this);
        v.m_x = -rest * v.m_x;
        return norm.nob_compose(v);
    }

    template <typename FloatT>
    FloatT
    Vector<FloatT>::distance2 (const Vector<FloatT>& other) const
    {
        return ((*this) - other).get_size2();
    }

    template <typename FloatT>
    FloatT
    Vector<FloatT>::distance (const Vector<FloatT>& other) const
    {
        return ((*this) - other).get_size();
    }

    template <typename FloatT>
    FloatT
    Vector<FloatT>::distance_to_line (const Vector<FloatT>& orientation,
                                      const Vector<FloatT>& transit) const
    {
        return std::fabs(exterior_product(orientation.get_unit(), transit - (*this)));
    }

    /**
     * @brief 直線へ向かうベクトルを計算する．
     *
     *  呼び出し元のベクトルの示す位置 S から方向ベクトル @c orientation，通過点 @c transit
     * で表される直線へ垂線を下ろしその交点を H とするとき，この関数は @f$ \overrightarrow{SH} @f$ を
     * 返す．
     * @f[
     *   \overrightarrow{SH}
     * = \overrightarrow{ST}
     * - \left( \frac{\vec{d} \cdot \overrightarrow{ST}}{|\vec{d}|^2} \right)
     *   \vec{d}
     * @f]
     *
     * @param orien 直線の任意の大きさの方向ベクトル．
     *              (上述の式における @f$ \vec{d} @f$)
     * @param trans 直線上の任意の点への位置ベクトル．
     *              (上述の式における @f$ \overrightarrow{OT} @f$)
     * @return      ベクトル @f$ \overrightarrow{SH} @f$ を返す．
     */
    template <typename FloatT>
    Vector<FloatT>
    Vector<FloatT>::vector_to_line (const Vector<FloatT>& orien,
                                    const Vector<FloatT>& trans) const
    {
        const Vector<FloatT> v = trans - (*this);
        return v - inner_product(orien, v) / orien.get_size2() * orien;
    }

    /**
     * @brief ベクトルの内積を計算する．
     *
     *  2次元のベクトルの内積は次のように定義されている．
     * @f[
     *   \vec{a} \cdot \vec{b} = a_x b_x + a_y b_y
     * @f]
     */
    template <typename FloatT>
    inline FloatT
    Vector<FloatT>::inner_product (const Vector<FloatT>& lhs,
                                   const Vector<FloatT>& rhs)
    {
        return lhs.m_x * rhs.m_x + lhs.m_y * rhs.m_y;
    }

    /**
     * @brief ベクトルの外積を計算する．
     *
     *  2次元のベクトルの外積は次のように定義されている．
     * @f[
     *   \vec{a} \times \vec{b} = a_x b_y - a_y b_x
     * @f]
     */
    template <typename FloatT>
    inline FloatT
    Vector<FloatT>::exterior_product (const Vector<FloatT>& lhs,
                                      const Vector<FloatT>& rhs)
    {
        return lhs.m_x * rhs.m_y - lhs.m_y * rhs.m_x;
    }

    /**
     * @brief Calculates an intersection point of 2 lines.
     *
     * This function uses the following formula to calculate the intersection
     * point @f$ \vec{P} @f$.
     * @f[
     *   \vec{P} = \frac{(\vec{D_1} \times \vec{T_1}) \vec{D_2}
     *                - (\vec{D_2} \times \vec{T_2}) \vec{D_1}}
     *                  {\vec{D_1} \times \vec{D_2}}
     * @f]
     *
     * @param orien1 An orientation vector of the 1st line. The length is optional.
     *               (@f$ \vec{D_1} @f$ at the above-mentioned formula.)
     * @param trans1 A position vector of an optional point on the 1st line.
     *               (@f$ \vec{T_1} @f$ at the above-mentioned formula.)
     * @param orien2 An orientation vector of the 2nd line. The length is optional.
     *               (@f$ \vec{D_2} @f$ at the above-mentioned formula.)
     * @param trans2 A position vector of an optional point on the 2nd line.
     *               (@f$ \vec{T_2} @f$ at the above-mentioned formula.)
     * @return       The position vector of the intersection point.
     *               (@f$ \vec{P} @f$ at the above-mentioned formula.)
     */
    template <typename FloatT>
    Vector<FloatT>
    Vector<FloatT>::intersection (const Vector<FloatT>& orien1,
                                  const Vector<FloatT>& trans1,
                                  const Vector<FloatT>& orien2,
                                  const Vector<FloatT>& trans2)
    {
        return (  exterior_product(orien1, trans1) * orien2
                - exterior_product(orien2, trans2) * orien1)
              / exterior_product(orien1, orien2);
    }
}

#endif /* ! INCLUDE_GUARD_6454D372_B900_4DBE_9073_762B529B07D3 */
