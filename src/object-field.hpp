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

#ifndef INCLUDE_GUARD_D3613EFB_3365_4995_9CA5_C51AA40E55B2
#define INCLUDE_GUARD_D3613EFB_3365_4995_9CA5_C51AA40E55B2

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <gdkmm.h>
#include "vector.hpp"
#include "object-objectbase.hpp"

namespace DropletCollection
{
    namespace Object
    {
        class Ball;
        class Globule;

        class Field : public ObjectBase
        {
            public:
                explicit Field (Manager& manager);
                virtual ~Field ();

                inline double get_width            () const;
                inline double get_height           () const;
                void          set_region           (double x,
                                                    double y,
                                                    double width,
                                                    double height);
                virtual void  load                 (const SettingFile& file);
                virtual void  draw                 (const Cairo::RefPtr<Cairo::Context>& cr) const;
                static bool   collide_with_globule (Field&   self,
                                                    Globule& other);
                static bool   collide_with_ball    (Field&   self,
                                                    Ball&    other);

            private:
                static bool collide_with_line    (Globule&              other,
                                                  const Vector<double>& norm,
                                                  double                distance);

                double m_left;
                double m_right;
                double m_top;
                double m_bottom;
                Glib::RefPtr<Gdk::Pixbuf> m_image;
        };

        inline double
        Field::get_width () const
        {
            dc_return_val_if_fail(m_image != 0, 0.0);

            return m_image->get_width();
        }

        inline double
        Field::get_height () const
        {
            dc_return_val_if_fail(m_image != 0, 0.0);

            return m_image->get_height();
        }
    }
}

#endif /* ! INCLUDE_GUARD_D3613EFB_3365_4995_9CA5_C51AA40E55B2 */
