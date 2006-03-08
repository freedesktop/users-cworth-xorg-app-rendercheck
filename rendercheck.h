/*
 * $Id$
 *
 * Copyright © 2004 Eric Anholt
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Eric Anholt not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Eric Anholt makes no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * ERIC ANHOLT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL ERIC ANHOLT BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include <X11/extensions/Xrender.h>

#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

typedef struct _color4d
{
	double r, g, b, a;
} color4d;

typedef struct _picture_info {
	Drawable d;
	Picture pict;
	XRenderPictFormat *format;
	char *name;		/* Possibly, some descriptive name. */
	color4d color;		/* If a 1x1R pict, the (corrected) color.*/
} picture_info;

struct op_info {
	int op;
	char *name;
};

#define TEST_FILL		0x0001
#define TEST_DSTCOORDS		0x0002
#define TEST_SRCCOORDS		0x0004
#define TEST_MASKCOORDS		0x0008
#define TEST_TSRCCOORDS		0x0010
#define TEST_TMASKCOORDS	0x0020
#define TEST_BLEND		0x0040
#define TEST_COMPOSITE		0x0080
#define TEST_CACOMPOSITE	0x0100
#define TEST_GRADIENTS  	0x0200

extern int pixmap_move_iter;
extern int win_width, win_height;
extern struct op_info ops[];
extern Bool is_verbose;
extern color4d colors[];
extern int enabled_tests;

/* main.c */
void
describe_format(char *desc, int len, XRenderPictFormat *format);

/* tests.c */
void
color_correct(picture_info *pi, color4d *color);

void
get_pixel(Display *dpy, picture_info *pi, int x, int y, color4d *color);

int
eval_diff(char *name, color4d *expected, color4d *test, int x, int y,
    Bool verbose);

void
argb_fill(Display *dpy, picture_info *p, int x, int y, int w, int h, float a,
    float r, float g, float b);

void
begin_test(Display *dpy, picture_info *win);

/* ops.c */
void
do_composite(int op, color4d *src, color4d *mask, color4d *dst, color4d *result,
    Bool componentAlpha);

/* The tests */
Bool
blend_test(Display *dpy, picture_info *win, picture_info *dst, int op,
    picture_info *src_color, picture_info *dst_color);

Bool
composite_test(Display *dpy, picture_info *win, picture_info *dst, int op,
    picture_info *src_color, picture_info *mask_color, picture_info *dst_color,
    Bool componentAlpha, Bool print_errors);

Bool
dstcoords_test(Display *dpy, picture_info *win, picture_info *dst,
    picture_info *bg, picture_info *fg);

Bool
fill_test(Display *dpy, picture_info *win, picture_info *src);

Bool
srccoords_test(Display *dpy, picture_info *win, picture_info *white,
    Bool test_mask);

Bool
trans_coords_test(Display *dpy, picture_info *win, picture_info *white,
    Bool test_mask);

Bool linear_gradient_test(Display *dpy, picture_info *win,
                          picture_info *dst, int op, picture_info *dst_color);
