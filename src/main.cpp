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

#include <gtkmm.h>
#include "error.hpp"
#include "object-manager.hpp"

namespace DropletCollection
{
    class Screen : public Gtk::DrawingArea
    {
        public:
            Screen ();

            void load   (const std::string& filename);
            void unload ();
            void reset  ();

        protected:
            virtual bool on_motion_notify_event (GdkEventMotion * event);
            virtual bool on_button_press_event  (GdkEventButton * event);
            virtual bool on_expose_event        (GdkEventExpose * event);
            virtual bool on_timeout             ();

        private:
            DropletCollection::Object::Manager * m_manager; // 現在プレイ中のステージ
    };

    Screen::Screen ()
        : m_manager(0)
    {
        add_events(Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_PRESS_MASK);
        Glib::signal_timeout().connect(sigc::mem_fun(*this, &Screen::on_timeout), 35);
    }

    void
    Screen::load (const std::string& filename)
    {
        delete m_manager;
        m_manager = 0;

        m_manager = new DropletCollection::Object::Manager (filename);
        m_manager->initialize();

        set_size_request(m_manager->get_width(), m_manager->get_height());

        queue_draw(); // 画面を再描画する
    }

    void
    Screen::unload ()
    {
        delete m_manager;
        m_manager = 0;
        queue_draw(); // 画面を再描画する
    }

    void
    Screen::reset ()
    {
        if( m_manager == 0 ) return;

        m_manager->initialize();
        queue_draw(); // 画面を再描画する
    }

    bool
    Screen::on_motion_notify_event (GdkEventMotion * const event)
    {
        if( m_manager == 0 ) return false;

        m_manager->move_racket(event->x); // ラケットの位置を移動する

        return true;
    }

    bool
    Screen::on_button_press_event (GdkEventButton * const event G_GNUC_UNUSED)
    {
        if( m_manager == 0 ) return false;

        if( event->button == 1 ) // 左クリック
        {
            if( m_manager->get_state() == DropletCollection::Object::Manager::STATE_STAND_BY )
            {
                m_manager->launch(); // ボールを発射する
            }
        }

        return true;
    }

    bool
    Screen::on_expose_event (GdkEventExpose * const event G_GNUC_UNUSED)
    {
        if( m_manager == 0 ) return false;

        m_manager->draw( get_window()->create_cairo_context() ); // 画面を描画する

        return true;
    }

    bool
    Screen::on_timeout ()
    {
        if( m_manager == 0 ) return true;

        m_manager->simulate(); // 物体の動きをシミュレーションする

        queue_draw(); // 画面を再描画する

        return true;
    }

    /**************************************************************************
     *
     *  class DropletCollection::StageSelector
     *
     *************************************************************************/

    class StageSelector : public Gtk::ComboBoxText
    {
        public:
            StageSelector ();

            std::string get_active_filename () const;

        private:
            bool append_stage_file (const SettingFile& file,
                                    unsigned int       id);

            std::vector<std::string> m_file_list;
    };

    StageSelector::StageSelector ()
    {
        std::list<std::string> filenames = SettingFile::get_full_filename_list("index");
        std::list<std::string>::const_iterator it;

        for( it = filenames.begin(); it != filenames.end(); ++it )
        {
            std::cerr << "目次ファイル '" << *it << "' を読み込んでいます" << std::endl;

            try
            {
                SettingFile file;
                unsigned int id;

                file.load_from_file(*it);

                for( id = 1; append_stage_file(file, id); ++id ) ;
            }
            catch (Glib::Error& e)
            {
                std::cerr << "Glib::Error::what() = " << e.what() << std::endl;
            }
        }
    }

    bool
    StageSelector::append_stage_file (const SettingFile& file,
                                      const unsigned int id)
    {
        const Glib::ustring name_key = Glib::ustring::compose("Name%1", id);
        const Glib::ustring file_key = Glib::ustring::compose("File%1", id);

        if( file.has_key("Index", name_key) && file.has_key("Index", file_key) )
        {
            Glib::ustring name_val = file.get_string("Index", name_key);
            Glib::ustring file_val = file.get_string("Index", file_key);
            std::string filename = SettingFile::get_full_filename(Glib::filename_from_utf8(file_val),
                                                                  file.get_dirname());

            append_text(name_val);
            m_file_list.push_back(filename);

            std::cerr << "ステージ '" << name_val << "' としてファイル '" << filename << "' を登録しました" << std::endl;

            return true;
        }
        else
        {
            return false;
        }
    }

    std::string
    StageSelector::get_active_filename () const
    {
        int i = property_active().get_value();

        dc_return_val_if_fail(i >= 0 && i < (int)m_file_list.size(), "");

        return m_file_list[i];
    }

    /**************************************************************************
     *
     *  class DropletCollection::MainWindow
     *
     *************************************************************************/

    class MainWindow : public Gtk::Window
    {
        public:
                     MainWindow ();
            virtual ~MainWindow ();

        private:
            void stage_selector_on_changed ();

            Gtk::VBox     m_box_main;
            Gtk::HBox     m_box_menu;
            StageSelector m_stage_selector;
            Screen        m_screen;
            Gtk::Button   m_btn_reset;
            Gtk::Image    m_img_reset;
    };

    MainWindow::MainWindow ()
        : m_box_main(false, 5),
          m_box_menu(false, 0),
          m_img_reset(Gtk::Stock::REFRESH, Gtk::ICON_SIZE_LARGE_TOOLBAR)
    {
        m_stage_selector.signal_changed().connect(sigc::mem_fun(*this, &MainWindow::stage_selector_on_changed));
        m_box_menu.pack_start(m_stage_selector, true, true, 0);

        m_btn_reset.add(m_img_reset);
        m_btn_reset.signal_clicked().connect(sigc::mem_fun(m_screen, &Screen::reset));
        m_box_menu.pack_start(m_btn_reset, false, false, 0);

        m_box_main.pack_start(m_box_menu, false, false, 0);
        m_box_main.pack_start(m_screen, true, true, 0);

        set_title(PACKAGE_STRING);
        set_position(Gtk::WIN_POS_CENTER);
        set_default_size(400, 500);
        add(m_box_main);
    }

    MainWindow::~MainWindow ()
    {
        // Nothing to do.
    }

    void
    MainWindow::stage_selector_on_changed ()
    {
        std::string filename = m_stage_selector.get_active_filename();

        dc_log("ファイル '%s' を読み込みます", filename.c_str());

        try
        {
            m_screen.load(filename);
        }
        catch(Glib::Error& e)
        {
            m_screen.unload();
            Gtk::MessageDialog alert(*this, e.what(), true, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE, true);
            alert.show_all();
            alert.run();
            dc_log("Glib::Error::what() = %s", e.what().c_str());
        }
    }
}

int
main (int    argc,
      char * argv[])
{
    Gtk::Main kit(argc, argv);
    DropletCollection::MainWindow window;

    window.show_all();

    kit.run(window);

    return EXIT_SUCCESS;
}
