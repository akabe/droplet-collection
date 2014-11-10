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

#ifndef INCLUDE_GUARD_61F9D21A_06E0_49D8_A10D_D1E241F9AE77
#define INCLUDE_GUARD_61F9D21A_06E0_49D8_A10D_D1E241F9AE77

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <cstdio>
#include <iostream>
#include <glibmm.h>

#define dc_log(fmt, ...) \
    std::fprintf(stderr, "%s:%d: " fmt "\n", __FILE__, __LINE__, ## __VA_ARGS__)

#define dc_warning(fmt, ...) \
    std::fprintf(stderr, "\n\e[32m** Warning ** %s:%d: " fmt "\e[0m\n", __FILE__, __LINE__, ## __VA_ARGS__)

#define dc_error(fmt, ...) \
    std::fprintf(stderr, "\n\e[31m** Error ** %s:%d: " fmt "\e[0m\n", __FILE__, __LINE__, ## __VA_ARGS__)

#define dc_return_if_fail(expr) \
    do { \
        if( !(expr) ) \
        { \
            dc_error("'%s': Failed '%s'", G_STRFUNC, #expr); \
            return; \
        } \
    } while(0)

#define dc_return_val_if_fail(expr, val) \
    do { \
        if( !(expr) ) \
        { \
            dc_error("'%s': Failed '%s'", G_STRFUNC, #expr); \
            return (val); \
        } \
    } while(0)

#define dc_warn_if_fail(expr) \
    do { \
        if( !(expr) ) dc_warning("'%s': Failed '%s'", G_STRFUNC, #expr); \
    } while(0)

#endif /* ! INCLUDE_GUARD_61F9D21A_06E0_49D8_A10D_D1E241F9AE77 */
