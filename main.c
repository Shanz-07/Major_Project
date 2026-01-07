#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
const uint led_pin=6;
int main()
{
    stdio_init_all();
    while (!stdio_usb_connected()) 
    {
    sleep_ms(100);
    }
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);
    
    gpio_set_function(led_pin,GPIO_FUNC_PWM);
    uint slice=pwm_gpio_to_slice_num(led_pin);
    pwm_set_enabled(slice,true);
    int mode=0;
    uint16_t duty=0;
    
    printf("Welcome to GreenHouse Monitoring System\n"
       "What Control would you like to proceed with?\n"
       "1) Manual Control\n"
       "2) Automatic Control\n");

    while (true) 
    {

    int c=getchar_timeout_us(0);

    if(c=='1')
    {
        mode=1;
        printf("Manual Mode Selected\n");
        printf("3)Decrease LED Intensity\n");
        printf("4)Increase LED Intensity\n");
    }

    if(c=='2')
    {
        mode=2;
        printf("Automatic Mode Selected\n");
    }

    uint16_t raw=adc_read();
    const float conversion=3.3f/(1<<12);
    float voltage=raw*conversion;
    float temp=27-(voltage-0.706)/0.001721;
    if (mode==2) 
    {
    if (temp<25)
        duty=65535;          
    else if (temp<=27)
        duty=32768;          
    else
        duty=16384;          

    pwm_set_gpio_level(led_pin,duty);

    printf("Temp: %.2f C | LED: %d%%\n",
            temp, (duty * 100) / 65535);
    }
    if(mode==1)
    {
        if(c=='3' && duty>=16384)
        {
            duty-=16384;
        }
        if(c=='4' && duty<=49152)
        {
            duty+=16384;
        }
        pwm_set_gpio_level(led_pin,duty);
        if(c=='3' || c=='4')
        {
        printf("Manual LED Intensity: %d%%\n",(duty*100)/65535);}
    }
    
    sleep_ms(1000);
}
    }

