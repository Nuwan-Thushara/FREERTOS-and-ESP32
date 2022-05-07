// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif
// Task: Perform some mundane task
void testTask(void *parameter) {
 while (1) {
  int a = 1;
  int b[100];
  // Do something with array so it's not optimized out by the compiler
  for (int i = 0; i < 100; i++) {
   b[i] = a + 1;
  }
  Serial.println(b[3]);

  // print the reamaining stack memory 
  Serial.print("High water mark(words): ");
  Serial.println(uxTaskGetStackHighWaterMark(NULL));
  }
}
void setup() {
  // Configure Serial
  Serial.begin(115200);
  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Memory Demo---");
  // Start the only other task
  xTaskCreatePinnedToCore(
            testTask,
            "Test Task",
            1400,
            NULL,
            1,
            NULL,
            app_cpu);
  // Delete "setup and loop" task
  vTaskDelete(NULL);
}
void loop() {
// Execution should never get here
}
