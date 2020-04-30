#include <limits.h>
/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO CONFIG_BLINK_GPIO

TaskHandle_t myTaskHandle1 = NULL, myTaskHandle2 = NULL;
QueueHandle_t myQueue = NULL;

void myTask1(void * pvParameters )
{
    int val = (int)pvParameters;
    ESP_LOGE("Task1", "My value is %d, setting GPIOs for blink and starting endless loop", val);
    gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while(1) {
        //printf("Turning off the LED Value: %d\n", val);
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        //printf("Turning on the LED\n");
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        int receivedVal;
        xQueueReceive(myQueue, &receivedVal, portMAX_DELAY);
        ESP_LOGW("Task1", "Received queue element with value %d", receivedVal);
    }
}

void myTask2(void * pvParameters )
{
    int val = (int)pvParameters;
    ESP_LOGW("Task2", "My value %d, starting endless loop", val);
    while(1) {
        val++;
        printf("Hello from myTask2 Value: %d\n", val);
        xQueueSend(myQueue, &val, portMAX_DELAY);
    }
}

void app_main(void)
{
    ESP_LOGI("Main", "Ich bin gerade in der Main!");
    myQueue = xQueueCreate(10, sizeof(int));
    xTaskCreatePinnedToCore(myTask1, "myTask1", 4096, (void*)42, 10, &myTaskHandle1, 0);
    xTaskCreatePinnedToCore(myTask2, "myTask2", 4096, (void*)22, 10, &myTaskHandle2, 1);
}
