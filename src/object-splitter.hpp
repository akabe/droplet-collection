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

#ifndef INCLUDE_GUARD_EF19FA07_8A9C_4BA8_A934_7427D2972EE1
#define INCLUDE_GUARD_EF19FA07_8A9C_4BA8_A934_7427D2972EE1

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <list>
#include "error.hpp"
#include "object-droplet.hpp"

namespace DropletCollection
{
    namespace Object
    {
        class Ball;

        class Splitter : public Droplet
        {
            public:
                explicit Splitter (Manager& manager);
                virtual ~Splitter ();

                void         add                  (Droplet * object);
                void         clear                ();
                virtual void load                 (const SettingFile& file);
                virtual void draw                 (const Cairo::RefPtr<Cairo::Context>& cr) const;
                static bool  collide_with_ball    (Splitter& self,
                                                   Ball&     other);
                static bool  collide_with_droplet (Splitter& self,
                                                   Droplet&  other);

            private:
                struct Segment
                {
                    Droplet      * droplet;
                    Vector<double> rel_pos;
                };

                void calc_relative_positions ();

                std::list<Segment> m_segm_list;
                bool m_collided;
        };
    }
}

#endif /* ! INCLUDE_GUARD_EF19FA07_8A9C_4BA8_A934_7427D2972EE1 */
