//! \file throttle.hh
//! Throttle regulates the rate at which functions are called.
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

#ifndef LIBMISC__THROTTLE_HH
#define LIBMISC__THROTTLE_HH

#include <queue>
#include <stdexcept>
#include <sigc++/connection.h>
#include <sigc++/functors/slot.h>
#include <glibmm/timer.h>

//******************************************************************************

namespace misc {

//! Regulates the rate at which functions are called.
/*! A throttle queues one or more action functors and calls them according to
 * a throttling mode (described in the Mode enumeration). Throttles must be used
 * inside a Glib::MainLoop. Exceptions thrown from actions are discarded. */
class Throttle
{
public:

	//! A single functor to be queued for calling.
	/*! To arrive at a functor of this type, use sigc::bind() to set function
	 * parameters and sigc::hide_return() if the function returns a value. */
	typedef sigc::slot<void> Action;

	//! A mode for the regulation of action calls.
	enum Mode {
		//! Actions are called immediately upon queueing.
		//! When this mode is set, all queued actions are called immediately.
		OPEN,
		//! No actions are called.
		CLOSED,
		//! No actions are called until the threshold time is elapsed.
		/*! The timer may be started immediately, or when the next action is
		 * queued. When the time has elapsed, the mode is set to \p OPEN. */
		PAUSE,
		//! Actions are called immediately until the threshold time is elapsed.
		/*! The timer may be started immediately, or when the next action is
		 * queued. When the time has elapsed, the mode is set to \p CLOSED. */
		UNTIL,
		//! Actions are called at a limited rate.
		/*! Each queued action is called after the elapsed time since the last
		 * action call has passed than the timer threshold. The timer is started
		 * when the next action is queued, or immediately if the queue is not
		 * empty. This mode is ideal for expensive interface updates. */
		LIMIT,
		//! Actions are called after the threshold time has elapsed.
		/*! This is equivalent to the \p LIMIT mode, except that queueing an
		 * action also resets the timer. This mode is ideal for idempotent
		 * background actions performed in response to events which may occur
		 * rapidly and frequently. */
		DELAY
	};

	//! Constructs a new throttle.
	/*! For parameter details, see set().
	 * \param mode The initial throttle mode.
	 * \param threshold The initial timer threshold in seconds.
	 * \param immediate Whether to start the timer immediately in some modes.
	 * \throw std::invalid_argument if the mode or threshold is invalid. */
	explicit Throttle (Mode mode = OPEN, double threshold = 0.0,
		bool immediate = false) throw (std::invalid_argument);
	
	//! Destroys the throttle.
	/*! Any actions remaining in the queue are destroyed without being called.
	 * To ensure that each action has been called, set the throttle to OPEN
	 * before destroying it. */
	~Throttle ();

	//! Returns the current throttle mode.
	Mode get_mode () const throw ();
	
	//! Returns whether the throttle's internal timer is running.
	/*! In \p PAUSE and \p UNTIL modes, the timer counts down to the impending
	 * mode change. In \p LIMIT and \p DELAY modes, the timer counts the time
	 * elapsed since the last action call. The timer is not used in \p OPEN
	 * and \p CLOSED modes. */
	bool is_running () const throw ();

	//! Sets the throttle mode.
	/*! If the initial state of the new mode allows, an action or actions may be
	 * called immediately.
	 * \param mode The new throttle mode.
	 * \param immediate For \p PAUSE and \p UNTIL modes, whether to start the
	 * timer immediately (\c true) or to start it when the next action is queued
	 * (\c false).
	 * \throw std::invalid_argument if the mode is not a valid Mode. */
	void set (Mode mode, bool immediate = false) throw (std::invalid_argument);
	
	//! Sets the throttle mode and timer threshold atomically.
	/*! For details, see set(Mode, bool) and set_threshold().
	 * \param mode The new throttle mode.
	 * \param threshold The new timer threshold in seconds.
	 * \param immediate Whether to start the timer immediately in some modes.
	 * \throw std::invalid_argument if the mode or threshold is invalid. */
	void set (Mode mode, double threshold, bool immediate = false)
		throw (std::invalid_argument);

	//! Restores the throttle to the initial state for the current mode.
	/*! For details, see set().
	 * \param immediate Whether to start the timer immediately in some modes. */
	void reset (bool immediate = false) throw ();

	//! Returns the timer threshold in seconds.
	//! See set_threshold() for details.
	double get_threshold () const throw ();
	
	//! Sets the timer threshold.
	/*! In \p PAUSE and \p UNTIL modes, the threshold is the total time to wait
	 * before the mode is changed. In \p LIMIT and \p DELAY modes, the threshold
	 * is the minimum time between action calls. The threshold is not used in
	 * \p OPEN and \p CLOSED modes.
	 * \param threshold The new timer threshold in seconds.
	 * \throw std::invalid_argument if the threshold is negative. */
	void set_threshold (double threshold) throw (std::invalid_argument);
	
	//! Adds a new functor to the action queue.
	/*! If the current state allows, this action or queued actions may be called
	 * immediately. For modes using the timer, the timer will be started if it
	 * is not already running.
	 *
	 * If \p idempotent is \c true, the queue will be cleared before this action
	 * is added. For practically idempotent actions such as many interface
	 * updates, this prevents unnecessarily repeated calls.
	 * \param action The action functor to be queued for calling.
	 * \param idempotent Whether \p action is idempotent with each action in the
	 * queue. */
	void queue (const Action& action, bool idempotent) throw ();
	
	//! Returns whether the action queue is empty.
	bool empty () const throw ();
	
	//! Removes all actions from the queue immediately without calling them.
	void clear () throw ();

private:

	void do_set (Mode mode, bool immediate) throw (std::invalid_argument);

	void start () throw ();
	void stop () throw ();

	bool update (bool from_queue = false) throw ();
	void request_update (unsigned int interval = 0) throw ();
	void cancel_update () throw ();
	
	void run_top () throw ();
	void run_all () throw ();

	Mode mode;
	double threshold;

	Glib::Timer timer;
	bool running;

	std::queue<Action> action_queue;
	sigc::connection update_conn;
};

} // namespace misc

#endif // LIBMISC__THROTTLE_HH
