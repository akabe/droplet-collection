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

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include "error.hpp"
#include "object-circle.hpp"

namespace DropletCollection
{
    namespace Object
    {
        Circle::Circle (Manager& manager,
                        bool     movable)
            : ObjectBase(manager, movable),
              m_position(0.0, 0.0),
              m_radius(0.0)
        {
            register_class("Circle");
        }

        Circle::Circle (Manager&      manager,
                        const Circle& other)
            : ObjectBase(manager, other),
              m_position(other.m_position),
              m_radius(other.m_radius)
        {
        }

        void
        Circle::load (const SettingFile& file)
        {
            m_position.set_x( file.get_double(get_name(), "X", 0, 65535) );
            m_position.set_y( file.get_double(get_name(), "Y", 0, 65535) );
            m_radius = file.get_double(get_name(), "Radius", 0, 65535);
        }

        double
        Circle::get_distance (const Circle& other) const
        {
            return m_radius + other.m_radius - m_position.distance(other.m_position);
        }

        Vector<double>
        Circle::get_norm_to (const Circle& other) const
        {
            return (other.m_position - m_position).get_unit();
        }

        Vector<double>
        Circle::get_norm_from (const Circle& other) const
        {
            return (m_position - other.m_position).get_unit();
        }
    }
}
