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

#ifndef INCLUDE_GUARD_EC538ACC_203D_44C9_907A_01D2B4A51D96
#define INCLUDE_GUARD_EC538ACC_203D_44C9_907A_01D2B4A51D96

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <list>
#include <string>
#include <bitset>
#include <glibmm/random.h>
#include <cairomm/cairomm.h>
#include "settingfile.hpp"

namespace DropletCollection
{
    namespace Object
    {
        class Manager;

        class ObjectBase
        {
            public:
                         ObjectBase (Manager&          manager,
                                     bool              movable);
                         ObjectBase (Manager&          manager,
                                     const ObjectBase& other);
                virtual ~ObjectBase ();

                static Glib::Rand&        get_random   ();
                inline std::string        get_class    () const;
                inline Manager&           get_manager  ();
                inline const Manager&     get_manager  () const;
                inline bool               get_movable  () const;
                inline bool               get_alive    () const;
                inline const std::string& get_name     () const;
                inline void               set_name     (const std::string& name);
                bool                      is           (const std::string& class_name) const;
                inline bool               is_collided  (const ObjectBase& other) const;

                // For DropletCollection::Object::Manager
                virtual void load    (const SettingFile& file) = 0;
                virtual void draw    (const Cairo::RefPtr<Cairo::Context>& cr) const = 0;
                virtual void advance ();
                void         collide (ObjectBase& other);

            protected:
                typedef bool (* CollideFunc) (ObjectBase& self,
                                              ObjectBase& other);

                inline void set_alive         (bool alive);
                bool        register_class    (const std::string& self_class);
                void        register_function (const std::string& other_class,
                                               CollideFunc        collide_func);

            private:
                enum { N_CLASSES = 32 };
                enum { N_OBJECTS = 128 }; // 同時に構築できるオブジェクト数の上限値

                struct ClassInfo
                {
                    std::string            name;
                    std::bitset<N_CLASSES> mask;
                    std::bitset<N_CLASSES> flags;
                };

                struct VTable
                {
                    const ClassInfo& self;
                    const ClassInfo& other;
                    CollideFunc      collide;
                };

                ObjectBase (); // Do not call!
                ObjectBase (const ObjectBase& other); // Do not call!
                ObjectBase& operator = (const ObjectBase& other); // Do not call!

                static ClassInfo            * find_class_info (const std::string& name);
                static ClassInfo            * add_class_info  (const std::string& name);
                static std::bitset<N_OBJECTS> get_object_id   ();

                static std::list<ClassInfo>   m_class_list;
                static std::list<VTable>      m_vtable_list;
                static std::bitset<N_CLASSES> m_class_mask;
                static std::bitset<N_OBJECTS> m_object_flags; //使用中のオブジェクトIDに対応するビットは1になる
                static Glib::Rand             m_rand;
                static bool                   m_rand_inited;

                ClassInfo                  * m_class;
                Manager&                     m_manager;
                const std::bitset<N_OBJECTS> m_id;
                std::bitset<N_OBJECTS>       m_prev_collided;
                std::bitset<N_OBJECTS>       m_curr_collided;
                const bool                   m_movable;
                bool                         m_alive;
                std::string                  m_name;
        };

        inline
        std::string
        ObjectBase::get_class () const
        {
            dc_return_val_if_fail(m_class != 0, "");

            return m_class->name;
        }

        inline
        Manager&
        ObjectBase::get_manager ()
        {
            return m_manager;
        }

        inline const Manager&
        ObjectBase::get_manager () const
        {
            return m_manager;
        }

        inline bool
        ObjectBase::get_movable () const
        {
            return m_movable;
        }

        inline bool
        ObjectBase::get_alive () const
        {
            return m_alive;
        }

        inline void
        ObjectBase::set_alive (const bool alive)
        {
            m_alive = alive;
        }

        inline const std::string&
        ObjectBase::get_name () const
        {
            return m_name;
        }

        inline void
        ObjectBase::set_name (const std::string& name)
        {
            m_name = name;
        }

        inline bool
        ObjectBase::is_collided (const ObjectBase& other) const
        {
            return (m_prev_collided & other.m_id).any();
        }
    }
}

#endif /* ! INCLUDE_GUARD_EC538ACC_203D_44C9_907A_01D2B4A51D96 */
