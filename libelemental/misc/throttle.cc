/*
 * This file is part of libmisc, an assortment of code for reuse.
 *
 * Copyright (C) 2006-2007 Kevin Daughtridge <kevin@kdau.com>
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

#include "throttle.hh"

#include <sigc++/adaptors/bind.h>
#include <glibmm/main.h>

namespace misc {


//******************************************************************************
// class Throttle


Throttle::Throttle (Mode mode_, double threshold_, bool immediate)
:	mode (OPEN), threshold (0.0), running (false)
{
	set (mode_, threshold_, immediate);
}


Throttle::~Throttle ()
{
	cancel_update ();
}


Throttle::Mode
Throttle::get_mode () const throw ()
{
	return mode;
}


bool
Throttle::is_running () const throw ()
{
	return running;
}


void
Throttle::set (Mode mode_, bool immediate)
{
	do_set (mode_, immediate);
	update ();
}


void
Throttle::set (Mode mode_, double threshold_, bool immediate)
{
	do_set (mode_, immediate);
	set_threshold (threshold_);
}


void
Throttle::reset (bool immediate) throw ()
{
	do_set (mode, immediate);
	update ();
}


double
Throttle::get_threshold () const throw ()
{
	return threshold;
}


void
Throttle::set_threshold (double threshold_)
{
	if (threshold_ < 0.0)
		throw std::invalid_argument ("negative threshold");
	threshold = threshold_;
	update ();
}


void
Throttle::queue (const Action& action, bool idempotent) throw ()
{
	if (idempotent) clear ();
	action_queue.push (action);
	update (true);
}


bool
Throttle::empty () const throw ()
{
	return action_queue.empty ();
}


void
Throttle::clear () throw ()
{
	while (!action_queue.empty ())
		action_queue.pop ();
}


void
Throttle::do_set (Mode mode_, bool immediate)
{
	switch (mode_)
	{
	case OPEN:
	case CLOSED:
	case LIMIT:
	case DELAY:
		stop ();
		break;
	case PAUSE:
	case UNTIL:
		if (immediate) start ();
		else stop ();
		break;
	default:
		throw std::invalid_argument ("invalid Throttle::Mode");
	}

	cancel_update ();
	mode = mode_;
}


void
Throttle::start () throw ()
{
	timer.start ();
	running = true;
}


void
Throttle::stop () throw ()
{
	timer.stop ();
	running = false;
}


bool
Throttle::update (bool from_queue) throw ()
{
	cancel_update ();

	switch (mode)
	{
	case OPEN:
		run_all ();
		return false;
	case CLOSED:
		return false;
	case PAUSE:
		if (running && timer.elapsed () >= threshold)
		{
			do_set (OPEN, false);
			run_all ();
			return false;
		}
		if (!running && from_queue)
			start ();
		break;
	case UNTIL:
		if (running && timer.elapsed () >= threshold)
		{
			do_set (CLOSED, false);
			return false;
		}
		if (!running && from_queue)
			start ();
		run_all ();
		return false;
	case LIMIT:
		if (!running || timer.elapsed () >= threshold)
		{
			if (!action_queue.empty ())
				start ();
			run_top ();
		}
		break;
	case DELAY:
		if (running && timer.elapsed () >= threshold)
		{
			if (!action_queue.empty ())
				start ();
			run_top ();
		}
		else if (!running || from_queue)
			start ();
		break;
	default:
		return false;
	}

	if (action_queue.empty ())
		return false;
	else
	{
		request_update ();
		return true;
	}
}


void
Throttle::request_update (unsigned int interval) throw ()
{
	if (interval == 0)
	{
		double until = threshold - timer.elapsed ();
		interval = (until > 0.0) ? (unsigned int) (until * 1000.0) : 0u;
	}
	if (!update_conn)
		update_conn = Glib::signal_timeout ().connect (sigc::bind
			(sigc::mem_fun (*this, &Throttle::update), false), ++interval);
}


void
Throttle::cancel_update () throw ()
{
	if (update_conn)
		update_conn.disconnect ();
	update_conn = sigc::connection ();
}


void
Throttle::run_top () throw ()
{
	if (!action_queue.empty ())
	{
		try
		{
			action_queue.front () ();
		}
		catch (...) {}
		action_queue.pop ();
	}
}


void
Throttle::run_all () throw ()
{
	while (!action_queue.empty ())
	{
		try
		{
			action_queue.front () ();
		}
		catch (...) {}
		action_queue.pop ();
	}
}


} // namespace misc
