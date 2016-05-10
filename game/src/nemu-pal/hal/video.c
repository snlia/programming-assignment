#include "hal.h"
#include "device/video.h"
#include "device/palette.h"

#include <string.h>
#include <stdlib.h>
#define MMAX(a,b) ((a) > (b) ? (a) : (b))
#define MMIN(a,b) ((a) < (b) ? (a) : (b))

int get_fps();

/*
 * typedef struct{
 *          Sint16 x, y;
 *          Uint16 w, h;
 *      } SDL_Rect;
 *
 * typedef struct SDL_Surface {
 *             Uint32 flags;                           * Read-only *
 *             SDL_PixelFormat *format;                * Read-only *
 *             int w, h;                               * Read-only *
 *             Uint16 pitch;                           * Read-only *
 *             void *pixels;                           * Read-write *
 *
 *             * clipping information *
 *             SDL_Rect clip_rect;                     * Read-only *
 *
 *             * Reference count -- used when freeing surface *
 *             int refcount;                           * Read-mostly *
 *
 *          * This structure also contains private fields not shown here *
 *      } SDL_Surface;
 * */

inline static uint32_t get_idx (int x, int y, int w, int h) {
    return y * w + x;
}

void SDL_BlitSurface(SDL_Surface *scr, SDL_Rect *scrrect, 
		SDL_Surface *dst, SDL_Rect *dstrect) {
//    Log ("start BlitSurface");
	assert(dst && scr);

	/* TODO: Performs a fast blit from the source surface to the 
	 * dstination surface. Only the position is used in the
	 * ``dstrect'' (the width and height are ignored). If either
	 * ``scrrect'' or ``dstrect'' are NULL, the entire surface 
	 * (``scr'' or ``dst'') is copied. The final blit rectangle 
	 * is saved in ``dstrect'' after all clipping is performed
	 * (``scrrect'' is not modified).
	 */

    int sx = 0, sy = 0, dx = 0, dy = 0, w = scr->w, h = scr->h;
    if (scrrect) {
        sx = scrrect->x;
        sy = scrrect->y;
        w = scrrect->w;
        h = scrrect->h;
    }
    if (dstrect) {
        dx = dstrect->x;
        dy = dstrect->y;
        dstrect->w = w;
        dstrect->h = h;
    }

    w = MMIN (w, dst->w - dx);
    h = MMIN (h, dst->h - dy);

    w = MMIN (w, scr->w - sx);
    h = MMIN (h, scr->h - sy);

    Log ("start BlitSurface %d %d %d %d %d %d %d %d %d %d\n", sx, sy, scr->w, scr->h, dx, dy, dst->w, dst->h, w, h);
//    195 7 320 200 0 0 112 34 112 34

    uint8_t* spixel = scr->pixels;
    uint8_t* dpixel = dst->pixels;
    printf ("%x %x\n", (uint32_t)spixel, (uint32_t) dpixel);

    for (int i = 0; i < h; ++i) {
        memcpy (dpixel + get_idx (dx, dy + i, dst->w, dst->h), spixel + get_idx (sx, sy + i, scr->w, scr->h), w);
    }
    dstrect->w = w;
    dstrect->h = h;
    Log ("end BlitSurface");
}

void SDL_FillRect(SDL_Surface *dst, SDL_Rect *dstrect, uint32_t color) {
    //Log ("start FillRect");
	assert(dst);
	assert(color <= 0xff);

	/* TODO: Fill the rectangle area described by ``dstrect''
	 * in surface ``dst'' with color ``color''. If dstrect is
	 * NULL, fill the whole surface.
	 */
    int x = 0, y = 0, w = dst->w, h = dst->h;
    if (dstrect) {
        x = dstrect->x;
        y = dstrect->y;
        h = dstrect->h; 
        w = dstrect->w;
    }

    w = MMIN (w, dst->w - x);
    h = MMIN (h, dst->h - y);

    uint8_t* pixel = dst->pixels;
    for (int i = 0; i < h; ++i)
        memset (pixel + get_idx (x, i + y, dst->w, dst->h), color, w);

    //Log ("end FillRect");
}

