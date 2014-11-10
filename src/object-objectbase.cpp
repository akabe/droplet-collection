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
#include "object-objectbase.hpp"

namespace DropletCollection
{
    namespace Object
    {
        std::bitset<ObjectBase::N_OBJECTS> ObjectBase::m_object_flags(0);

        ObjectBase::ObjectBase (Manager&   manager,
                                const bool movable)
            : m_class(0),
              m_manager(manager),
              m_id(get_object_id()),
              m_movable(movable),
              m_alive(true),
              m_name("")
        {
        }

        ObjectBase::ObjectBase (Manager&          manager,
                                const ObjectBase& other)
            : m_class(other.m_class),
              m_manager(manager),
              m_id(get_object_id()),
              m_movable(other.m_movable),
              m_alive(other.m_alive),
              m_name(other.m_name)
        {
        }

        ObjectBase::~ObjectBase ()
        {
            m_object_flags &= ~m_id; // 使用中のオブジェクト ID を使用可能に設定する
            dc_log("オブジェクト '%s' を解放しました", m_name.c_str());
        }

        std::bitset<ObjectBase::N_OBJECTS>
        ObjectBase::get_object_id ()
        {
            for( std::size_t i = 0; i < N_OBJECTS; ++i )
            {
                if( ! m_object_flags.test(i) ) // i ビット目を使用することができる
                {
                    m_object_flags.set(i); // i ビット目を使用中に設定する

                    std::bitset<N_OBJECTS> id;
                    id.set(i);
                    return id; // i ビット目のみが 1 のビット列をオブジェクトIDとして返す
                }
            }

            dc_log("割り当て可能なオブジェクト ID が存在しません");
            std::exit(EXIT_FAILURE);
        }

        void
        ObjectBase::advance ()
        {
            m_prev_collided = m_curr_collided;
            m_curr_collided = 0;
        }

        void
        ObjectBase::collide (ObjectBase& other)
        {
            std::list<VTable>::const_iterator it;
            unsigned int score, max_score = 0;
            const VTable *vtable = 0;
            bool matched, reversed = false, vt_reversed = false;

            dc_return_if_fail(get_alive());
            dc_return_if_fail(other.get_alive());
            dc_return_if_fail(m_class != 0);
            dc_return_if_fail(other.m_class != 0);

            // スコアが最大となる衝突規則を検索する
            for( it = m_vtable_list.begin(); it != m_vtable_list.end(); ++it )
            {
                if( (m_class->flags & it->self.mask).any() &&
                    (other.m_class->flags & it->other.mask).any() )
                {
                    reversed = false;
                    matched  = true;
                }
                else if( (m_class->flags & it->other.mask).any() &&
                         (other.m_class->flags & it->self.mask).any() )
                {
                    reversed = true;
                    matched  = true;
                }
                else
                {
                    matched = false;
                }

                if( matched )
                {
                    // 派生クラスの末端側ほどスコアが高くなる
                    score = it->self.flags.count() + it->other.flags.count();
                    if( score > max_score )
                    {
                        vt_reversed = reversed;
                        max_score = score;
                        vtable = &(*it);
                    }
                }
            }

            if( vtable != 0 ) // 衝突規則が見つかった
            {
                if( vtable->collide == 0 ) return; // 決して衝突しない物体間の規則

                bool res;
                if( vt_reversed ) res = ( *(vtable->collide) )(other, *this);
                else              res = ( *(vtable->collide) )(*this, other);

                if( res )
                {
                    // 衝突した物体の ID を記憶しておく
                    m_curr_collided       |= other.m_id;
                    other.m_curr_collided |= m_id;
                }
            }
            else
            {
                dc_error("衝突規則が定義されていません '%s' <=> '%s'",
                         m_class->name.c_str(), other.m_class->name.c_str());
            }
        }

        /**********************************************************************
         *  派生クラスの管理
         *********************************************************************/

        std::list<ObjectBase::ClassInfo>   ObjectBase::m_class_list;
        std::list<ObjectBase::VTable>      ObjectBase::m_vtable_list;
        std::bitset<ObjectBase::N_CLASSES> ObjectBase::m_class_mask(1);

        bool
        ObjectBase::is (const std::string& class_name) const
        {
            dc_return_val_if_fail(m_class != 0, false);

            ClassInfo *info = find_class_info(class_name);

            return info != 0 && (m_class->flags & info->mask).any();
        }

        bool
        ObjectBase::register_class (const std::string& self_class)
        {
            ClassInfo *info = find_class_info(self_class);
            bool newly_registered = false;

            if( info == 0 ) // クラスは未登録
            {
                dc_log("未登録のクラス '%s' を登録しました", self_class.c_str());
                newly_registered = true;
                info = add_class_info(self_class);
            }
            else if( ! info->mask.any() ) // クラスは仮登録
            {
                dc_log("仮登録のクラス '%s' を本登録しました", self_class.c_str());
                newly_registered = true;
            }

            if( newly_registered )
            {
                info->mask  = m_class_mask;
                info->flags = m_class_mask;
                if( m_class != 0 ) info->flags |= m_class->flags;
                m_class_mask <<= 1;

                std::cerr << "\tmask  = " << info->mask << std::endl;
                std::cerr << "\tflags = " << info->flags << std::endl;
            }

            m_class = info;

            return newly_registered;
        }

        void
        ObjectBase::register_function (const std::string& other_class,
                                       const CollideFunc  collide_func)
        {
            ClassInfo *info;

            dc_return_if_fail(m_class != 0);

            info = find_class_info(other_class); // 衝突相手のクラスを探す
            if( info == 0 ) info = add_class_info(other_class); // 衝突相手のクラスが未登録なら，仮登録する

            // VTable を登録する
            VTable vtable = {*m_class, *info, collide_func};
            m_vtable_list.push_back(vtable);

            dc_log("衝突規則を登録しました '%s' <=> '%s'",
                   m_class->name.c_str(), info->name.c_str());
        }

        ObjectBase::ClassInfo *
        ObjectBase::find_class_info (const std::string& name)
        {
            std::list<ObjectBase::ClassInfo>::iterator it;

            for( it = m_class_list.begin(); it != m_class_list.end(); ++it )
                if( it->name == name )
                    return &(*it);

            return 0;
        }

        ObjectBase::ClassInfo *
        ObjectBase::add_class_info (const std::string& name)
        {
            ClassInfo info = {name, 0, 0};

            m_class_list.push_back(info);

            return &( m_class_list.back() );
        }

        /**********************************************************************
         *  乱数の提供
         *********************************************************************/

        Glib::Rand ObjectBase::m_rand;
        bool       ObjectBase::m_rand_inited = false;

        Glib::Rand&
        ObjectBase::get_random ()
        {
            if( m_rand_inited == false )
            {
                m_rand.set_seed(std::time(0));
                m_rand_inited = true;
            }

            return m_rand;
        }
    }
}
