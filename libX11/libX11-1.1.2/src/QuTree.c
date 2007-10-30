/* $Xorg: QuTree.c,v 1.4 2001/02/09 02:03:35 xorgcvs Exp $ */
/*

Copyright 1986, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.

*/
/* $XFree86: xc/lib/X11/QuTree.c,v 1.5 2001/01/17 19:41:42 dawes Exp $ */

#define NEED_REPLIES
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "Xlibint.h"

Status XQueryTree (
    register Display *dpy,
    Window w,
    Window *root,	/* RETURN */
    Window *parent,	/* RETURN */
    Window **children,	/* RETURN */
    unsigned int *nchildren)  /* RETURN */
{
    long nbytes;
    xQueryTreeReply rep;
    register xResourceReq *req;

    LockDisplay(dpy);
    GetResReq(QueryTree, w, req);
    if (!_XReply(dpy, (xReply *)&rep, 0, xFalse)) {
	UnlockDisplay(dpy);
	SyncHandle();
	return (0);
	}

    *children = (Window *) NULL; 
    if (rep.nChildren != 0) {
	nbytes = rep.nChildren * sizeof(Window);
	*children = (Window *) Xmalloc((unsigned) nbytes);
	nbytes = rep.nChildren << 2;
	if (! *children) {
	    _XEatData(dpy, (unsigned long) nbytes);
	    UnlockDisplay(dpy);
	    SyncHandle();
	    return (0);
	}
	_XRead32 (dpy, (long *) *children, nbytes);
    }
    *parent = rep.parent;
    *root = rep.root;
    *nchildren = rep.nChildren;
    UnlockDisplay(dpy);
    SyncHandle();
    return (1);
}

