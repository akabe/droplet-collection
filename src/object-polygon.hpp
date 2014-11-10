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

#ifndef INCLUDE_GUARD_A10FBBA7_E597_451B_80E8_E3323B132E2D
#define INCLUDE_GUARD_A10FBBA7_E597_451B_80E8_E3323B132E2D

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <list>
#include <gdkmm/pixbuf.h>
#include "object-objectbase.hpp"
#include "vector.hpp"

namespace DropletCollection
{
    namespace Object
    {
        class Globule;

        class Polygon : public ObjectBase
        {
            public:
                explicit Polygon (Manager&       manager);
                         Polygon (Manager&       manager,
                                  const Polygon& other);
                virtual ~Polygon ();

                inline bool                      get_show_outline     () const;
                inline void                      set_show_outline     (bool show_outline);
                inline Glib::RefPtr<Gdk::Pixbuf> get_image            () const;
                inline void                      set_image            (const Glib::RefPtr<Gdk::Pixbuf>& image);
                bool                             set_points           (const std::list<Vector<double> >& points,
                                                                       const Vector<double>              origin);
                virtual void                     load                 (const SettingFile& file);
                virtual void                     draw                 (const Cairo::RefPtr<Cairo::Context>& cr) const;
                static bool                      collide_with_globule (const Polygon& self,
                                                                       Globule&       other);

            private:
                struct LineSegment
                {
                    Vector<double> bp;
                    Vector<double> ep;
                    Vector<double> norm;
                };

                void calc_bounding_volume      ();
                void load_points               (const SettingFile&    file,
                                                const Glib::ustring&  key,
                                                const Vector<double>& origin);
                bool collide_with_line_segment (Globule&           other,
                                                const LineSegment& segm) const;

                std::list<LineSegment>    m_segm_list;
                Vector<double>            m_position; //< 座標原点・画像の表示位置
                Vector<double>            m_center; //< 重心座標
                double                    m_radius; //< バウンディングボリュームの半径
                bool                      m_show_outline;
                Glib::RefPtr<Gdk::Pixbuf> m_image;
        };

        inline bool
        Polygon::get_show_outline () const
        {
            return m_show_outline;
        }

        inline void
        Polygon::set_show_outline (const bool show_outline)
        {
            m_show_outline = show_outline;
        }

        inline Glib::RefPtr<Gdk::Pixbuf>
        Polygon::get_image () const
        {
            return m_image;
        }

        inline void
        Polygon::set_image (const Glib::RefPtr<Gdk::Pixbuf>& image)
        {
            m_image = image;
        }
    }
}

#endif /* ! INCLUDE_GUARD_A10FBBA7_E597_451B_80E8_E3323B132E2D */
