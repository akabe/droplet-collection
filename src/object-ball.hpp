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

#ifndef INCLUDE_GUARD_33EEC54B_74A0_421C_B74E_06EE44CDC185
#define INCLUDE_GUARD_33EEC54B_74A0_421C_B74E_06EE44CDC185

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <gdkmm.h>
#include "error.hpp"
#include "object-globule.hpp"

namespace DropletCollection
{
    namespace Object
    {
        class Ball : public Globule
        {
            public:
                explicit Ball (Manager& manager);
                virtual ~Ball ();

                inline double                    get_speed            () const;
                inline void                      set_speed            (double speed);
                inline Glib::RefPtr<Gdk::Pixbuf> get_image            () const;
                inline void                      set_image            (const Glib::RefPtr<Gdk::Pixbuf>& image);
                void                             launch               ();
                virtual void                     load                 (const SettingFile& file);
                virtual void                     draw                 (const Cairo::RefPtr<Cairo::Context>& cr) const;
                virtual void                     advance              ();
                static bool                      collide_with_globule (Ball&    self,
                                                                       Globule& other);

            private:
                static const double       m_min_vy;
                static const unsigned int m_time_period;

                unsigned int              m_time_count; //< 速度ベクトルの定期的修復に利用
                double                    m_speed;
                Glib::RefPtr<Gdk::Pixbuf> m_image;
        };

        inline double
        Ball::get_speed () const
        {
            return m_speed;
        }

        inline void
        Ball::set_speed (const double speed)
        {
            m_speed = speed;
        }

        inline Glib::RefPtr<Gdk::Pixbuf>
        Ball::get_image () const
        {
            return m_image;
        }

        inline void
        Ball::set_image (const Glib::RefPtr<Gdk::Pixbuf>& image)
        {
            m_image = image;
        }
    }
}

#endif /* ! INCLUDE_GUARD_33EEC54B_74A0_421C_B74E_06EE44CDC185 */
