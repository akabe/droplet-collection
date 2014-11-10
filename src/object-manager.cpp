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
#include "object-objectbase.hpp"
#include "object-field.hpp"
#include "object-racket.hpp"
#include "object-ball.hpp"
#include "object-droplet.hpp"
#include "object-splitter.hpp"
#include "object-polygon.hpp"
#include "object-flicker.hpp"
#include "object-forcefield.hpp"
#include "object-manager.hpp"

namespace DropletCollection
{
    namespace Object
    {
        Manager::Manager (const std::string& filename)
            : m_state(STATE_STAND_BY),
              m_field(0),
              m_racket(0),
              m_ball(0),
              m_image_gameover(0),
              m_image_gameclear(0)
        {
            m_file.load_from_file(filename);

            m_image_gameover = Gdk::Pixbuf::create_from_file(
                Glib::build_filename(DC_DATADIR, "image/gameover.png")
            );
            m_image_gameclear = Gdk::Pixbuf::create_from_file(
                Glib::build_filename(DC_DATADIR, "image/gameclear.png")
            );
        }

        Manager::~Manager ()
        {
            clear();
        }

        double
        Manager::get_width () const
        {
            dc_return_val_if_fail(m_field != 0, 0.0);

            return m_field->get_width();
        }

        double
        Manager::get_height () const
        {
            dc_return_val_if_fail(m_field != 0, 0.0);

            return m_field->get_height();
        }

        void
        Manager::move_racket (const double x)
        {
            dc_return_if_fail(m_racket != 0);

            m_racket->set_x(x); // ラケットを移動する

            if( m_state == STATE_STAND_BY ) // まだボールは発射されていないなら，ボールも動かす
            {
                dc_return_if_fail(m_ball != 0);
                m_ball->set_position(Vector<double>(m_racket->get_x(),
                                                    m_racket->get_y() - (m_ball->get_radius() + 0.5)));
            }
        }

        void
        Manager::launch ()
        {
            dc_return_if_fail(m_state == STATE_STAND_BY);
            dc_return_if_fail(m_ball != 0);

            m_ball->launch();
            m_state = STATE_PLAYING;
        }

        void
        Manager::attach_internal (ObjectBase * const object)
        {
            dc_return_if_fail(object != 0);

            if( object->get_movable() )
            {
                m_movable_list.push_back(object);

                dc_log("movable 集合へ '%s' (クラス '%s') を追加しました",
                       object->get_name().c_str(),
                       object->get_class().c_str());
            }
            else
            {
                m_fixed_list.push_back(object);

                dc_log("fixed 集合へ '%s' (クラス '%s') を追加しました",
                       object->get_name().c_str(),
                       object->get_class().c_str());
            }
        }

        bool
        Manager::exists (const ObjectBase& object) const
        {
            std::list<ObjectBase *>::const_iterator it;

            if( object.get_movable() )
            {
                // movable 物体集合を検索する
                for( it = m_movable_list.begin(); it != m_movable_list.end(); ++it )
                    if( *it == &object )
                        return true;
            }
            else
            {
                // fixed 物体集合を検索する
                for( it = m_fixed_list.begin(); it != m_fixed_list.end(); ++it )
                    if( *it == &object )
                        return true;
            }

            return false;
        }

        /**
         * @brief 登録されている全ての物体を破棄する
         */
        void
        Manager::clear ()
        {
            std::list<ObjectBase *>::iterator it;

            // fixed 物体の削除
            for( it = m_fixed_list.begin(); it != m_fixed_list.end(); ++it ) delete (*it);

            // movable 物体の削除
            for( it = m_movable_list.begin(); it != m_movable_list.end(); ++it ) delete (*it);

            m_fixed_list.clear();
            m_movable_list.clear();

            m_field = 0;
            m_racket = 0;
            m_ball = 0;
        }

        void
        Manager::initialize ()
        {
            std::list<Glib::ustring>::iterator it;
            std::list<Glib::ustring> group_list;

            clear(); // 現在登録されている全ての要素を削除する
            m_state = STATE_STAND_BY; // 初期状態に戻す

            // フィールドを構築する
            m_field = attach<Field>();
            m_field->set_name("Field");
            m_field->load(m_file);

            // ラケットを構築する
            m_racket = attach<Racket>();
            m_racket->set_name("Racket");
            m_racket->load(m_file);

            // ボールを構築する
            m_ball = attach<Ball>();
            m_ball->set_name("Ball");
            m_ball->load(m_file);
            m_ball->set_position(Vector<double>(100, 500));

            move_racket(m_racket->get_x()); // ラケットとボールを初期位置へ移動する

            // その他の要素を構築する
            group_list = m_file.get_groups();
            for( it = group_list.begin(); it != group_list.end(); ++it )
            {
                if( m_file.has_key(*it, "Class") )
                {
                    const std::string class_name = m_file.get_string(*it, "Class");

                    dc_log("物体 '%s' を '%s' として構築します", it->c_str(), class_name.c_str());

                    initialize_optional_object(class_name, *it);
                }
            }
        }

