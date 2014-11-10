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

#include <string>
#include <list>
#include "error.hpp"
#include "float.hpp"
#include "settingfile.hpp"

namespace DropletCollection
{
    std::string
    SettingFile::get_full_filename (const std::string& basename,
                                    const std::string& dirname)
    {
        std::list<std::string> array = get_full_filename_list(basename, dirname);

        if( array.empty() )
        {
            return basename; // ファイルが見つからなければ basename をそのまま返す (エラーメッセージにでも使ってもらえ)
        }
        else
        {
            return std::string(*(array.begin()));
        }
    }

    std::list<std::string>
    SettingFile::get_full_filename_list (const std::string& basename,
                                         const std::string& dirname)
    {
        std::list<std::string> array;

        // dirname/basename を調べる
        std::string fullname = Glib::build_filename(dirname, basename);
        if( Glib::file_test(fullname, Glib::FILE_TEST_EXISTS | Glib::FILE_TEST_IS_REGULAR) )
        {
            array.push_back(fullname);
        }

        // ホームディレクトリを検索する
        bool homedir_ok;
        const std::string homedir = Glib::getenv("HOME", homedir_ok);
        if( homedir_ok )
        {
            const std::string dirname = Glib::build_filename(homedir, ".droplet-collection");
            fullname = Glib::build_filename(dirname, basename);
            if( Glib::file_test(fullname, Glib::FILE_TEST_EXISTS | Glib::FILE_TEST_IS_REGULAR) )
            {
                array.push_back(fullname);
            }
        }

        // 共通ディレクトリを検索する
        fullname = Glib::build_filename(DC_DATADIR, basename);
        if( Glib::file_test(fullname, Glib::FILE_TEST_EXISTS | Glib::FILE_TEST_IS_REGULAR) )
        {
            array.push_back(fullname);
        }

        return array;
    }

    SettingFile::SettingFile ()
        : Glib::KeyFile()
    {
        set_list_separator(',');
    }

    bool
    SettingFile::load_from_file (const std::string&       filename,
                                 const Glib::KeyFileFlags flags)
    {
        m_filename = filename;

        return Glib::KeyFile::load_from_file(filename, flags);
    }

    /**
     * @throw Glib::KeyFileError
     */
    double
    SettingFile::get_double (const Glib::ustring& group_name,
                             const Glib::ustring& key,
                             const double         min,
                             const double         max) const
    {
        double value = Glib::KeyFile::get_double(group_name, key);

        if( ! fbetweq(value, min, max) ) // 値が指定の範囲に存在しない
        {
            Glib::ustring msg = Glib::ustring::compose("キー '%1' の値は %2 〜 %3 の範囲で設定してください",
                                                       key, min, max);
            throw Glib::KeyFileError(Glib::KeyFileError::INVALID_VALUE, msg);
        }

        return value;
    }

    std::list<double>
    SettingFile::get_double_list (const Glib::ustring& group_name,
                                  const Glib::ustring& key,
                                  const double         min,
                                  const double         max) const
    {
        std::list<double> list = Glib::KeyFile::get_double_list(group_name, key);
        std::list<double>::const_iterator it;

        for( it = list.begin(); it != list.end(); ++it )
        {
            if( ! fbetweq(*it, min, max) )
            {
                Glib::ustring msg = Glib::ustring::compose("キー '%1' の値は %2 〜 %3 の範囲で設定してください",
                                                           key, min, max);
                throw Glib::KeyFileError(Glib::KeyFileError::INVALID_VALUE, msg);
            }
        }

        return list;
    }

    /**
     * @throw Glib::KeyFileError
     * @throw Glib::FileError
     * @throw Gdk::PixbufError
     */
    Glib::RefPtr<Gdk::Pixbuf>
    SettingFile::get_image (const Glib::ustring& group_name,
                            const Glib::ustring& key) const
    {
        Cairo::RefPtr<Cairo::ImageSurface> image;
        std::string filename;

        filename = Glib::filename_from_utf8(Glib::KeyFile::get_string(group_name, key));

        return Gdk::Pixbuf::create_from_file(get_full_filename(filename, get_dirname()));
    }

    Color
    SettingFile::get_color (const Glib::ustring& group_name,
                            const Glib::ustring& key) const
    {
        Glib::ustring str_rep;
        unsigned int n_elems, elem_len, n_max, i, j;

        str_rep = Glib::KeyFile::get_string(group_name, key);

        switch( str_rep.size() )
        {
            case 7: // "#RRGGBB"
                n_elems  = 3;
                elem_len = 2;
                n_max    = 0xff;
            break;
            case 9: // "#RRGGBBAA"
                n_elems  = 4;
                elem_len = 2;
                n_max    = 0xff;
            break;
            case 13: // "#RRRRGGGGBBBB"
                n_elems  = 3;
                elem_len = 4;
                n_max    = 0xffff;
            break;
            case 17: // "#RRRRGGGGBBBBAAAA"
                n_elems  = 4;
                elem_len = 4;
                n_max    = 0xffff;
            break;
            default:
                throw Glib::KeyFileError(Glib::KeyFileError::INVALID_VALUE,
                                         Glib::ustring::compose("キー '%1' を色として解釈できません",
                                                                key));
        }

        // 文字列を解析して，色成分の割合に変換する
        double data[4] = {0.0, 0.0, 0.0, 1.0};
        const char *s = str_rep.c_str();

        if( s[0] == '#' ) ++s;
        else
        {
            throw Glib::KeyFileError(Glib::KeyFileError::INVALID_VALUE,
                                     Glib::ustring::compose("キー '%1' を色として解釈できません",
                                                            key));
        }

        for( i = 0; i < n_elems; ++i )
        {
            int n = 0;
            for( j = 0; j < elem_len; ++j )
            {
                int v = g_ascii_xdigit_value(s[i * elem_len + j]);

                if( v == -1 ) // 16進数値ではない
                {
                    throw Glib::KeyFileError(Glib::KeyFileError::INVALID_VALUE,
                                             Glib::ustring::compose("キー '%1' を色として解釈できません",
                                                                    key));
                }
                else
                {
                    n = 16 * n + v;
                }
            }

            data[i] = (double) n / n_max;
        }

        return Color(data[0], data[1], data[2], data[3]);
    }

}
