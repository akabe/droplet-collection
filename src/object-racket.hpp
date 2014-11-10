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

#ifndef INCLUDE_GUARD_7E904304_C379_48C5_9F27_906E2BFDD1E5
#define INCLUDE_GUARD_7E904304_C379_48C5_9F27_906E2BFDD1E5

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <gdkmm.h>
#include "object-objectbase.hpp"

namespace DropletCollection
{
    namespace Object
    {
        class Ball;

        class Racket : public ObjectBase
        {
            public:
                explicit Racket (Manager& manager);
                virtual ~Racket ();

                inline double get_x             () const;
                inline void   set_x             (double x);
                inline double get_y             () const;
                void          set_region        (double x_min,
                                                 double x_max,
                                                 double y);
                virtual void  load              (const SettingFile& file);
                virtual void  draw              (const Cairo::RefPtr<Cairo::Context>& cr) const;
                static bool   collide_with_ball (Racket& self,
                                                 Ball&   other);

            private:

                static const double m_margin;
                static const double m_curve_max;

                double m_x;
                double m_y;
                double m_left;
                double m_right;
                double m_width;
                Glib::RefPtr<Gdk::Pixbuf> m_image;
        };

        inline double
        Racket::get_x () const
        {
            return m_x;
        }

        inline void
        Racket::set_x (const double x)
        {
            if( m_left > x )       m_x = m_left;
            else if( m_right < x ) m_x = m_right;
            else                   m_x = x;
        }

        inline double
        Racket::get_y () const
        {
            return m_y;
        }
    }
}

#endif /* ! INCLUDE_GUARD_7E904304_C379_48C5_9F27_906E2BFDD1E5 */
