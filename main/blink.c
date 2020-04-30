#include <limits.h>
/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO CONFIG_BLINK_GPIO

void myTask1(void * pvParameters )
{
    int val = (int)pvParameters;
    gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while(1) {

        printf("Turning off the LED Value: %d\n", val);
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        printf("Turning on the LED\n");
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void myTask2(void * pvParameters )
{
    int val = (int)pvParameters;
    while(1) {
        printf("Hello from myTask2 Value: %d\n", val);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    TaskHandle_t myTaskHandle1 = NULL;
    xTaskCreate(myTask1, "myTask", 4096, (void*)42, 10, &myTaskHandle1);
    TaskHandle_t myTaskHandle2 = NULL;
    xTaskCreate(myTask2, "myTask2", 4096, (void*)22, 10, &myTaskHandle2);
    for(;;){
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
