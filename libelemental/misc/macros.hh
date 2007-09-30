//! \file macros.hh
//! Syntactic sugar macros for private use in programs.
/*
 * This file is part of libmisc, an assortment of code for reuse.
 *
 * Copyright (C) 2007 Kevin Daughtridge <kevin@kdau.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBMISC__MACROS_HH
#define LIBMISC__MACROS_HH

#include <glibmm/refptr.h>

#define CAST(ptr, to) dynamic_cast<to*>(ptr)
#define cast(ptr, to, var) (to *var = CAST(ptr, to))

#define REFPTR_CAST(ptr, to) Glib::RefPtr<to>::cast_dynamic(ptr)
#define refptr_cast(ptr, to, var) (Glib::RefPtr<to> var = REFPTR_CAST(ptr, to))

#define FOREACH_BASE(itertype, seq, var) \
for (itertype var = seq.begin(); var != seq.end(); ++var)
#define FOREACH(type, seq, var) FOREACH_BASE(type::iterator, seq, var)
#define CONST_FOREACH(type, seq, var) \
FOREACH_BASE(type::const_iterator, seq, var)

#define RFOREACH_BASE(itertype, seq, var) \
for (itertype var = seq.rbegin(); var != seq.rend(); ++var)
#define RFOREACH(type, seq, var) RFOREACH_BASE(type::reverse_iterator, seq, var)
#define CONST_RFOREACH(type, seq, var) \
RFOREACH_BASE(type::const_reverse_iterator, seq, var)

#endif // LIBMISC__MACROS_HH
