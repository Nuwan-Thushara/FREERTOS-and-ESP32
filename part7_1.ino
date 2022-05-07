//core definitions
static const BaseType_t pro_cpu = 0;
static const BaseType_t app_cpu = 1;

// tasks

void TaskA(void *parameters){
  //TickType_t timestamp;
  char str[20];

  //Do forever
  while(1){
    //say somthing
    sprintf(str, "I am Task A I am Running on CORE %i\r\n", xPortGetCoreID());
    Serial.print(str);
  }
}

// Task for Higher Priority
void TaskB(void *parameters){
  //TickType_t timestamp;
  char str[20];

  //Do forever
  while(1){
    //say somthing
    sprintf(str, "I am Task B I am Running on CORE %i\r\n", xPortGetCoreID());
    Serial.print(str);
  }
}
//*****************************************************************


// Main (runs as its own task with priority 1 on core 1)
void setup() {
  
  // Configure Serial
  Serial.begin(300);
  
  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Race Condition Demo---");

  // Start task 1
  xTaskCreatePinnedToCore(
            TaskA,
            "Task A",
            2048,
            NULL,
            1,
            NULL,
            pro_cpu);

  // Start task 2
  xTaskCreatePinnedToCore(
            TaskB,
            "Task B",
            2048,
            NULL,
            2,
            NULL,
            app_cpu);
            
}

void loop(){
    // Execution should never get here
}