void SDL_UpdateRect(SDL_Surface *screen, int x, int y, int w, int h) {
    //Log ("start UpdateRect");
	assert(screen);
    assert(screen->pitch == 320);
    if(screen->flags & SDL_HWSURFACE) {
        if(x == 0 && y == 0) {
            /* Draw FPS */
            vmem = VMEM_ADDR;
            char buf[80];
            sprintf(buf, "%dFPS", get_fps());
            draw_string(buf, 0, 0, 10);
        }
        return ;
    }

    /* TODO: Copy the pixels in the rectangle area to the screen. */
    w = MMIN (w, 320 - x);
    h = MMIN (h, 200 - y);

    uint8_t* vmem = (uint8_t *) VMEM_ADDR;
    uint8_t* pixel = screen->pixels;
    for (int j = 0; j < h; ++j)
        memcpy (vmem + get_idx (x, y + j, 320, 200), pixel + get_idx (x, y + j, 320, 200), w);

    //Log ("end UpdateRect");
}

void SDL_SetPalette(SDL_Surface *s, int flags, SDL_Color *colors, 
        int firstcolor, int ncolors) {
    //Log ("start SetPlatte");
    assert(s);
    assert(s->format);
    assert(s->format->palette);
    assert(firstcolor == 0);

    if(s->format->palette->colors == NULL || s->format->palette->ncolors != ncolors) {
        if(s->format->palette->ncolors != ncolors && s->format->palette->colors != NULL) {
            /* If the size of the new palette is different 
             * from the old one, free the old one.
             */
            free(s->format->palette->colors);
        }

        /* Get new memory space to store the new palette. */
        s->format->palette->colors = malloc(sizeof(SDL_Color) * ncolors);
        assert(s->format->palette->colors);
    }

    /* Set the new palette. */
    s->format->palette->ncolors = ncolors;
    memcpy(s->format->palette->colors, colors, sizeof(SDL_Color) * ncolors);

    if(s->flags & SDL_HWSURFACE) {
        /* TODO: Set the VGA palette by calling write_palette(). */
        write_palette (colors, ncolors);
    }
    //Log ("end SetPlatte");
}

/* ======== The following functions are already implemented. ======== */

void SDL_SoftStretch(SDL_Surface *scr, SDL_Rect *scrrect, 
        SDL_Surface *dst, SDL_Rect *dstrect) {
    assert(scr && dst);
    int x = (scrrect == NULL ? 0 : scrrect->x);
    int y = (scrrect == NULL ? 0 : scrrect->y);
    int w = (scrrect == NULL ? scr->w : scrrect->w);
    int h = (scrrect == NULL ? scr->h : scrrect->h);

    assert(dstrect);
    if(w == dstrect->w && h == dstrect->h) {
        /* The source rectangle and the dstination rectangle
         * are of the same size. If that is the case, there
         * is no need to stretch, just copy. */
        SDL_Rect rect;
        rect.x = x;
        rect.y = y;
        rect.w = w;
        rect.h = h;
        SDL_BlitSurface(scr, &rect, dst, dstrect);
    }
    else {
        /* No other case occurs in NEMU-PAL. */
        assert(0);
    }
}

SDL_Surface* SDL_CreateRGBSurface(uint32_t flags, int width, int height, int depth,
        uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask) {
    SDL_Surface *s = malloc(sizeof(SDL_Surface));
    assert(s);
    s->format = malloc(sizeof(SDL_PixelFormat));
    assert(s);
    s->format->palette = malloc(sizeof(SDL_Palette));
    assert(s->format->palette);
    s->format->palette->colors = NULL;

    s->format->BitsPerPixel = depth;

    s->flags = flags;
    s->w = width;
    s->h = height;
    s->pitch = (width * depth) >> 3;
    s->pixels = (flags & SDL_HWSURFACE ? (void *)VMEM_ADDR : malloc(s->pitch * height));
    assert(s->pixels);

    return s;
}

SDL_Surface* SDL_SetVideoMode(int width, int height, int bpp, uint32_t flags) {
    return SDL_CreateRGBSurface(flags,  width, height, bpp,
            0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
}

void SDL_FreeSurface(SDL_Surface *s) {
    if(s != NULL) {
        if(s->format != NULL) {
            if(s->format->palette != NULL) {
                if(s->format->palette->colors != NULL) {
                    free(s->format->palette->colors);
                }
                free(s->format->palette);
            }

            free(s->format);
        }

        if(s->pixels != NULL) {
            free(s->pixels);
        }

        free(s);
    }
}

