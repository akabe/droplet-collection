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

#ifndef INCLUDE_GUARD_9F9CD28B_DD70_45C2_AE0F_A0CE1C879FB5
#define INCLUDE_GUARD_9F9CD28B_DD70_45C2_AE0F_A0CE1C879FB5

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <glibmm/keyfile.h>
#include <glibmm/arrayhandle.h>
#include <gdkmm.h>

namespace DropletCollection
{
    struct Color
    {
        double red;
        double green;
        double blue;
        double alpha;

        Color (double r, double g, double b)
            : red(r), green(g), blue(b), alpha(1.0)
        {
        }

        Color (double r, double g, double b, double a)
            : red(r), green(g), blue(b), alpha(a)
        {
        }
    };

    class SettingFile : public Glib::KeyFile
    {
        public:
            SettingFile ();

            static std::string            get_full_filename      (const std::string& basename,
                                                                  const std::string& dirname = ".");
            static std::list<std::string> get_full_filename_list (const std::string& basename,
                                                                  const std::string& dirname = ".");

            bool                          load_from_file         (const std::string& filename,
                                                                  Glib::KeyFileFlags flags = Glib::KEY_FILE_NONE);

            inline std::string            get_filename    () const;
            inline std::string            get_dirname     () const;

            double                        get_double      (const Glib::ustring& group_name,
                                                           const Glib::ustring& key,
                                                           double               min,
                                                           double               max) const;
            std::list<double>             get_double_list (const Glib::ustring& group_name,
                                                           const Glib::ustring& key,
                                                           double               min,
                                                           double               max) const;
            Glib::RefPtr<Gdk::Pixbuf>     get_image       (const Glib::ustring& group_name,
                                                           const Glib::ustring& key) const;
            Color                         get_color       (const Glib::ustring& group_name,
                                                           const Glib::ustring& key) const;

        private:
            std::string m_filename;
    };

    inline std::string
    SettingFile::get_filename () const
    {
        return m_filename;
    }

    inline std::string
    SettingFile::get_dirname () const
    {
        return Glib::path_get_dirname(m_filename);
    }
}

#endif /* ! INCLUDE_GUARD_9F9CD28B_DD70_45C2_AE0F_A0CE1C879FB5 */
