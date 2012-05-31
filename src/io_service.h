#pragma once

#ifndef _IO_SERVICE_H_
#define _IO_SERVICE_H_

#include <cstdint>
#include <sys/epoll.h>

namespace IO {

#define IO_SETSIZE (1024*10) ///< 

#define IO_ERR 1
#define IO_OK	0

#define	IO_NONE  	0
#define IO_READABLE 1
#define IO_WRITABLE 2


typedef void IOCallback(int fd, void *userdata, int mask);

/* File event structure */
typedef struct tagIOEvent {
    int 		mask; /* one of AE_(READABLE|WRITABLE) */
    IOCallback 	*rCb;
    IOCallback 	*wCb;
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
		for (int32_t i = 0; i < IO_SETSIZE; i++) {
			_io_events[i].mask = IO_NONE;
		}

	}

	int32_t AddEvent(int fd, int mask, IOCallback *proc, void *userdata)
	{
		if (fd >= IO_SETSIZE)
			return IO_ERR;

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
			return IO_ERR;
		}

		_io_events[fd].mask |= mask;
		if (mask & IO_READABLE) {
		    _io_events[fd].rCb = proc;
		}
		if (mask & IO_WRITABLE) {
		    _io_events[fd].wCb = proc;
		}
		
		_io_events[fd].userdata = userdata;
		if (fd > _maxfd)
			_maxfd = fd;

		return IO_OK;
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
            for (int i = 0; i < event_count; i++) {
                int mask = IO_NONE;
                if (_epoll_events[i].events & EPOLLIN) { mask |= IO_READABLE; }
                if (_epoll_events[i].events & EPOLLOUT) { mask |= IO_WRITABLE; }

				int fd = _epoll_events[i].data.fd;
                if (_io_events[fd].mask & mask & IO_READABLE) {
                    _io_events[fd].rCb(fd, _io_events[fd].userdata, mask);
                }
                if (_io_events[fd].mask & mask & IO_WRITABLE) {
                    _io_events[fd].wCb(fd, _io_events[fd].userdata,mask);
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
};






} /// namespace io


#endif /// _IO_SERVICE_H_
