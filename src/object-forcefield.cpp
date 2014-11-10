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
#include "object-globule.hpp"
#include "object-forcefield.hpp"

namespace DropletCollection
{
    namespace Object
    {
        ForceField::ForceField (Manager& manager)
            : Circle(manager, false),
              m_force(0.0),
              m_image(0)
        {
            if( register_class("ForceField") )
            {
                register_function("Globule",
                                  (CollideFunc) &ForceField::collide_with_globule);
            }
        }

        ForceField::ForceField (Manager&          manager,
                                const ForceField& other)
            : Circle(manager, other),
              m_force(other.m_force),
              m_image(other.m_image)
        {
        }

        void
        ForceField::load (const SettingFile& file)
        {
            Circle::load(file);

            m_force = file.get_double(get_name(), "Force", 0, 255);

            if( file.has_key(get_name(), "Image") )
            {
                m_image = file.get_image(get_name(), "Image");
            }
            else
            {
                m_image.clear();
            }
        }

        void
        ForceField::draw (const Cairo::RefPtr<Cairo::Context>& cr) const
        {
            dc_return_if_fail(m_image != 0);
            dc_return_if_fail(cr != 0);

            Gdk::Cairo::set_source_pixbuf(cr, m_image,
                                          get_position().get_x() - get_radius(),
                                          get_position().get_y() - get_radius());
            cr->paint();
        }

        bool
        ForceField::collide_with_globule (const ForceField& self,
                                          Globule&          other)
        {
            double distance = self.get_distance(other);

            if( fge(distance, 0.0) )
            {
                Vector<double> norm = (other.get_position() - self.get_position()).get_unit();
                double ratio = distance / (self.get_radius() + other.get_radius()); // 距離の割合 (中心で 1，円周上で 0)
                double force = self.m_force * ratio * ratio;

                other.set_velocity(other.get_velocity() + force * norm);
            }

            return false;
        }
    }
}
