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

#ifndef INCLUDE_GUARD_54EBB0B9_39F5_453D_8AA2_E91B4860CE86
#define INCLUDE_GUARD_54EBB0B9_39F5_453D_8AA2_E91B4860CE86

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <gdkmm/pixbuf.h>
#include "vector.hpp"
#include "object-circle.hpp"

namespace DropletCollection
{
    namespace Object
    {
        class ForceField : public Circle
        {
            public:
                explicit ForceField (Manager&          manager);
                         ForceField (Manager&          manager,
                                     const ForceField& other);

                inline double                    get_force            () const;
                inline void                      set_force            (double force);
                inline Glib::RefPtr<Gdk::Pixbuf> get_image            () const;
                inline void                      set_image            (const Glib::RefPtr<Gdk::Pixbuf>& image);
                virtual void                     load                 (const SettingFile& file);
                virtual void                     draw                 (const Cairo::RefPtr<Cairo::Context>& cr) const;
                static bool                      collide_with_globule (const ForceField& self,
                                                                       Globule&          other);

            private:
                double                    m_force; // 中心点における速度加算量
                Glib::RefPtr<Gdk::Pixbuf> m_image;
        };

        inline double
        ForceField::get_force () const
        {
            return m_force;
        }

        inline void
        ForceField::set_force (const double force)
        {
            m_force = force;
        }

        inline Glib::RefPtr<Gdk::Pixbuf>
        ForceField::get_image () const
        {
            return m_image;
        }

        inline void
        ForceField::set_image (const Glib::RefPtr<Gdk::Pixbuf>& image)
        {
            m_image = image;
        }
    }
}


#endif /* ! INCLUDE_GUARD_54EBB0B9_39F5_453D_8AA2_E91B4860CE86 */
