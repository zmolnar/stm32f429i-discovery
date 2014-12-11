/**
 * This file has a different license to the rest of the uGFX system.
 * You can copy, modify and distribute this file as you see fit.
 * You do not need to publish your source modifications to this file.
 * The only thing you are not permitted to do is to relicense it
 * under a different license.
 */

/**
 * Copy this file into your project directory and rename it as gfxconf.h
 * Edit your copy to turn on the uGFX features you want to use.
 * The values below are the defaults. You should delete anything
 * you are leaving as default.
 *
 * Please use spaces instead of tabs in this file.
 */

#ifndef _GFXCONF_H
#define _GFXCONF_H

///////////////////////////////////////////////////////////////////////////
// GDISP                                                                 //
///////////////////////////////////////////////////////////////////////////
#define GFX_USE_GDISP                                TRUE

#define GDISP_NEED_VALIDATION                        TRUE
#define GDISP_NEED_CLIP                              TRUE
#define GDISP_NEED_TEXT                              TRUE
    #define GDISP_INCLUDE_FONT_UI2                   TRUE
#define GDISP_NEED_MULTITHREAD                       TRUE


///////////////////////////////////////////////////////////////////////////
// GWIN                                                                  //
///////////////////////////////////////////////////////////////////////////
#define GFX_USE_GWIN                                 TRUE

#define GWIN_NEED_WINDOWMANAGER                      TRUE

#define GWIN_NEED_CONSOLE                            TRUE
    #define GWIN_CONSOLE_USE_HISTORY                 TRUE
        #define GWIN_CONSOLE_HISTORY_AVERAGING       TRUE
        #define GWIN_CONSOLE_HISTORY_ATCREATE        TRUE
    #define GWIN_CONSOLE_ESCSEQ                      TRUE
    #define GWIN_CONSOLE_USE_BASESTREAM              TRUE
#define GWIN_NEED_GRAPH                              TRUE

#define GWIN_NEED_WIDGET                             TRUE
    #define GWIN_NEED_BUTTON                         TRUE
        #define GWIN_BUTTON_LAZY_RELEASE             TRUE
    #define GWIN_NEED_CHECKBOX                       TRUE
#define GWIN_NEED_CONTAINERS                         TRUE
    #define GWIN_NEED_CONTAINER                      TRUE

#define GWIN_CONSOLE_USE_FLOAT                       TRUE
 

///////////////////////////////////////////////////////////////////////////
// GEVENT                                                                //
///////////////////////////////////////////////////////////////////////////
#define GFX_USE_GEVENT                               TRUE

///////////////////////////////////////////////////////////////////////////
// GTIMER                                                                //
///////////////////////////////////////////////////////////////////////////
#define GFX_USE_GTIMER                               TRUE

///////////////////////////////////////////////////////////////////////////
// GQUEUE                                                                //
///////////////////////////////////////////////////////////////////////////
#define GFX_USE_GQUEUE                               TRUE

#define GQUEUE_NEED_ASYNC                            TRUE
#define GQUEUE_NEED_GSYNC                            TRUE

///////////////////////////////////////////////////////////////////////////
// GINPUT                                                                //
///////////////////////////////////////////////////////////////////////////
#define GFX_USE_GINPUT                               TRUE
#define GINPUT_TOUCH_NOCALIBRATE_GUI                 TRUE

#define GINPUT_NEED_MOUSE                            TRUE

#endif /* _GFXCONF_H */
