from .catfork import pthread_atfork

def atfork(prepare=None, parent=None, child=None):
    ret = pthread_atfork(prepare, parent, child)
    if ret != 0:
        raise OSError(ret, "Failed to call pthread_atfork()")
