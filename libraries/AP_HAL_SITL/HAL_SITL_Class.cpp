/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include <AP_HAL/AP_HAL.h>

#if CONFIG_HAL_BOARD == HAL_BOARD_SITL

#include <assert.h>

#include "AP_HAL_SITL.h"
#include "AP_HAL_SITL_Namespace.h"
#include "HAL_SITL_Class.h"
#include "Scheduler.h"
#include "AnalogIn.h"
#include "UARTDriver.h"
#include "Storage.h"
#include "RCInput.h"
#include "RCOutput.h"
#include "SITL_State.h"
#include "Util.h"

#include <AP_HAL_Empty/AP_HAL_Empty.h>
#include <AP_HAL_Empty/AP_HAL_Empty_Private.h>

using namespace HALSITL;

static EEPROMStorage sitlEEPROMStorage;
static SITL_State sitlState;
static Scheduler sitlScheduler(&sitlState);
static RCInput  sitlRCInput(&sitlState);
static RCOutput sitlRCOutput(&sitlState);
static AnalogIn sitlAnalogIn(&sitlState);

// use the Empty HAL for hardware we don't emulate
static Empty::GPIO emptyGPIO;
static Empty::Semaphore emptyI2Csemaphore;
static Empty::I2CDriver emptyI2C(&emptyI2Csemaphore);
static Empty::SPIDeviceManager emptySPI;
static Empty::OpticalFlow emptyOpticalFlow;

static UARTDriver sitlUart0Driver(0, &sitlState);
static UARTDriver sitlUart1Driver(1, &sitlState);
static UARTDriver sitlUart2Driver(2, &sitlState);
static UARTDriver sitlUart3Driver(3, &sitlState);
static UARTDriver sitlUart4Driver(4, &sitlState);

static Util utilInstance(&sitlState);

HAL_SITL::HAL_SITL() :
    AP_HAL::HAL(
        &sitlUart0Driver,  /* uartA */
        &sitlUart1Driver,  /* uartB */
        &sitlUart2Driver,  /* uartC */
        &sitlUart3Driver,  /* uartD */
        &sitlUart4Driver,  /* uartE */
        &emptyI2C, /* i2c */
        &emptyI2C, /* i2c */
        &emptyI2C, /* i2c */
        &emptySPI, /* spi */
        &sitlAnalogIn, /* analogin */
        &sitlEEPROMStorage, /* storage */
        &sitlUart0Driver, /* console */
        &emptyGPIO, /* gpio */
        &sitlRCInput,  /* rcinput */
        &sitlRCOutput, /* rcoutput */
        &sitlScheduler, /* scheduler */
        &utilInstance, /* util */
        &emptyOpticalFlow), /* onboard optical flow */
    _sitl_state(&sitlState)
{}

void HAL_SITL::run(int argc, char * const argv[], Callbacks* callbacks) const
{
    assert(callbacks);

    _sitl_state->init(argc, argv);
    scheduler->init();
    uartA->begin(115200);

    rcin->init();
    rcout->init();

    //spi->init();
    //i2c->begin();
    //i2c->setTimeout(100);
    analogin->init();

    callbacks->setup();
    scheduler->system_initialized();

    for (;;) {
        callbacks->loop();
    }
}

const AP_HAL::HAL& AP_HAL::get_HAL() {
    static const HAL_SITL hal;
    return hal;
}

#endif // CONFIG_HAL_BOARD == HAL_BOARD_SITL
