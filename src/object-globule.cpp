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

namespace DropletCollection
{
    namespace Object
    {
        Globule::Globule (Manager& manager)
            : Circle(manager, true),
              m_velocity(0.0, 0.0),
              m_rest(1.0),
              m_weight(0.0)
        {
            if( register_class("Globule") )
            {
                register_function("Globule",
                                  (CollideFunc) &Globule::collide_with_globule);
            }
        }

        Globule::Globule (Manager&       manager,
                          const Globule& other)
            : Circle(manager, other),
              m_velocity(other.m_velocity),
              m_rest(other.m_rest),
              m_weight(other.m_weight)
        {
        }

        void
        Globule::advance ()
        {
            set_position(get_position() + m_velocity);

            Circle::advance();
        }

        void
        Globule::load (const SettingFile& file)
        {
            Circle::load(file);
            set_weight();

            set_restitution(file.get_double(get_name(), "Restitution", 0, 1));
        }

        bool
        Globule::collide_with_globule (Globule& self,
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
                }
                else // 食い込み修正用の擬似衝突規則
                {
                    if( fiszero(va.get_x()) ) va.set_x(distance * self.get_restitution());
                    if( fiszero(vb.get_x()) ) vb.set_x(-distance * other.get_restitution());
                }

                self.set_velocity(norm.nob_compose(va));
                other.set_velocity(norm.nob_compose(vb));

                return true;
            }

            return false;
        }

        void
        Globule::inelastic_collision (Globule&        self,
                                      Globule&        other,
                                      Vector<double>& v_self,
                                      Vector<double>& v_other)
        {
            const double e = (self.m_rest + other.m_rest) / 2.0; // 反発係数 (平均)
            const double wa = self.m_weight  / (self.m_weight + other.m_weight); // 自分の質量比率
            const double wb = other.m_weight / (self.m_weight + other.m_weight); // 相手の質量比率

            const double p  = wa * v_self.get_x() + wb * v_other.get_x(); // 系の運動量総和 (衝突方向)
            const double dv = e * (v_other.get_x() - v_self.get_x()); // 衝突後の速度の差分 (衝突方向)

            v_self.set_x(p + wb * dv);
            v_other.set_x(p - wa * dv);
        }

        void
        Globule::decompose_velocity (const Globule&  self,
                                     const Globule&  other,
                                     Vector<double>& norm,
                                     Vector<double>& v_self,
                                     Vector<double>& v_other)
        {
            norm    = self.get_norm_from(other);
            v_self  = norm.nob_decompose(self.get_velocity());
            v_other = norm.nob_decompose(other.get_velocity());
        }
    }
}
