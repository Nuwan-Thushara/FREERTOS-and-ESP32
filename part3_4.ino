#include <stdlib.h>
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

static const int led_pin = 2;
static int led_delay = 500;  

//*************************************************
// Tasks

// Task: Blink LED at rate set by global variable
void toggleLED(void *parameter) {
  while (1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
  }
}
// second task to  read the serial
void readSerial(void *parameters) {

  char c;
  char Arr[10];
  int i = 0;

  // Clear whole buffer
  memset(Arr, 0, 10);

  while (1) {

    // Read serial charactersl
    if (Serial.available() > 0) {
      c = Serial.read();

      //If we get a newline character, we should update the delay variable and reset the Array.
      if (c == '\n') {
        led_delay = atoi(Arr);
        Serial.print("Updated LED delay to: ");
        Serial.println(led_delay);
        memset(Arr, 0, 10);
        i = 0;
      } else {
        
       // This occurs only if the index does not exceed the message limit.
       if (i< 10 - 1) {
         Arr[i] = c;
         i++;
        }
      }
    }
  }
}

void setup() {

  // Configure pin
  pinMode(led_pin, OUTPUT);

  // Configure serial and wait a second
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("Enter a number in milliseconds to change the LED delay.");

  // Start blink task
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
            toggleLED,      // Function to be called
            "Toggle LED",   // Name of task
            1024,           // Stack size (bytes in ESP32, words in FreeRTOS)
            NULL,           // Parameter to pass
            1,              // Task priority
            NULL,           // Task handle
            app_cpu);       // Run on one core for demo purposes (ESP32 only)
            
  // Start serial read task
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
            readSerial,     // Function to be called
            "Read Serial",  // Name of task
            1024,           // Stack size (bytes in ESP32, words in FreeRTOS)
            NULL,           // Parameter to pass
            1,              // Task priority (must be same to prevent lockup)
            NULL,           // Task handle
            app_cpu);       // Run on one core for demo purposes (ESP32 only)

  vTaskDelete(NULL);
}

void loop() {
  
}
