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
#include "object-polygon.hpp"

namespace DropletCollection
{
    namespace Object
    {
        Polygon::Polygon (Manager& manager)
            : ObjectBase(manager, false),
              m_position(0.0, 0.0),
              m_center(0.0, 0.0),
              m_radius(0.0),
              m_show_outline(false),
              m_image(0)
        {
            if( register_class("Polygon") )
            {
                register_function("Globule",
                                  (CollideFunc) &Polygon::collide_with_globule);
            }
        }

        Polygon::Polygon (Manager&       manager,
                          const Polygon& other)
            : ObjectBase(manager, other),
              m_segm_list(other.m_segm_list),
              m_position(other.m_position),
              m_center(other.m_center),
              m_radius(0.0),
              m_image(other.m_image)
        {
        }

        Polygon::~Polygon ()
        {
            // Nothing to do.
        }

        bool
        Polygon::set_points (const std::list< Vector<double> >& points,
                             const Vector<double>               origin)
        {
            std::list<Vector<double> >::const_iterator p1, p2, p3;
            Vector<double> n0, n1, n2;
            LineSegment lseg;

            g_return_val_if_fail(points.size() >= 3, false);

            m_segm_list.clear(); // 古いデータを除去する
            m_radius = 0.0;

            m_position = origin; // 原点座標を設定

            // 線分列に変換する
            --(p2 = points.end()); // 最後の要素
            --(p1 = p2); // 最後の1つ前の要素
            for( p3 = points.begin(); p3 != points.end(); n0 = n2, p1 = p2, p2 = p3, ++p3 )
            {
                dc_log("Polygon: P1:[%g,%g]; P2:[%g,%g]; P3:[%g,%g]",
                       p1->get_x(), p1->get_y(),
                       p2->get_x(), p2->get_y(),
                       p3->get_x(), p3->get_y());

                n1 = p3->vector_to_line(*p2 - *p1, *p1); // P1->P2 に対する法線ベクトル
                n2 = p1->vector_to_line(*p3 - *p2, *p2); // P2->P3 に対する法線ベクトル

                dc_log("Polygon: n0:[%g,%g]; n1:[%g,%g]; n2:[%g,%g]",
                       n0.get_x(), n0.get_y(),
                       n1.get_x(), n1.get_y(),
                       n2.get_x(), n2.get_y());

                if( p3 != points.begin() && fle(Vector<double>::inner_product(n0, n1), 0.0) ) // n0 が定義されている
                {
                    dc_error("Polygon: 法線ベクトルが矛盾しています Failed 'n0.n1 > 0'");
                    m_segm_list.clear();
                    return false;
                }

                lseg.bp   = *p2 + origin;
                lseg.ep   = *p3 + origin;
                lseg.norm = n2;
                m_segm_list.push_back(lseg);

                dc_log("Polygon: append LineSegment {bp:[%g,%g]; ep:[%g,%g]; norm:[%g,%g]}",
                       lseg.bp.get_x(), lseg.bp.get_y(),
                       lseg.ep.get_x(), lseg.ep.get_y(),
                       lseg.norm.get_x(), lseg.norm.get_y());
            }

            calc_bounding_volume(); // バウンディングボリュームを計算する

            return true;
        }

        void
        Polygon::calc_bounding_volume ()
        {
            std::list<LineSegment>::const_iterator it;
            Vector<double> sum(0.0, 0.0);

            // 重心を計算する
            for( it = m_segm_list.begin(); it != m_segm_list.end(); ++it ) sum += it->bp;
            m_center = sum / m_segm_list.size();

            // バウンディングボリュームの半径を計算する
            double distance2, max_distance2 = 0.0;
            for( it = m_segm_list.begin(); it != m_segm_list.end(); ++it )
            {
                distance2 = m_center.distance2(it->bp);
                if( max_distance2 < distance2 ) max_distance2 = distance2;
            }
            m_radius = std::sqrt(max_distance2); // 重心と各点の距離の最大値が半径となる

            dc_log("Polygon: 重心:[%g,%g]; 半径:%g",
                   m_center.get_x(), m_center.get_y(), m_radius);
        }

        void
        Polygon::load (const SettingFile& file)
        {
            Vector<double> origin;

            m_show_outline = file.get_boolean(get_name(), "ShowOutline");

            origin.set_x( file.get_double(get_name(), "X", 0, 65535) );
            origin.set_y( file.get_double(get_name(), "Y", 0, 65535) );
            load_points(file, "Points", origin);

            if( file.has_key(get_name(), "Image") )
            {
                m_image = file.get_image(get_name(), "Image");
            }
            else
            {
                m_image.clear();
            }
        }

