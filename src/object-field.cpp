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
#include "object-globule.hpp"
#include "object-field.hpp"

namespace DropletCollection
{
    namespace Object
    {
        Field::Field (Manager& manager)
            : ObjectBase(manager, false),
              m_left(0.0),
              m_right(0.0),
              m_top(0.0),
              m_bottom(0.0),
              m_image(0)
        {
            if( register_class("Field") )
            {
                register_function("Globule",
                                  (CollideFunc) &Field::collide_with_globule);
                register_function("Ball",
                                  (CollideFunc) &Field::collide_with_ball);
            }
        }

        Field::~Field ()
        {
            // Nothing to do.
        }

        void
        Field::set_region (const double x,
                           const double y,
                           const double width,
                           const double height)
        {
            m_left   = x;
            m_right  = x + width;
            m_top    = y;
            m_bottom = y + height;
        }

        void
        Field::load (const SettingFile& file)
        {
            double x, y, width, height;

            // 画像を取得する
            m_image = file.get_image(get_name(), "Image");

            // フィールド領域の取得
            x      = file.get_double(get_name(), "X"     , 0, 65535);
            y      = file.get_double(get_name(), "Y"     , 0, 65535);
            width  = file.get_double(get_name(), "Width" , 0, 65535);
            height = file.get_double(get_name(), "Height", 0, 65535);

            // フィールド領域の設定
            set_region(x, y, width, height);
        }

        void
        Field::draw (const Cairo::RefPtr<Cairo::Context>& cr) const
        {
            dc_return_if_fail(cr != 0);
            dc_return_if_fail(m_image != 0);

            Gdk::Cairo::set_source_pixbuf(cr, m_image, 0.0, 0.0);
            cr->paint();
        }

        bool
        Field::collide_with_globule (Field&   self,
                                     Globule& other)
        {
            const Vector<double>& p = other.get_position();
            const double radius = other.get_radius();
            bool res1, res2, res3, res4;

            res1 = collide_with_line(other,
                                     Vector<double>(1.0, 0.0),
                                     radius + self.m_left - p.get_x()); // 左端
            res2 = collide_with_line(other,
                                     Vector<double>(-1.0, 0.0),
                                     radius - self.m_right + p.get_x()); // 右端
            res3 = collide_with_line(other,
                                     Vector<double>(0.0, 1.0),
                                     radius + self.m_top - p.get_y()); // 上端
            res4 = collide_with_line(other,
                                     Vector<double>(0.0, -1.0),
                                     radius - self.m_bottom + p.get_y()); // 下端

            return res1 || res2 || res3 || res4;
        }

        bool
        Field::collide_with_ball (Field& self,
                                  Ball&  other)
        {
            const Vector<double>& p = other.get_position();
            const double radius = other.get_radius();
            bool res1, res2, res3;

            res1 = collide_with_line(other,
                                     Vector<double>(1.0, 0.0),
                                     radius + self.m_left - p.get_x()); // 左端
            res2 = collide_with_line(other,
                                     Vector<double>(-1.0, 0.0),
                                     radius - self.m_right + p.get_x()); // 右端
            res3 = collide_with_line(other,
                                     Vector<double>(0.0, 1.0),
                                     radius + self.m_top - p.get_y()); // 上端

            return res1 || res2 || res3;
        }

        bool
        Field::collide_with_line (Globule&              other,
                                  const Vector<double>& norm,
                                  const double          distance)
        {
            if( distance > 0.0 )
            {
                const double ip = Vector<double>::inner_product(norm, other.get_velocity());

                if( flt(ip, 0.0) ) // 非弾性衝突を計算する
                {
                    Vector<double> v = norm.nob_decompose(other.get_velocity());
                    v.set_x(-other.get_restitution() * v.get_x());
                    other.set_velocity(norm.nob_compose(v));
                }
                else if( feq(ip, 0.0) ) // 擬似非弾性衝突を計算する
                {
                    other.set_velocity(other.get_velocity() + distance * other.get_restitution() * norm);
                }

                return true;
            }

            return false;
        }
    }
}
