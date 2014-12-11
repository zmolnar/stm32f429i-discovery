/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

/**
 * @file    src/ginput/sys_options.h
 * @brief   GINPUT sub-system options header file.
 *
 * @addtogroup GINPUT
 * @{
 */

#ifndef _GINPUT_OPTIONS_H
#define _GINPUT_OPTIONS_H

/**
 * @name    GINPUT Functionality to be included
 * @{
 */
	/**
	 * @brief   Should mouse/touch functions be included.
	 * @details	Defaults to FALSE
	 * @note	Also add the a mouse/touch hardware driver to your makefile.
	 * 			Eg.
	 * 				include $(GFXLIB)/drivers/ginput/touch/MCU/ginput_lld.mk
	 */
	#ifndef GINPUT_NEED_MOUSE
		#define GINPUT_NEED_MOUSE		FALSE
	#endif
	/**
	 * @brief   Should keyboard functions be included.
	 * @details	Defaults to FALSE
	 * @note	Also add the a keyboard hardware driver to your makefile.
	 * 			Eg.
	 * 				include $(GFXLIB)/drivers/ginput/keyboard/XXXX/ginput_lld.mk
	 */
	#ifndef GINPUT_NEED_KEYBOARD
		#define GINPUT_NEED_KEYBOARD	FALSE
	#endif
	/**
	 * @brief   Should hardware toggle/switch/button functions be included.
	 * @details	Defaults to FALSE
	 * @note	Also add the a toggle hardware driver to your makefile.
	 * 			Eg.
	 * 				include $(GFXLIB)/drivers/ginput/toggle/Pal/ginput_lld.mk
	 */
	#ifndef GINPUT_NEED_TOGGLE
		#define GINPUT_NEED_TOGGLE		FALSE
	#endif
	/**
	 * @brief   Should analog dial functions be included.
	 * @details	Defaults to FALSE
	 * @note	Also add the a dial hardware driver to your makefile.
	 * 			Eg.
	 * 				include $(GFXLIB)/drivers/ginput/dial/analog/ginput_lld.mk
	 */
	#ifndef GINPUT_NEED_DIAL
		#define GINPUT_NEED_DIAL		FALSE
	#endif
/**
 * @}
 *
 * @name    GINPUT Optional Sizing Parameters
 * @{
 */
/**
 * @}
 *
 * @name    GINPUT Optional Low Level Driver Defines
 * @{
 */
	/**
	 * @brief   Start touch devices without loading or running calibration.
	 * @details	Defaults to FALSE
	 * @note	This is used if you want to manually control the initial calibration
	 * 			process. In practice this is only useful for a touch driver test program.
	 */
	#ifndef GINPUT_TOUCH_STARTRAW
		#define GINPUT_TOUCH_STARTRAW					FALSE
	#endif
 	/**
	 * @brief   Turn off the touch calibration GUI.
	 * @details	Defaults to FALSE
	 * @note	Turning off the calibration GUI just turns off the manual calibration
	 * 			process. Readings may still be calibrated if calibration data
	 * 			can be loaded.
	 * @note	Calibration requires a lot of code. If your device doesn't require it
	 * 			using this option can save a lot of space.
	 */
	#ifndef GINPUT_TOUCH_NOCALIBRATE_GUI
		#define GINPUT_TOUCH_NOCALIBRATE_GUI			FALSE
	#endif
	/**
	 * @brief   Turn off all touch calibration support.
	 * @details	Defaults to FALSE
	 * @note	With this set to TRUE touch readings will not be calibrated.
	 * @note	This automatically turns off the calibration GUI too!
	 * @note	Calibration requires a lot of code. If your device doesn't require it
	 * 			using this option can save a lot of space.
	 */
	#ifndef GINPUT_TOUCH_NOCALIBRATE
		#define GINPUT_TOUCH_NOCALIBRATE				FALSE
	#endif
	/**
	 * @brief   Turn off all touch support.
	 * @details	Defaults to FALSE
	 * @note	This automatically turns off all calibration and the calibration GUI too!
	 * @note	Touch device handling requires a lot of code. If your device doesn't require it
	 * 			using this option can save a lot of space.
	 */
	#ifndef GINPUT_TOUCH_NOTOUCH
		#define GINPUT_TOUCH_NOTOUCH					FALSE
	#endif
	/**
	 * @brief   Milliseconds between mouse polls.
	 * @details	Defaults to 25 millseconds
	 * @note	How often mice should be polled. More often leads to smoother mouse movement
	 * 			but increases CPU usage.
	 */
	#ifndef GINPUT_MOUSE_POLL_PERIOD
		#define GINPUT_MOUSE_POLL_PERIOD				25
	#endif

	/**
	 * @brief   Maximum length of CLICK in milliseconds
	 * @details	Defaults to 300 millseconds
	 * @note	Mouse down to Mouse up times greater than this are not clicks.
	 */
	#ifndef GINPUT_MOUSE_CLICK_TIME
		#define GINPUT_MOUSE_CLICK_TIME					300
	#endif
	/**
	 * @brief   Milliseconds to generate a CXTCLICK on a touch device.
	 * @details	Defaults to 700 millseconds
	 * @note	If you hold the touch down for longer than this a CXTCLICK is generated
	 * 			but only on a touch device.
	 */
	#ifndef GINPUT_TOUCH_CXTCLICK_TIME
		#define GINPUT_TOUCH_CXTCLICK_TIME				700
	#endif
   /**
     * @brief   There is a user supplied routine to load mouse calibration data
	 * @details	Defaults to FALSE
     * @note    If TRUE the user must supply the @p LoadMouseCalibration() routine.
     */
	#ifndef GINPUT_TOUCH_USER_CALIBRATION_LOAD
		#define GINPUT_TOUCH_USER_CALIBRATION_LOAD		FALSE
	#endif
   /**
     * @brief   The buffer returned by the users @p LoadMouseCalibration() routine must be gfxFree()'d
	 * 			by the mouse code.
	 * @details	Defaults to FALSE
     */
	#ifndef GINPUT_TOUCH_USER_CALIBRATION_FREE
		#define GINPUT_TOUCH_USER_CALIBRATION_FREE		FALSE
	#endif
   /**
     * @brief   There is a user supplied routine to save mouse calibration data
	 * @details	Defaults to FALSE
     * @note    If TRUE the user must supply the @p SaveMouseCalibration() routine.
     */
	#ifndef GINPUT_TOUCH_USER_CALIBRATION_SAVE
		#define GINPUT_TOUCH_USER_CALIBRATION_SAVE		FALSE
	#endif
/** @} */

#endif /* _GINPUT_OPTIONS_H */
/** @} */
