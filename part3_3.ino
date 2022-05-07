//use only core 1 
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Some string to print from one task
const char msg[] = "this is the task";
static const int led_pin1 = 5;
static const int led_pin2 = 23;
// Task handles

static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

//****************************************************************
//Tasks

//Task 1: blink 500 ms
void startTask1(void *parameter){ // return nothing except a parameter
  while(1){
    Serial.println("task 1");
    digitalWrite(led_pin1, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(led_pin1, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
//Task 2: blink 300 ms
void startTask2(void *parameter){ // return nothing except a parameter
  while(1){
    Serial.println("task 2");
    digitalWrite(led_pin2, HIGH);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    digitalWrite(led_pin2, LOW);
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}
void setup() {
  Serial.begin(300);
  //configure pin
  pinMode(led_pin1, OUTPUT);
  pinMode(led_pin2, OUTPUT);
  
  //TASK TO RUN
  xTaskCreatePinnedToCore( // Use xTaskCreate() in vanilla FreeRTos
              startTask1, //function to be called
              "Task 1", // name of the task
              1024,       // Stack size (bytes in esp32, words in FreeRTOS)
              NULL,       // parameter to pass to function
              1,          // task priority (0 to configMAX_PRIORITIES=1 (24))
              &task_1,       // Task handle
              app_cpu);   // Run on one core
              
   xTaskCreatePinnedToCore( // Use xTaskCreate() in vanilla FreeRTos
              startTask2, //function to be called
              "Task 2", // name of the task
              1024,       // Stack size (bytes in esp32, words in FreeRTOS)
              NULL,       // parameter to pass to function
              2,          // task priority (0 to configMAX_PRIORITIES=1 (24))
              &task_2,       // Task handle
              app_cpu);   // Run on one core
}
void loop() {
  // suspend higher priority task for a interval
  for(int i=0; i<3; i++){
    vTaskSuspend(task_2);
    vTaskDelay(2000/ portTICK_PERIOD_MS);
    vTaskResume(task_2);
    vTaskDelay(2000/ portTICK_PERIOD_MS);  
  }
}
