/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020, 2021, 2022 Volker Christian <me@vchrist.at>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "TimerEventReceiver.h"

#include "EventLoop.h"
#include "EventMultiplexer.h"
#include "Timer.h"
#include "TimerEventPublisher.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace core {

    TimerEventReceiver::TimerEventReceiver(const utils::Timeval& delay)
        : timerEventPublisher(EventLoop::instance().getEventMultiplexer().getTimerEventPublisher())
        , absoluteTimeout(utils::Timeval::currentTime() + delay)
        , delay(delay) {
    }

    TimerEventReceiver::~TimerEventReceiver() {
        if (timer != nullptr) {
            timer->removeTimerEventReceiver();
        }
    }

    utils::Timeval TimerEventReceiver::getTimeout() const {
        return absoluteTimeout;
    }

    void TimerEventReceiver::updateTimeout() {
        absoluteTimeout += delay;
    }

    void TimerEventReceiver::enable() {
        timerEventPublisher.add(this);
    }

    void TimerEventReceiver::update() {
        timerEventPublisher.erase(this);
        absoluteTimeout += delay;
        timerEventPublisher.insert(this);
    }

    void TimerEventReceiver::cancel() {
        timerEventPublisher.remove(this);
    }

    void TimerEventReceiver::setTimer(Timer* timer) {
        this->timer = timer;
    }

} // namespace core
