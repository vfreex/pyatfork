#include <Python.h>

#include "atfork.h"

#include <pthread.h>


struct catfork_callback_list {
    struct catfork_callback_list *prev, *next;
    PyObject *func;
};

static struct catfork_callback_list prepare_callbacks = { .prev = &prepare_callbacks, .next = &prepare_callbacks, .func = NULL  },
                                      parent_callbacks = { .prev = &parent_callbacks, .next = &parent_callbacks, .func = NULL  },
                                      child_callbacks = { .prev = &child_callbacks, .next = &child_callbacks, .func = NULL  };


static void catfork_callback_append(struct catfork_callback_list *prev, struct catfork_callback_list *node)
{
    assert(prev);
    assert(node);
    node->prev = prev;
    node->next = prev->next;
    prev->next = node;
    node->next->prev = node;
}

static void catfork_callback_remove(const struct catfork_callback_list *node)
{
    assert(node);
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

static void catfork_prepare_wrapper(void)
{
    struct catfork_callback_list *node = prepare_callbacks.next;
    assert(node != &prepare_callbacks && node->func != NULL);
    catfork_callback_remove(node);
    catfork_callback_append(prepare_callbacks.prev, node);
    PyObject_CallObject(node->func, NULL);
}

static void catfork_parent_wrapper(void)
{
    struct catfork_callback_list *node = parent_callbacks.next;
    assert(node != &parent_callbacks && node->func != NULL);
    catfork_callback_remove(node);
    catfork_callback_append(parent_callbacks.prev, node);
    PyObject_CallObject(node->func, NULL);
}

static void catfork_child_wrapper(void)
{
    struct catfork_callback_list *node = child_callbacks.next;
    assert(node != &child_callbacks && node->func != NULL);
    catfork_callback_remove(node);
    catfork_callback_append(child_callbacks.prev, node);
    PyObject_CallObject(node->func, NULL);
}


/*
 * Wrapper function for pthread_atfork
 * This function is not thread-safe!
 * int pthread_atfork(void (*prepare)(void), void (*parent)(void),
 *            void (*child)(void));
 * See pthread_atfork(3)
 */
PyObject *catfork_pthread_atfork(PyObject *self, PyObject *args)
{
    PyObject *prepare, *parent, *child;
    struct catfork_callback_list *prepare_node = NULL,
                                   *parent_node = NULL,
                                   *child_node = NULL;
    int err;
    PyObject *retval = NULL;
    void (*prepare_wrapper)(void) = NULL;
    void (*parent_wrapper)(void) = NULL;
    void (*child_wrapper)(void) = NULL;
    if (!PyArg_ParseTuple(args, "OOO", &prepare, &parent, &child))
        return NULL;
    
    if (PyCallable_Check(prepare)) {
        prepare_node = malloc(sizeof(struct catfork_callback_list));
        if (!prepare_node)
            goto on_error;
        prepare_node->func = prepare;
        Py_INCREF(prepare);
        catfork_callback_append(&prepare_callbacks, prepare_node);
        prepare_wrapper = catfork_prepare_wrapper;
    }
    else if (prepare != Py_None)
        goto on_error;

    if (PyCallable_Check(parent)) {
        parent_node = malloc(sizeof(struct catfork_callback_list));
        if (!parent_node)
            goto on_error;
        parent_node->func = parent;
        Py_INCREF(parent);
        catfork_callback_append(parent_callbacks.prev, parent_node);
        parent_wrapper = catfork_parent_wrapper;
    }
    else if (parent != Py_None)
        goto on_error;
        
    if (PyCallable_Check(child)) {
        child_node = malloc(sizeof(struct catfork_callback_list));
        if (!child_node)
            goto on_error;
        child_node->func = child;
        Py_INCREF(child);
        catfork_callback_append(child_callbacks.prev, child_node);
        child_wrapper = catfork_child_wrapper;
    }
    else if (child != Py_None)
        goto on_error;
    err = pthread_atfork(prepare_wrapper, parent_wrapper, child_wrapper);
    retval = PyLong_FromLong(err);
    if (err)
        goto on_error;
    return retval;
on_error:
    if (prepare_node) {
        catfork_callback_remove(prepare_node);
        Py_DECREF(prepare);
        free(prepare_node);
    }
    if (parent_node) {
        catfork_callback_remove(parent_node);
        Py_DECREF(parent);
        free(parent_node);
    } 
    if (child_node) {
        catfork_callback_remove(child_node);
        Py_DECREF(child);
        free(child_node);
    } 
 return retval;   
}


