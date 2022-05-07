//use only core 1 
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

//pins
static const int led_pin = 2;

//task : blink an LED
void toggleLED(void *parameter){ // return nothing except a parameter
  while(1){
    digitalWrite(led_pin, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
void setup() {
  //configure pin
  pinMode(led_pin, OUTPUT);
  
  //TASK TO RUN
  xTaskCreatePinnedToCore( // Use xTaskCreate() in vanilla FreeRTos
              toggleLED, //function to be called
              "Toggle LED", // name of the task
              1024,       // Stack size (bytes in esp32, words in FreeRTOS)
              NULL,       // parameter to pass to function
              1,          // task priority (0 to configMAX_PRIORITIES=1 (24))
              NULL,       // Task handle
              app_cpu);   // Run on one core
}

void loop() {
  // put your main code here, to run repeatedly:

}
