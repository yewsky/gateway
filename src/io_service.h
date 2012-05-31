#pragma once

#ifndef _IO_SERVICE_H_
#define _IO_SERVICE_H_

#include <cstdint>
#include <sys/epoll.h>

namespace IO {

#define IO_SETSIZE (1024*10) ///< 最大观察的


typedef void IOCallback(int fd, void *userdata, int mask);

/* File event structure */
typedef struct tagIOEvent {
    int 		mask; /* one of AE_(READABLE|WRITABLE) */
    IoCallback 	*rCb;
    IoCallback 	*wCb;
    void 		*userdata;
} IOEvent;


class IOService
{
    enum Status {
          Running
        , Stopped
    };

public:


public:
	int32_t Initialize()
	{
		_epoll_fd = epoll_create(1024);
		if (-1 == _epoll_fd) {
			return -1;
		}
		for (i = 0; i < IO_SETSIZE; i++) {
			_io_events[i].mask = AE_NONE;
		}

	}

	void AddEvent(int fd, int mask, IOCallback *proc, void *userdata)
	{
		if (fd >= IO_SETSIZE)
			return AE_ERR;

		int op = _io_events[fd].mask == IO_NONE ? EPOLL_CTL_ADD : EPOLL_CTL_MOD;
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

		_io_event[fd].mask |= mask;
		if (mask & IO_READABLE) {
		    _io_event[fd].rCb = proc;
		}
		if (mask & IO_WRITABLE) {
		    _io_event[fd].wCb = proc;
		}

		fe->userdata = userdata;
		if (fd > _maxfd)
			_maxfd = fd;

		return AE_OK;
	}

    void Run()
    {
        while (_loop_status != Stopped) {
            ProcessEvents();
        }
    }

    int32_t ProcessEvents()
    {
        int event_count = 0; ///<
        int retval = epoll_wait(_epoll_fd, _epoll_events, IO_SETSIZE, -1);
        if (retval > 0) {
            event_count = retval;
            for (int i = 0; i < event_number; i++) {
                int mask = IO_NONE;
                if (_epoll_events[i].events & EPOLLIN) { mask |= IO_READABLE; }
                if (_epoll_events[i].events & EPOLLOUT) { mask |= IO_WRITABLE; }


                if (_io_events[fd].mask & mask & IO_READABLE) {
                    _io_events[fd].rCb(fd, _io_events[fd].userdata);
                }
                if (_io_events[fd].mask & mask & IO_WRITABLE) {
                    _io_events[fd].wCb(fd, _io_events[fd].userdata);
                }
            }
        }
        return event_count;
    }



private:
    int32_t             _loop_status;
    int 		        _maxfd;
    IOEvent 	        _io_events[IO_SETSIZE]; /* Registered events */
	int 				_epoll_fd;
    struct epoll_event 	_epoll_events[IO_SETSIZE];
}






} /// namespace io


#endif /// _IO_SERVICE_H_