        void
        Polygon::load_points (const SettingFile&    file,
                              const Glib::ustring&  key,
                              const Vector<double>& origin)
        {
            std::list<double> values = file.get_double_list(get_name(), key, 0, 65535);
            std::list<double>::const_iterator it_x, it_y;

            if( values.size() % 2 == 1 ) // 値の個数が奇数である
            {
                throw Glib::KeyFileError(Glib::KeyFileError::INVALID_VALUE,
                                         Glib::ustring::compose("キー '%s' の値は偶数個でなければなりません",
                                                                key));
            }

            // 点列を構成する
            std::list<Vector<double> > points;
            for( it_x = values.begin(); it_x != values.end(); ++it_x, ++it_x )
            {
                ++(it_y = it_x);

                points.push_back(Vector<double>(*it_x, *it_y));
            }

            // 点列を設定する
            if( ! set_points(points, origin) )
            {
                throw Glib::KeyFileError(Glib::KeyFileError::INVALID_VALUE,
                                         Glib::ustring::compose("キー '%1' の点列は凸多角形を成している必要があります",
                                                                key));
            }
        }

        void
        Polygon::draw (const Cairo::RefPtr<Cairo::Context>& cr) const
        {
            if( m_image ) // 画像を表示する
            {
                Gdk::Cairo::set_source_pixbuf(cr, m_image, m_position.get_x(), m_position.get_y());
                cr->paint();
            }

            if( m_show_outline ) // 輪郭線を表示する
            {
                std::list<LineSegment>::const_iterator it;

                cr->set_source_rgb(0.0, 0.0, 0.0);

                for( it = m_segm_list.begin(); it != m_segm_list.end(); ++it )
                {
                    cr->move_to(it->bp.get_x(), it->bp.get_y());
                    cr->line_to(it->ep.get_x(), it->ep.get_y());
                }

                cr->stroke();

                // バウンディングボリュームの表示
                cr->set_source_rgb(0.5, 0.5, 0.5);
                cr->arc(m_center.get_x(), m_center.get_y(), m_radius, 0.0, 2.0 * M_PI);
                cr->stroke();
            }
        }

        bool
        Polygon::collide_with_globule (const Polygon& self,
                                       Globule&       other)
        {
            Vector<double> orien = self.m_center - other.get_position();

            if( fle(orien.get_size(), self.m_radius + other.get_radius()) ) // バウンディングボリュームとの衝突判定
            {
                std::list<LineSegment>::const_iterator it;
                double h;

                for( it = self.m_segm_list.begin(); it != self.m_segm_list.end(); ++it )
                {
                    h = Vector<double>::exterior_product(orien, it->bp - other.get_position())
                      / Vector<double>::exterior_product(orien, it->bp - it->ep);
                    if( fbetweq(h, 0.0, 1.0) )
                    {
                        if( self.collide_with_line_segment(other, *it) )
                        {
                            return true;
                        }
                    }
                }
            }

            return false;
        }

        bool
        Polygon::collide_with_line_segment (Globule&           other,
                                            const LineSegment& segm) const
        {
            Vector<double> dcp = other.get_position().vector_to_line(segm.bp - segm.ep, segm.bp);
            Vector<double> cp = dcp + other.get_position(); // 衝突点の位置ベクトル
            Vector<double> vb = cp - segm.bp, ve = cp - segm.ep;

            // 衝突点が辺の中に収まっている場合
            if( fle(Vector<double>::inner_product(vb, ve), 0.0) )
            {
                double distance = other.get_radius() - dcp.get_size();
                if( fge(distance, 0.0) ) // 衝突の条件を満たした
                {
                    if( ! is_collided(other) )
                    {
                        Vector<double> norm = dcp.get_unit();
                        Vector<double> v = norm.nob_decompose(other.get_velocity());
                        v.set_x(-other.get_restitution() * v.get_x());
                        other.set_velocity(norm.nob_compose(v));
                    }
                    else if( ! other.is("Ball") )
                    {
                        Vector<double> norm = dcp.get_unit();
                        Vector<double> v;
                        v.set_x(-other.get_restitution() * distance);
                        other.set_velocity(norm.nob_compose(v) + other.get_velocity());
                    }

                    return true;
                }
            }

            return false;
        }
    }
}
