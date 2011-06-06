#include "kgio.h"

static ID id_wait_rd, id_wait_wr;

/*
 * avoiding rb_thread_select() or similar since rb_io_wait_*able can be
 * made to use poll() later on.  It's highly unlikely Ruby will move to
 * use an edge-triggered event notification, so assigning EAGAIN is
 * probably safe...
 */


/*
 * Blocks the running Thread indefinitely until +self+ IO object is readable.
 * This method is automatically called by default whenever kgio_read needs
 * to block on input.
 *
 * Users of alternative threading/fiber libraries are
 * encouraged to override this method in their subclasses or modules to
 * work with their threading/blocking methods.
 */
static VALUE kgio_wait_readable(VALUE self)
{
	int fd = my_fileno(self);

	errno = EAGAIN;
	if (!rb_io_wait_readable(fd))
		rb_sys_fail("kgio_wait_readable");

	return self;
}

/*
 * Blocks the running Thread indefinitely until +self+ IO object is writable.
 * This method is automatically called whenever kgio_write needs to
 * block on output.
 *
 * Users of alternative threading/fiber libraries are
 * encouraged to override this method in their subclasses or modules to
 * work with their threading/blocking methods.
 */
static VALUE kgio_wait_writable(VALUE self)
{
	int fd = my_fileno(self);

	errno = EAGAIN;
	if (!rb_io_wait_writable(fd))
		rb_sys_fail("kgio_wait_writable");

	return self;
}

VALUE kgio_call_wait_writable(VALUE io)
{
	return rb_funcall(io, id_wait_wr, 0, 0);
}

VALUE kgio_call_wait_readable(VALUE io)
{
	return rb_funcall(io, id_wait_rd, 0, 0);
}

void init_kgio_wait(void)
{
	VALUE mKgio = rb_define_module("Kgio");

	/*
	 * Document-module: Kgio::DefaultWaiters
	 *
	 * This module contains default kgio_wait_readable and
	 * kgio_wait_writable methods that block indefinitely (in a
	 * thread-safe manner) until an IO object is read or writable.
	 * This module is included in the Kgio::PipeMethods and
	 * Kgio::SocketMethods modules used by all bundled IO-derived
	 * objects.
	 */
	VALUE mWaiters = rb_define_module_under(mKgio, "DefaultWaiters");

	id_wait_rd = rb_intern("kgio_wait_readable");
	id_wait_wr = rb_intern("kgio_wait_writable");

	rb_define_method(mWaiters, "kgio_wait_readable",
	                 kgio_wait_readable, 0);
	rb_define_method(mWaiters, "kgio_wait_writable",
	                 kgio_wait_writable, 0);
}
