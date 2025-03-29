

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"

/*********************
 *      DEFINES
 *********************/

#define DISP_HOR_RES 320
#define DISP_VER_RES 172

#define BYTE_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB565)) /*will be 2 for RGB565 */
#define BUFFER_HEIGHT 80

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map);

st7789_conf_t st7789_conf = {
    .pins = {
        .mosi = GPIO_NUM_6,
        .sclk = GPIO_NUM_7,
        .reset = GPIO_NUM_21,
        .bl = GPIO_NUM_22,
        .cs = GPIO_NUM_14,
        .dc = GPIO_NUM_15,
    },
    .display = {
        .width = DISP_HOR_RES,
        .height = DISP_VER_RES,
        .offset_x = 0,
        .offset_y = 34,
        .rotation = ST7789_ROTATION_90,
    },
    .clock_speed_hz = SPI_MASTER_FREQ_20M,
    .spi_host = SPI2_HOST,

};

void tick(void *p)
{
    for (;;)
    {
        lv_tick_inc(1);
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

void handle(void *p)
{
    TickType_t pxPreviousWakeTime = xTaskGetTickCount();
    for (;;)
    {
        lv_timer_handler();
        vTaskDelayUntil(&pxPreviousWakeTime, pdMS_TO_TICKS(10));
    }
}

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    lv_init();
    disp_init();

    /*------------------------------------
     * Create a display and set a flush_cb
     * -----------------------------------*/
    lv_display_t *disp = lv_display_create(DISP_HOR_RES, DISP_VER_RES);
    lv_display_set_flush_cb(disp, disp_flush);

    /* Example 2
     * Two buffers for partial rendering
     * In flush_cb DMA or similar hardware should be used to update the display in the background.*/
    LV_ATTRIBUTE_MEM_ALIGN
    static uint8_t buff_1[DISP_HOR_RES * BUFFER_HEIGHT * BYTE_PER_PIXEL];

    LV_ATTRIBUTE_MEM_ALIGN
    static uint8_t buf_2[DISP_HOR_RES * BUFFER_HEIGHT * BYTE_PER_PIXEL];
    lv_display_set_buffers(disp, buff_1, buf_2, sizeof(buff_1), LV_DISPLAY_RENDER_MODE_PARTIAL);

    /*-------------------
     * Create lvgl tasks
     * ------------------*/

    xTaskCreate(tick, "lv tick task", 2048, NULL, 1, NULL);
    xTaskCreate(handle, "lv timer task", 4096, NULL, 1, NULL);

    vTaskDelay(pdMS_TO_TICKS(100));
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    st7789_init(&st7789_conf);
}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

static void disp_flush(lv_display_t *disp_drv, const lv_area_t *area, uint8_t *px_map)
{

    if (disp_flush_enabled)
    {
        st7789_set_window(&st7789_conf, area->x1, area->x2, area->y1, area->y2);
        st7789_write_data(&st7789_conf, (uint16_t *)px_map, lv_area_get_size(area));
    }

    lv_display_flush_ready(disp_drv);
}
