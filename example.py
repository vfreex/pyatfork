#!/usr/bin/env python

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

