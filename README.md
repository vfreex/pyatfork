# PyAtfork
PyAtfork provides API to register fork handlers. 

PyAtfork is a wrapper for the POSIX Pthreads API [pthread_atfork(3)][]. I wrote this module because Python's [ctypes][] library cannot handle this function correctly. 

## Function prototype
### atfork(prepare=None, parent=None, child=None)
Register fork handlers to the caller process. If an error occurs OSError is raised.

The order of calls to `pyatfork.atfork()` is significant. The `parent` and `child` fork handlers shall be called in the
order in which they were established by calls to `pyatfork.atfork()`. The `prepare` fork handlers shall be called in
the opposite order.

## Sample 

``` python
import os
import pyatfork

def prepare_hdr():
    print("prepare handler: pid=%s" % os.getpid())

def parent_hdr():
    print("parent handler: pid=%s" % os.getpid())

def child_hdr():
    print("child handler: pid=%s" % os.getpid())

pyatfork.atfork(prepare=prepare_hdr, parent=parent_hdr, child=child_hdr)

pid = os.fork()
if pid == -1:
    raise RuntimeError("do fork failed")
elif pid == 0:
    print("child process: pid=%s" % os.getpid())
elif pid > 0:
    print("parent process: pid=%s" % os.getpid())
    os.waitpid(pid, 0)
```

[pthread_atfork(3)]: http://linux.die.net/man/3/pthread_atfork
[ctypes]: https://docs.python.org/3/library/ctypes.html

