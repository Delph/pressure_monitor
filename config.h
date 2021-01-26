#ifndef CONFIG_H_INCLUDE
#define CONFIG_H_INCLUDE


/**
 * The display value is computed from an average of a number of readings
 * This controls how many readings to take to compute the average from
 * The sampling rate is about 120/s, so at 60, it'll take about half a second
 * to get a completely fresh average.
 */
const size_t AVERAGE_NUM_READINGS = 60;


/**
 * The LED and LCD display is updated every this ms
 * This is separate to AVERAGE_NUM_READINGS, every UPDATE_INTERVAL
 * the value currently stored in the averages is just copied to the
 * display and LED
 */
const uint32_t UPDATE_INTERVAL = 500;


#endif
