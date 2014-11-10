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

#ifndef INCLUDE_GUARD_71659B5A_D89E_4BA9_A309_03CF2AACC93E
#define INCLUDE_GUARD_71659B5A_D89E_4BA9_A309_03CF2AACC93E

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <gdkmm/pixbuf.h>
#include "object-circle.hpp"
#include "vector.hpp"

namespace DropletCollection
{
    namespace Object
    {
        class Globule;
        class Ball;

        class Flicker : public Circle
        {
            public:
                explicit Flicker (Manager&       manager);
                         Flicker (Manager&       manager,
                                  const Flicker& other);

                inline double                    get_speed            () const;
                inline void                      set_speed            (double speed);
                inline Glib::RefPtr<Gdk::Pixbuf> get_image            () const;
                inline void                      set_image            (const Glib::RefPtr<Gdk::Pixbuf>& image);
                virtual void                     load                 (const SettingFile& file);
                virtual void                     draw                 (const Cairo::RefPtr<Cairo::Context>& cr) const;
                static bool                      collide_with_globule (const Flicker& self,
                                                                       Globule&       other);
                static bool                      collide_with_ball    (const Flicker& self,
                                                                       Ball&          other);

            private:
                static const int          m_effect_count_max;
                static const double       m_effect_motion;

                double                    m_speed;
                Glib::RefPtr<Gdk::Pixbuf> m_image;
                mutable int               m_effect_count;
        };

        inline double
        Flicker::get_speed () const
        {
            return m_speed;
        }

        inline void
        Flicker::set_speed (const double speed)
        {
            m_speed = speed;
        }

        inline Glib::RefPtr<Gdk::Pixbuf>
        Flicker::get_image () const
        {
            return m_image;
        }

        inline void
        Flicker::set_image (const Glib::RefPtr<Gdk::Pixbuf>& image)
        {
            m_image = image;
        }
    }
}

#endif /* ! INCLUDE_GUARD_71659B5A_D89E_4BA9_A309_03CF2AACC93E */
