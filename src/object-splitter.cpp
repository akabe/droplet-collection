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

#include <cstddef>
#include "object-ball.hpp"
#include "object-splitter.hpp"

namespace DropletCollection
{
    namespace Object
    {
        Splitter::Splitter (Manager& manager)
            : Droplet(manager),
              m_collided(false)
        {
            if( register_class("Splitter") )
            {
                register_function("Ball",
                                  (CollideFunc) &Splitter::collide_with_ball);
                register_function("Droplet",
                                  (CollideFunc) &Splitter::collide_with_droplet);
            }
        }

        Splitter::~Splitter ()
        {
            clear();
        }

        void
        Splitter::add (Droplet * const object)
        {
            dc_return_if_fail(object != 0);

            Segment segm;
            segm.droplet = object;
            m_segm_list.push_back(segm);

            calc_relative_positions();
        }

        /**
         * @brief 内包する Droplet の相対位置を計算する
         */
        void
        Splitter::calc_relative_positions ()
        {
            std::size_t n_droplets = m_segm_list.size();
            double darg = 2.0 * M_PI / n_droplets; // それぞれの Droplet を配置するときの角度

            // 初期の法線ベクトルを決める
            Glib::Rand& rand = Droplet::get_random();
            Vector<double> norm(rand.get_double_range(-1.0, 1.0),
                                rand.get_double_range(-1.0, 1.0));
            norm.set_unit();

            // 各 Droplet の相対座標を求める
            std::list<Segment>::iterator it;
            for( it = m_segm_list.begin(); it != m_segm_list.end(); ++it )
            {
                // 相対位置座標を決める
                it->rel_pos = (get_radius() - it->droplet->get_radius()) * norm;

                // 速度は外向きランダム
                it->droplet->set_random_velocity(norm);

                norm.set_rotated(darg); // norm を darg だけ回転させる
            }
        }

        void
        Splitter::clear ()
        {
            std::list<Segment>::iterator it;

            for( it = m_segm_list.begin(); it != m_segm_list.end(); ++it )
            {
                delete it->droplet;
            }

            m_segm_list.clear();
        }

        void
        Splitter::load (const SettingFile& file)
        {
            Droplet::load(file);

            // 部品となる Droplet の情報を読み込む
            std::list<std::string> name_list = file.get_string_list(get_name(), "Parts");
            std::list<std::string>::iterator it;
            for( it = name_list.begin(); it != name_list.end(); ++it )
            {
                dc_log("物体 '%s'('Droplet') を 'Splitter' の部品として構築します", it->c_str());

                Droplet *dp = new Droplet (get_manager());
                dp->set_name(*it); // オブジェクト名を設定する
                dp->load(file); // データを読み込む

                add(dp);
            }
        }

        void
        Splitter::draw (const Cairo::RefPtr<Cairo::Context>& cr) const
        {
            std::list<Segment>::const_iterator it;

            Droplet::draw(cr);

            if( m_collided == false ) // 他の Droplet と合成していない場合にのみ，Splitter は機能する
            {
                for( it = m_segm_list.begin(); it != m_segm_list.end(); ++it )
                {
                    it->droplet->set_position(get_position() + it->rel_pos); // 相対座標で位置合わせ
                    it->droplet->draw(cr);
                }
            }
        }

        bool
        Splitter::collide_with_ball (Splitter& self,
                                     Ball&     other)
        {
            if( self.m_collided == false ) // Splitter としての効果を持っている
            {
                if( fge(self.get_distance(other), 0.0) )
                {
                    Vector<double> norm, va, vb;

                    // ボールは壁面弾性衝突で跳ね返す
                    decompose_velocity(self, other, norm, va, vb);
                    vb.set_x(-vb.get_x());
                    other.set_velocity(norm.nob_compose(vb));

                    // 部品を配置する
                    std::list<Segment>::iterator it;
                    for( it = self.m_segm_list.begin(); it != self.m_segm_list.end(); ++it )
                    {
                        it->droplet->set_position(self.get_position() + it->rel_pos); // 相対座標で位置合わせ
                        self.get_manager().attach(*(it->droplet)); // droplet のコピーを追加する
                    }

                    self.set_alive(false); // self を破棄する

                    return true;
                }

                return false;
            }
            else
            {
                return Ball::collide_with_globule(other, self);
            }
        }

        bool
        Splitter::collide_with_droplet (Splitter& self,
                                        Droplet&  other)
        {
            bool collided = Droplet::collide_with_droplet(self, other);

            // Splitter は他の Droplet と合成するとその効果を失う
            if( collided == true && self.get_group_id() == other.get_group_id() )
            {
                self.m_collided = true;
            }

            return collided;
        }
    }
}
