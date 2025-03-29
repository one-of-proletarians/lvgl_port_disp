#ifndef LV_PORT_DISP_TEMPL_H
#define LV_PORT_DISP_TEMPL_H

#include <stdint.h>
#include <stdbool.h>
#include "st7789.h"
#include "driver/gpio.h"
#include <driver/spi_master.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /*********************
     *      INCLUDES
     *********************/

#include "lvgl.h"

    /*********************
     *      DEFINES
     *********************/

    /**********************
     *      TYPEDEFS
     **********************/

    /**********************
     * GLOBAL PROTOTYPES
     **********************/
    /* Initialize low level display driver */
    void lv_port_disp_init(void);

    /* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
     */
    void disp_enable_update(void);

    /* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
     */
    void disp_disable_update(void);

    /**********************
     *      MACROS
     **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_PORT_DISP_TEMPL_H*/
