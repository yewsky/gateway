#pragma once

#ifndef _IO_H_
#define _IO_H_

#include <cstdint>
#include <sys/epoll.h>

/* File event structure */
typedef struct tagIoEvent {
    int 		mask; /* one of AE_(READABLE|WRITABLE) */
    IoCallback 	*rfileProc;
    IoCallback 	*wfileProc;
    void 		*userdata;
} IoEvent;

typedef struct tagFiredEvent {
    int 	fd;
    int 	mask;
} FiredEvent;

typedef struct EventLoop {
    int 		maxfd;
    IoEvent 	events[IO_SETSIZE]; /* Registered events */
    FiredEvent 	fired[IO_SETSIZE]; /* Fired events */
    int 		stop;
    void 		*apidata; /* This is used for polling API specific data */
} aeEventLoop;

class EpollEvent
{
public:
	typedef void Callback(struct aeEventLoop *eventLoop, int fd, void *userdata, int mask);

public:
	int32_t Initialize()
	{
		_epoll_fd = epoll_create(1024);
		if (-1 == _epoll_fd) {
			return -1;
		}
		for (i = 0; i < AE_SETSIZE; i++) {
			event_loop->events[i].mask = AE_NONE;
		}

	}

	void AddEvent(int fd, int mask, Callback *proc, void *userdata)
	{
		if (fd >= AE_SETSIZE)
			return AE_ERR;

		int op = _event_loop.events[fd].mask == IO_NONE ? EPOLL_CTL_ADD : EPOLL_CTL_MOD;
		struct epoll_event ee;
		if (mask & IO_READABLE) {
			ee.events |= EPOLLIN;
		}
		if (mask & IO_WRITABLE) {
			ee.events = EPOLLOUT;
		}
		ee.data.u64 = 0;
		ee.data.fd = fd;
		int ret = epoll_ctl(_epoll_fd, op, fd, &ee);
		if (0 != ret) {
			return AE_ERR;
		}


		IoEvent *event = &_event_loop->events[fd];

		event->mask |= mask;
		if (mask & AE_READABLE) {
		    event->rfileProc = proc;
		}
		if (mask & AE_WRITABLE) {
		    event->wfileProc = proc;
		}

		fe->userdata = userdata;
		if (fd > _event_loop->maxfd)
			_event_loop->maxfd = fd;
		return AE_OK;
	}

    void Run()
    {
        while (_loop_stop != 1) {
            ProcessEvents();
        }
    }

    int32_t ProcessEvents()
    {
        int event_count = 0; ///<
        int retval = epoll_wait(_epoll_fd, _epoll_events, AE_SETSIZE, -1);
        if (retval > 0) {
            event_count = retval;
            for (int i = 0; i < event_number; i++) {
                int mask = IO_NONE;
                if (_epoll_events[i].events & EPOLLIN) { mask |= IO_READABLE; }
                if (_epoll_events[i].events & EPOLLOUT) { mask |= IO_WRITABLE; }


                IoEvent *io_event = &_event_loop.events[fd];
                if (io_event->mask & mask & IO_READABLE) {
                    io_event->rfileProc(fd, io_event->userdata);
                }
            }
        }
        return event_count;
    }

private:
    int32_t             _loop_stop;
	EventLoop 			_event_loop;
	int 				_epoll_fd;
    struct epoll_event 	_epoll_events[AE_SETSIZE];
}


#endif /// _IO_H_
