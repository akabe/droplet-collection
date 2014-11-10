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

#ifndef INCLUDE_GUARD_ED145F81_7D25_4FCD_978C_2D9D8B5E66C6
#define INCLUDE_GUARD_ED145F81_7D25_4FCD_978C_2D9D8B5E66C6

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include "error.hpp"
#include "object-manager.hpp"
#include "object-globule.hpp"

namespace DropletCollection
{
    namespace Object
    {
        class Droplet : public Globule
        {
            public:
                explicit Droplet (Manager&       manager);
                         Droplet (Manager&       manager,
                                  const Droplet& other);
                virtual ~Droplet ();

                inline int         get_group_id         () const;
                static Glib::Rand& get_random           ();
                void               set_random_velocity  ();
                void               set_random_velocity  (const Vector<double>& norm);
                virtual void       load                 (const SettingFile& file);
                virtual void       advance              ();
                virtual void       draw                 (const Cairo::RefPtr<Cairo::Context>& cr) const;
                static bool        collide_with_droplet (Droplet& self,
                                                         Droplet& other);

            protected:
                inline void set_group_id (int group_id);

            private:
                static Glib::Rand   m_rand;
                static bool         m_rand_inited;
                static const double m_init_speed_max;
                static const double m_visc_resist;
                static const int    m_effect_count_max;
                static const double m_effect_radius;

                mutable int m_effect_count;
                int m_group_id;
                Color m_bg_color;
                Color m_line_color;
                Color m_effect_color;
        };

        inline int
        Droplet::get_group_id () const
        {
            return m_group_id;
        }

        inline void
        Droplet::set_group_id (const int group_id)
        {
            m_group_id = group_id;
        }
    }
}

#endif /* ! INCLUDE_GUARD_ED145F81_7D25_4FCD_978C_2D9D8B5E66C6 */
