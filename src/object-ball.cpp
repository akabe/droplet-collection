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
#include "object-ball.hpp"

namespace DropletCollection
{
    namespace Object
    {
        const double       Ball::m_min_vy = 2.0; // 速度ベクトルのY成分の最小値 (必ずボールが手元に戻てくるために)
        const unsigned int Ball::m_time_period = 20; // 速度調整周期

        Ball::Ball (Manager& manager)
            : Globule(manager),
              m_time_count(0),
              m_speed(0.0),
              m_image(0)
        {
            if( register_class("Ball") )
            {
                register_function("Globule",
                                  (CollideFunc) &Ball::collide_with_globule);
            }
        }

        Ball::~Ball ()
        {
            // Nothing to do.
        }

        void
        Ball::launch ()
        {
            set_velocity(Vector<double>(0.0, -m_speed));
        }

        void
        Ball::load (const SettingFile& file)
        {
            // 画像を取得する
            m_image = file.get_image(get_name(), "Image");
            set_radius((m_image->get_width() + m_image->get_height()) / 4.0);
            set_weight();

            // 速度の取得
            m_speed = file.get_double(get_name(), "Speed", 0, 65535);
            // 質量の取得
            if( file.has_key(get_name(), "Weight") )
            {
                set_weight(file.get_double(get_name(), "Weight", 0, 65535));
            }
        }

        void
        Ball::draw (const Cairo::RefPtr<Cairo::Context>& cr) const
        {
            dc_return_if_fail(cr != 0);
            dc_return_if_fail(m_image != 0);

            Gdk::Cairo::set_source_pixbuf(cr, m_image,
                                          get_position().get_x() - get_radius(),
                                          get_position().get_y() - get_radius());
            cr->paint();
        }

        void
        Ball::advance ()
        {
            if( ++m_time_count == m_time_period ) // 速度ベクトルの大きさを定期的に修正する
            {
                if( ! get_velocity().is_zero() ) set_velocity(get_velocity().set_size(m_speed));
                m_time_count = 0;
            }

            Globule::advance();
        }

        bool
        Ball::collide_with_globule (Ball&    self,
                                    Globule& other)
        {
            const double distance = self.get_distance(other);

            if( fge(distance, 0.0) )
            {
                Vector<double> norm, va, vb;
                decompose_velocity(self, other, norm, va, vb);

                if( !self.is_collided(other) ) // まだ衝突してないなら，非弾性衝突をする
                {
                    inelastic_collision(self, other, va, vb);
                    va = norm.nob_decompose(self.get_velocity());
                    va.set_x(-va.get_x());
                }
                else // 食い込み修正用の擬似衝突規則
                {
                    if( fiszero(va.get_x()) ) va.set_x(distance * self.get_restitution());
                    if( fiszero(vb.get_x()) ) vb.set_x(-distance * other.get_restitution());
                }

                self.set_velocity(norm.nob_compose(va));
                other.set_velocity(norm.nob_compose(vb));

                // 速度ベクトルの Y 成分の値を調整する
                Vector<double> new_v = self.get_velocity();
                if( std::fabs(new_v.get_y()) < m_min_vy ) // 衝突後の Y 座標変化が少なすぎる
                {
                    if( new_v.get_y() > 0.0 ) new_v.set_y(m_min_vy);
                    else                      new_v.set_y(-m_min_vy);
                    self.set_velocity(new_v);

                    dc_log("Ball の Y 座標変化量を増加させました");
                }

                return true;
            }

            return false;
        }
    }
}
