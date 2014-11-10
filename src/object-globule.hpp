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

#ifndef INCLUDE_GUARD_B8E0B0CD_C4C0_4DBC_9B81_0FA67885DE75
#define INCLUDE_GUARD_B8E0B0CD_C4C0_4DBC_9B81_0FA67885DE75

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include "vector.hpp"
#include "object-circle.hpp"

namespace DropletCollection
{
    namespace Object
    {
        class Globule : public Circle
        {
            public:
                explicit Globule (Manager&       manager);
                         Globule (Manager&       manager,
                                  const Globule& other);

                inline Vector<double> get_velocity         () const;
                inline void           set_velocity         (const Vector<double>& v);
                inline double         get_restitution      () const;
                inline void           set_restitution      (double rest);
                inline double         get_weight           () const;
                inline void           set_weight           ();
                inline void           set_weight           (double weight);
                virtual void          advance              ();
                virtual void          load                 (const SettingFile& file);
                static bool           collide_with_globule (Globule& self,
                                                            Globule& other);

            protected:
                static void   inelastic_collision  (Globule&        self,
                                                    Globule&        other,
                                                    Vector<double>& v_self,
                                                    Vector<double>& v_other);
                static void   decompose_velocity   (const Globule&  self,
                                                    const Globule&  other,
                                                    Vector<double>& norm,
                                                    Vector<double>& v_self,
                                                    Vector<double>& v_other);


            private:
                Vector<double> m_velocity;
                double         m_rest;
                double         m_weight;
        };

        inline Vector<double>
        Globule::get_velocity () const
        {
            return m_velocity;
        }

        inline void
        Globule::set_velocity (const Vector<double>& v)
        {
            m_velocity = v;
        }

        inline double
        Globule::get_restitution () const
        {
            return m_rest;
        }

        inline void
        Globule::set_restitution (const double rest)
        {
            m_rest = rest;
        }

        inline double
        Globule::get_weight () const
        {
            return m_weight;
        }

        inline void
        Globule::set_weight ()
        {
            m_weight = get_radius() * get_radius();
        }

        inline void
        Globule::set_weight (double weight)
        {
            m_weight = weight;
        }
    }
}

#endif /* ! INCLUDE_GUARD_B8E0B0CD_C4C0_4DBC_9B81_0FA67885DE75 */
