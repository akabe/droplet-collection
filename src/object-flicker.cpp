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
#include "object-ball.hpp"
#include "object-flicker.hpp"

namespace DropletCollection
{
    namespace Object
    {
        const int    Flicker::m_effect_count_max = 20; // 衝突エフェクトを表示する時間
        const double Flicker::m_effect_motion    = 2.0; // 衝突エフェクトにおけるズレの大きさ

        Flicker::Flicker (Manager& manager)
            : Circle(manager, false),
              m_speed(1.0),
              m_image(0),
              m_effect_count(-1)
        {
            if( register_class("Flicker") )
            {
                register_function("Globule",
                                  (CollideFunc) &Flicker::collide_with_globule);
                register_function("Ball",
                                  (CollideFunc) &Flicker::collide_with_ball);
            }
        }

        Flicker::Flicker (Manager&       manager,
                          const Flicker& other)
            : Circle(manager, other),
              m_speed(other.m_speed),
              m_image(other.m_image),
              m_effect_count(-1)
        {
        }

        void
        Flicker::load (const SettingFile& file)
        {
            Circle::load(file);

            m_speed = file.get_double(get_name(), "Speed", 1, 255);
            m_image = file.get_image(get_name(), "Image");
        }

        void
        Flicker::draw (const Cairo::RefPtr<Cairo::Context>& cr) const
        {
            dc_return_if_fail(m_image != 0);
            dc_return_if_fail(cr != 0);

            Vector<double> p = get_position() - get_radius();

            if( m_effect_count >= 0 )
            {
                Glib::Rand& rand = get_random();

                p += Vector<double>(rand.get_double_range(-m_effect_motion,
                                                          +m_effect_motion),
                                    rand.get_double_range(-m_effect_motion,
                                                          +m_effect_motion));

                if( ++m_effect_count == m_effect_count_max ) m_effect_count = -1;
            }

            Gdk::Cairo::set_source_pixbuf(cr, m_image, p.get_x(), p.get_y());
            cr->paint();
        }

        bool
        Flicker::collide_with_globule (const Flicker& self,
                                       Globule&       other)
        {
            if( fge(self.get_distance(other), 0.0) )
            {
                if( ! self.is_collided(other) )
                {
                    Vector<double> norm = self.get_norm_to(other);
                    other.set_velocity(self.m_speed * norm);

                    self.m_effect_count = 0; // 衝突エフェクトの表示を始める
                }

                return true;
            }

            return false;
        }

        bool
        Flicker::collide_with_ball (const Flicker& self,
                                    Ball&          other)
        {
            if( fge(self.get_distance(other), 0.0) )
            {
                if( ! self.is_collided(other) )
                {
                    Vector<double> norm = self.get_norm_to(other);
                    other.set_velocity(other.get_speed() * norm);

                    self.m_effect_count = 0; // 衝突エフェクトの表示を始める
                }

                return true;
            }

            return false;
        }
    }
}
