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

#ifndef INCLUDE_GUARD_3AC8643C_5B3C_4430_BAD0_A4BD19EC503E
#define INCLUDE_GUARD_3AC8643C_5B3C_4430_BAD0_A4BD19EC503E

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <string>
#include <list>
#include <gdkmm/pixbuf.h>
#include <cairomm/cairomm.h>
#include "settingfile.hpp"

namespace DropletCollection
{
    namespace Object
    {
        class ObjectBase;
        class Field;
        class Racket;
        class Ball;

        class Manager
        {
            public:
                enum State
                {
                    STATE_STAND_BY,
                    STATE_PLAYING,
                    STATE_GAMEOVER,
                    STATE_GAMECLEAR
                };

                 Manager (const std::string& filename);
                ~Manager ();

                inline State  get_state   () const;
                inline void   set_state   (State state);
                double        get_width   () const;
                double        get_height  () const;
                void          move_racket (double x);
                void          launch      ();

                template <class ChildT>
                inline ChildT * attach     ();
                template <class ChildT>
                inline ChildT * attach     (const ChildT& other);
                bool            exists     (const ObjectBase& object) const;
                void            clear      ();
                void            initialize ();
                void            simulate   ();
                void            draw       (const Cairo::RefPtr<Cairo::Context>& cr) const;

            private:
                Manager (); // Do not call!
                Manager (const Manager& other); // Do not call!
                Manager& operator = (const Manager& other); // Do not call!

                void attach_internal            (ObjectBase * object);
                void initialize_optional_object (const std::string& class_name,
                                                 const std::string& object_name);
                bool check_gameclear            () const;

                SettingFile               m_file;
                State                     m_state;
                std::list<ObjectBase *>   m_fixed_list;
                std::list<ObjectBase *>   m_movable_list;
                Field                   * m_field;
                Racket                  * m_racket;
                Ball                    * m_ball;
                Glib::RefPtr<Gdk::Pixbuf> m_image_gameover;
                Glib::RefPtr<Gdk::Pixbuf> m_image_gameclear;
        };

        inline Manager::State
        Manager::get_state () const
        {
            return m_state;
        }

        inline void
        Manager::set_state (const State state)
        {
            m_state = state;
        }

        template <class ChildT>
        inline ChildT *
        Manager::attach ()
        {
            ChildT *p = new ChildT (*this);
            attach_internal(p);
            return p;
        }

        template <class ChildT>
        inline ChildT *
        Manager::attach (const ChildT& other)
        {
            ChildT *p = new ChildT (*this, other);
            attach_internal(p);
            return p;
        }
    }
}

#endif /* ! INCLUDE_GUARD_3AC8643C_5B3C_4430_BAD0_A4BD19EC503E */