        void
        Manager::initialize_optional_object (const std::string& class_name,
                                             const std::string& object_name)
        {
            if( class_name == "Droplet" )
            {
                Droplet *obj = attach<Droplet>();
                obj->set_name(object_name);
                obj->load(m_file);
            }
            else if( class_name == "Splitter" )
            {
                Splitter *obj = attach<Splitter>();
                obj->set_name(object_name);
                obj->load(m_file);
            }
            else if( class_name == "Polygon" )
            {
                Polygon *obj = attach<Polygon>();
                obj->set_name(object_name);
                obj->load(m_file);
            }
            else if( class_name == "Flicker" )
            {
                Flicker *obj = attach<Flicker>();
                obj->set_name(object_name);
                obj->load(m_file);
            }
            else if( class_name == "ForceField" )
            {
                ForceField *obj = attach<ForceField>();
                obj->set_name(object_name);
                obj->load(m_file);
            }
            else
            {
                throw Glib::KeyFileError(Glib::KeyFileError::INVALID_VALUE,
                                         "クラス '" + class_name + "' は定義されていません");
            }
        }

        void
        Manager::simulate ()
        {
            std::list<ObjectBase *>::iterator it, vit;

            // 各物体の時間を進める
            for( it = m_fixed_list.begin(); it != m_fixed_list.end(); ++it ) (*it)->advance();
            for( it = m_movable_list.begin(); it != m_movable_list.end(); ++it ) (*it)->advance();

            // movable-movable 間の衝突計算を行う
            for( it = m_movable_list.begin(); it != m_movable_list.end(); ++it )
            {
                if( (*it)->get_alive() )
                {
                    for( vit = it, ++vit; vit != m_movable_list.end(); ++vit )
                    {
                        if( (*vit)->get_alive() )
                        {
                            (*it)->collide(*(*vit));
                        }
                    }
                }
            }

            // fixed-movable 間の衝突計算を行う
            for( it = m_movable_list.begin(); it != m_movable_list.end(); ++it )
            {
                if( (*it)->get_alive() )
                {
                    for( vit = m_fixed_list.begin(); vit != m_fixed_list.end(); ++vit )
                    {
                        (*it)->collide(*(*vit));
                    }
                }
            }

            // 不要になった movable 物体を削除する
            for( it = m_movable_list.begin(); it != m_movable_list.end(); )
            {
                if( (*it)->get_alive() ) // オブジェクトが生存していれば，何もしない
                {
                    ++it;
                }
                else // オブジェクトが死亡している
                {
                    it = m_movable_list.erase(it); // オブジェクトを削除する
                }
            }

            // ゲームクリアの処理
            if( m_state == STATE_PLAYING && check_gameclear() ) // 各 ID の Droplet が全て集まった
            {
                m_state = STATE_GAMECLEAR;

                // ボールを削除する
                for( it = m_movable_list.begin(); it != m_movable_list.end(); ++it )
                {
                    if( *it == m_ball )
                    {
                        m_movable_list.erase(it); // オブジェクトを削除する
                        delete m_ball; m_ball = 0;
                        break;
                    }
                }

                dc_log("ユーザはゲームをクリアしました");
            }
        }

        bool
        Manager::check_gameclear () const
        {
            std::list<ObjectBase *>::const_iterator it, vit;

            for( it = m_movable_list.begin(); it != m_movable_list.end(); ++it )
            {
                if( ! (*it)->is("Droplet") ) continue; // Droplet でないなら，チェックは不要
                const Droplet *p1 = static_cast<const Droplet *>(*it);

                for( vit = it, ++vit; vit != m_movable_list.end(); ++vit )
                {
                    if( ! (*vit)->is("Droplet") ) continue; // Droplet でないなら，チェックは不要
                    const Droplet *p2 = static_cast<const Droplet *>(*vit);

                    // 同じグループIDの droplet が存在するなら，まだゲームはクリアしていない
                    if( p1->get_group_id() == p2->get_group_id() ) return false;
                }
            }

            return true;
        }

        void
        Manager::draw (const Cairo::RefPtr<Cairo::Context>& cr) const
        {
            std::list<ObjectBase *>::const_iterator it;

            // fixed 物体を描画する
            for( it = m_fixed_list.begin(); it != m_fixed_list.end(); ++it ) (*it)->draw(cr);

            // movable 物体を描画する
            for( it = m_movable_list.begin(); it != m_movable_list.end(); ++it ) (*it)->draw(cr);

            // ゲームオーバーの表示
            if( m_state == STATE_GAMEOVER )
            {
                dc_return_if_fail(m_image_gameover != 0);
                dc_return_if_fail(m_field != 0);

                const double width  = m_field->get_width(),
                             height = m_field->get_height();

                cr->set_source_rgba(0.0, 0.0, 0.0, 0.5);
                cr->rectangle(0.0, 0.0, width, height);
                cr->fill();

                Gdk::Cairo::set_source_pixbuf(cr,
                                              m_image_gameover,
                                              (width - m_image_gameover->get_width()) / 2.0,
                                              (height - m_image_gameover->get_height()) / 2.0);
                cr->paint();
            }
            // ゲームクリアの表示
            if( m_state == STATE_GAMECLEAR )
            {
                dc_return_if_fail(m_image_gameclear != 0);
                dc_return_if_fail(m_field != 0);

                const double width  = m_field->get_width(),
                             height = m_field->get_height();

                cr->set_source_rgba(1.0, 1.0, 1.0, 0.5);
                cr->rectangle(0.0, 0.0, width, height);
                cr->fill();

                Gdk::Cairo::set_source_pixbuf(cr,
                                              m_image_gameclear,
                                              (width - m_image_gameclear->get_width()) / 2.0,
                                              (height - m_image_gameclear->get_height()) / 2.0);
                cr->paint();
            }
        }
    }
}
