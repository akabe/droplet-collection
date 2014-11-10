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
#include "object-manager.hpp"
#include "object-ball.hpp"
#include "object-racket.hpp"

namespace DropletCollection
{
    namespace Object
    {
        const double Racket::m_margin = 5.0; // ラケットへの当たり判定における余裕
        const double Racket::m_curve_max = 80. * (M_PI / 180.);

        Racket::Racket (Manager& manager)
            : ObjectBase(manager, false),
              m_x(0.0),
              m_y(0.0),
              m_left(0.0),
              m_right(0.0),
              m_width(0.0),
              m_image(0)
        {
            if( register_class("Racket") )
            {
                register_function("Globule", 0);
                register_function("Ball",
                                  (CollideFunc) &Racket::collide_with_ball);
            }
        }

        Racket::~Racket ()
        {
            // Nothing to do.
        }

        void
        Racket::set_region (const double x_min,
                            const double x_max,
                            const double y)
        {
            dc_return_if_fail(fge(x_min, 0.0));
            dc_return_if_fail(fge(x_max, x_min));
            dc_return_if_fail(fge(y, 0.0));
            dc_return_if_fail(m_image != 0);

            m_x     = (x_min + x_max) / 2.0;
            m_y     = y;
            m_left  = x_min + m_width;
            m_right = x_max - m_width;
        }

        void
        Racket::load (const SettingFile& file)
        {
            double y, x_min, x_max;

            m_image = file.get_image(get_name(), "Image");
            m_width = m_image->get_width() / 2.0;

            x_min = file.get_double(get_name(), "XMin", 0, 65535);
            x_max = file.get_double(get_name(), "XMax", x_min, 65535);
            y     = file.get_double(get_name(), "Y"   , 0, 65535);

            set_region(x_min, x_max, y);
        }

        void
        Racket::draw (const Cairo::RefPtr<Cairo::Context>& cr) const
        {
            dc_return_if_fail(m_image != 0);
            dc_return_if_fail(cr != 0);

            Gdk::Cairo::set_source_pixbuf(cr, m_image, m_x - m_width, m_y);
            cr->paint();
        }

        bool
        Racket::collide_with_ball (Racket& self,
                                   Ball&   other)
        {
            const Vector<double>& p = other.get_position();

            if( self.m_y - p.get_y() < other.get_radius() )
            {
                if( !self.is_collided(other) )
                {
                    double distance = self.m_x - p.get_x();

                    if( fle(std::fabs(distance), self.m_width + m_margin) ) // ラケットにボールが当たった
                    {
                        Vector<double> v = other.get_velocity();
                        double ratio = distance / (self.m_width + m_margin);
                        double arg = -(M_PI / 2.) - m_curve_max * ratio; // ボールの反射角
                        v.set_arg(arg);
                        other.set_velocity(v);
                    }
                    else // ラケットからボールが外れた
                    {
                        other.set_velocity(Vector<double>(0.0, 0.0));
                        self.get_manager().set_state(Manager::STATE_GAMEOVER);
                    }
                }

                return true;
            }

            return false;
        }
    }
}
