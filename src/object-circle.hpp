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

#ifndef INCLUDE_GUARD_41AAF713_15EB_4460_94EF_67488687032D
#define INCLUDE_GUARD_41AAF713_15EB_4460_94EF_67488687032D

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include "vector.hpp"
#include "object-objectbase.hpp"

namespace DropletCollection
{
    namespace Object
    {
        class Circle : public ObjectBase
        {
            public:
                explicit Circle (Manager&      manager,
                                 bool          movable);
                         Circle (Manager&      manager,
                                 const Circle& other);

                inline Vector<double> get_position () const;
                inline void           set_position (const Vector<double>& pos);
                inline double         get_radius   () const;
                inline void           set_radius   (double radius);
                virtual void          load         (const SettingFile& file);

            protected:
                double         get_distance  (const Circle& other) const;
                Vector<double> get_norm_to   (const Circle& other) const;
                Vector<double> get_norm_from (const Circle& other) const;

            private:
                Vector<double> m_position;
                double         m_radius;
        };

        inline Vector<double>
        Circle::get_position () const
        {
            return m_position;
        }

        inline void
        Circle::set_position (const Vector<double>& pos)
        {
            m_position = pos;
        }

        inline double
        Circle::get_radius () const
        {
            return m_radius;
        }

        inline void
        Circle::set_radius (const double radius)
        {
            m_radius = radius;
        }
    }
}

#endif /* ! INCLUDE_GUARD_41AAF713_15EB_4460_94EF_67488687032D */
