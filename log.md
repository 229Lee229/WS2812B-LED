Plan:	点灯, 测试按键控制



### Questions

+   如何在keil中使用_Static_assert()?




25/10/2
	该函数提供了一个默认实现（通常是空的或占位性的），但如果在其他源文件中存在同名函数的“强定义”（即没有 __weak 的正常定义），编译器会优先使用用户的强定义来覆盖这个弱版本。

25/10/3
	在STM32的NVIC中断优先级系统中, 优先级值越小,优先级越高.
	注意: 在FreeRTOS中,任务优先级可能相反

	Crystal/Ceramic Oscillator if you use a crystal, BYPASS Clock Source if you use an oscillator





### **assert的使用**

+   标准库需要自定义函数  并包含头文件assert.h  

+   assert()宏是为了标识出程序中某些条件为真的关键位置，如果其中的一个具体条件为假，就用 assert()语句终止程序。如果认为已经排除了程序的 bug，就可以把下面的宏定义写在包含assert.h的位置前面：**#define NDEBUG** 或者在keil中的 C/C++的宏定义中定义全局

    +   ```c
        void __aeabi_assert(const char *expr, const char *file, int line) {
          char msg[128];
          sprintf(msg, "Assert failed: %s, file %s, line %d\r\n", expr, file, line);
          printf("%s",(uint8_t*)msg);
          
          while(1);
        }
        ```

         

### _Static_assert的使用

+   如果不成立, _Static_assert()可以导致程序无法通过编译。
+   _Static_assert要求它的第1个参数是整型常量表达式, 这保证了能在编译期求值(sizeof表达式被视为整型常量)