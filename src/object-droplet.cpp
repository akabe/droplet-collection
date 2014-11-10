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

#include <ctime>
#include "error.hpp"
#include "object-droplet.hpp"

namespace DropletCollection
{
    namespace Object
    {
        Glib::Rand   Droplet::m_rand;
        bool         Droplet::m_rand_inited      = false;
        const double Droplet::m_init_speed_max   = 3;
        const double Droplet::m_visc_resist      = 1e-3;
        const int    Droplet::m_effect_count_max = 20;
        const double Droplet::m_effect_radius    = 10.0;

        Droplet::Droplet (Manager& manager)
            : Globule(manager),
              m_effect_count(-1),
              m_group_id(0),
              m_bg_color(0.0, 0.0, 0.0),
              m_line_color(0.0, 0.0, 0.0),
              m_effect_color(0.0, 0.0, 0.0)
        {
            if( register_class("Droplet") )
            {
                register_function("Droplet",
                                  (CollideFunc) &Droplet::collide_with_droplet);
            }
        }

        Droplet::Droplet (Manager&       manager,
                          const Droplet& other)
            : Globule(manager, other),
              m_effect_count(-1),
              m_group_id(other.m_group_id),
              m_bg_color(other.m_bg_color),
              m_line_color(other.m_line_color),
              m_effect_color(other.m_effect_color)
        {
        }

        Droplet::~Droplet ()
        {
        }

        Glib::Rand&
        Droplet::get_random ()
        {
            if( m_rand_inited == false )
            {
                m_rand.set_seed(std::time(0));
                m_rand_inited = true;
            }

            return m_rand;
        }

        void
        Droplet::set_random_velocity ()
        {
            get_random();

            Vector<double> v(m_rand.get_double_range(-m_init_speed_max,
                                                     +m_init_speed_max),
                             m_rand.get_double_range(-m_init_speed_max,
                                                     +m_init_speed_max));

            set_velocity(v);
        }

        void
        Droplet::set_random_velocity (const Vector<double>& norm)
        {
            get_random();
            set_velocity(m_rand.get_double_range(0.0, m_init_speed_max) * norm);
        }

        void
        Droplet::advance ()
        {
            Vector<double> v = get_velocity();
            double k = m_visc_resist * get_radius();

            if( k > 1.0 )
            {
                dc_warning("粘性抵抗力が 1.0 を超過しました (k = %g, radius = %g)\n", k, get_radius());
                k = 1.0;
            }

            set_velocity(v - k * v);

            Globule::advance();
        }

        void
        Droplet::load (const SettingFile& file)
        {
            Globule::load(file);

            m_bg_color     = file.get_color(get_name(), "BgColor");
            m_line_color   = file.get_color(get_name(), "LineColor");
            m_effect_color = file.get_color(get_name(), "EffectColor");
            set_group_id(file.get_double(get_name(), "ID", 0, 255));

            // 速度ベクトルの取得
            if( file.has_key(get_name(), "Vx") && file.has_key(get_name(), "Vy") )
            {
                double vx, vy;
                vx = file.get_double(get_name(), "Vx", 0, 255);
                vy = file.get_double(get_name(), "Vy", 0, 255);
                set_velocity(Vector<double>(vx, vy));
            }
            else
            {
                set_random_velocity();
            }
        }

        void
        Droplet::draw (const Cairo::RefPtr<Cairo::Context>& cr) const
        {
            cr->set_source_rgba(m_bg_color.red,
                                m_bg_color.green,
                                m_bg_color.blue,
                                m_bg_color.alpha);
            cr->arc(get_position().get_x(),
                    get_position().get_y(),
                    get_radius(),
                    0.0, 2.0 * M_PI);
            cr->fill_preserve();
            cr->set_source_rgba(m_line_color.red,
                                m_line_color.green,
                                m_line_color.blue,
                                m_line_color.alpha);
            cr->stroke();

            // 吸収エフェクトを表示する
            if( m_effect_count >= 0 )
            {
                double alpha = (double)(m_effect_count_max - m_effect_count) / m_effect_count_max;

                cr->set_source_rgba(m_effect_color.red,
                                    m_effect_color.green,
                                    m_effect_color.blue,
                                    alpha);
                cr->arc(get_position().get_x(),
                        get_position().get_y(),
                        get_radius() + m_effect_radius * m_effect_count / m_effect_count_max,
                        0.0, 2.0 * M_PI);
                cr->stroke();

                if( m_effect_count == m_effect_count_max )
                {
                    m_effect_count = -1;
                }
                else
                {
                    ++m_effect_count;
                }
            }
        }

        bool
        Droplet::collide_with_droplet (Droplet& self,
                                       Droplet& other)
        {
            if( self.m_group_id == other.m_group_id ) // 同じグループなら小滴は合成する
            {
                if( fge(self.get_distance(other), 0.0) )
                {
                    double sum = self.get_weight() + other.get_weight();
                    double wa  = self.get_weight() / sum; // self の相対質量比
                    double wb  = other.get_weight() / sum; // other の相対質量比

                    self.set_position(wa * self.get_position() + wb * other.get_position());
                    self.set_velocity(wa * self.get_velocity() + wb * other.get_velocity());
                    self.set_radius(std::sqrt(self.get_radius() * self.get_radius() +
                                              other.get_radius() * other.get_radius())); // 面積が等しくなるように，半径を設定

                    self.m_effect_count = 0; // 吸収エフェクトの表示を開始する
                    other.set_alive(false); // other を削除する

                    return true;
                }

                return false;
            }
            else // グループが異なる小滴同士の衝突
            {
                return Globule::collide_with_globule(self, other);
            }
        }
    }
}
