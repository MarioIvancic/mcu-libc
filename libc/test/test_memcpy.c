
#include <string.h>
#include <stdio.h>
#include "appconfig.h"
#include "libmcu.h"
#include "lpc_uart_magic.h"

#include "sys_init.h"

#ifdef STROBE_LED_TIME
static unsigned strobe_led_timer;
#endif // STROBE_LED_TIME


extern void set_eh_output(void* fcn);
extern void enable_div0_and_unaligned_fault(void);

int main_loop_count;




/* Delay for the specified number of milliSeconds */
void msDelay(uint32_t ms)
{
    unsigned start = _systime_ms();
	while((_systime_ms() - start) < ms) ;
}






void *memcpy_cm3 (void *dst, const void *src, size_t count);
void *memcpy_7m (void *dst, const void *src, size_t count);
void *memcpy_f9 (void *dst, const void *src, size_t count);

static char A[1024];
static char B[1024];

//#define B A+512

void loop_cm3(int ao, int bo, int len)
{
    uint32_t s, e;
    char *a, *b;
    int i;

    for(i = 0, a = A+ao, b = B+bo, s = _systime_tick(); i < 1000/10; i++)
    {
        memcpy_cm3(a, b, len);
        memcpy_cm3(a, b, len);
        memcpy_cm3(a, b, len);
        memcpy_cm3(a, b, len);
        memcpy_cm3(a, b, len);
        memcpy_cm3(a, b, len);
        memcpy_cm3(a, b, len);
        memcpy_cm3(a, b, len);
        memcpy_cm3(a, b, len);
        memcpy_cm3(a, b, len);
    }
    e = _systime_tick();
    printf("1000 * memcpy_m3(%d, %d, %d) = %d x10^-7 sec\r\n", ao, bo, len, e - s);
    //printf("stack: %p\r\n", &a);
}

void loop_7m(int ao, int bo, int len)
{
    uint32_t s, e;
    char *a, *b;
    int i;

    for(i = 0, a = A+ao, b = B+bo, s = _systime_tick(); i < 1000/10; i++)
    {
        memcpy_7m(a, b, len);
        memcpy_7m(a, b, len);
        memcpy_7m(a, b, len);
        memcpy_7m(a, b, len);
        memcpy_7m(a, b, len);
        memcpy_7m(a, b, len);
        memcpy_7m(a, b, len);
        memcpy_7m(a, b, len);
        memcpy_7m(a, b, len);
        memcpy_7m(a, b, len);
    }
    e = _systime_tick();
    printf("1000 * memcpy_7m(%d, %d, %d) = %d x10^-7 sec\r\n", ao, bo, len, e - s);
}


void loop_f9(int ao, int bo, int len)
{
    uint32_t s, e;
    char *a, *b;
    int i;

    for(i = 0, a = A+ao, b = B+bo, s = _systime_tick(); i < 1000/10; i++)
    {
        memcpy_f9(a, b, len);
        memcpy_f9(a, b, len);
        memcpy_f9(a, b, len);
        memcpy_f9(a, b, len);
        memcpy_f9(a, b, len);
        memcpy_f9(a, b, len);
        memcpy_f9(a, b, len);
        memcpy_f9(a, b, len);
        memcpy_f9(a, b, len);
        memcpy_f9(a, b, len);
    }
    e = _systime_tick();
    printf("1000 * memcpy_f9(%d, %d, %d) = %d x10^-7 sec\r\n", ao, bo, len, e - s);
}



void loop(int ao, int bo, int len)
{
    uint32_t s, e;
    char *a, *b;
    int i;

    for(i = 0, a = A+ao, b = B+bo, s = _systime_tick(); i < 1000/10; i++)
    {
        memcpy(a, b, len);
        memcpy(a, b, len);
        memcpy(a, b, len);
        memcpy(a, b, len);
        memcpy(a, b, len);
        memcpy(a, b, len);
        memcpy(a, b, len);
        memcpy(a, b, len);
        memcpy(a, b, len);
        memcpy(a, b, len);
    }
    e = _systime_tick();
    printf("1000 * memcpy(%d, %d, %d) = %d x10^-7 sec\r\n", ao, bo, len, e - s);
}



void test_memcpy(void)
{
    int ao, bo, len;

    printf("memcpy test, A=%p, B=%p\r\n", A, B);

    // testing memcpy_m3
    //for(len = 4; len < 1024; )
    for(len = 4; len < 400; )
    {
        for(ao = 0, bo = 0; ao < 4; ao++)
        {
            loop_cm3(ao, bo, len);
        }
        for(ao = 0, bo = 1; bo < 4; bo++)
        {
            loop_cm3(ao, bo, len);
        }
        switch(len)
        {
            case 4:     len = 10; break;
            case 10:    len = 30; break;
            case 30:    len = 100; break;
            case 100:   len = 300; break;
            case 300:   len = 1000; break;
            case 1000:  len = 2000; break;
        }
    }
#if 0
    // testing memcpy_7m
    //for(len = 4; len < 1024; )
    for(len = 4; len < 400; )
    {
        for(ao = 0, bo = 0; ao < 4; ao++)
        {
            loop_7m(ao, bo, len);
        }
        for(ao = 0, bo = 1; bo < 4; bo++)
        {
            loop_7m(ao, bo, len);
        }
        switch(len)
        {
            case 4:     len = 10; break;
            case 10:    len = 30; break;
            case 30:    len = 100; break;
            case 100:   len = 300; break;
            case 300:   len = 1000; break;
            case 1000:  len = 2000; break;
        }
    }
#endif
    // testing memcpy_f9
    //for(len = 4; len < 1024; )
    for(len = 4; len < 400; )
    {
        for(ao = 0, bo = 0; ao < 4; ao++)
        {
            loop_f9(ao, bo, len);
        }
        for(ao = 0, bo = 1; bo < 4; bo++)
        {
            loop_f9(ao, bo, len);
        }
        switch(len)
        {
            case 4:     len = 10; break;
            case 10:    len = 30; break;
            case 30:    len = 100; break;
            case 100:   len = 300; break;
            case 300:   len = 1000; break;
            case 1000:  len = 2000; break;
        }
    }

    // testing memcpy
    //for(len = 4; len < 1024; )
    for(len = 4; len < 400; )
    {
        for(ao = 0, bo = 0; ao < 4; ao++)
        {
            loop(ao, bo, len);
        }
        for(ao = 0, bo = 1; bo < 4; bo++)
        {
            loop(ao, bo, len);
        }
        switch(len)
        {
            case 4:     len = 10; break;
            case 10:    len = 30; break;
            case 30:    len = 100; break;
            case 100:   len = 300; break;
            case 300:   len = 1000; break;
            case 1000:  len = 2000; break;
        }
    }
}



void test_memcpy_f9(void)
{
    strcpy(A, "0123456789Jedan jako dugacak string, treba da bude bar 33 bajta dug kako bi se moglo sve istestirati");
    size_t len, L = strlen(A);
    int ao, bo, err = 0;
    char *a, *b;

    printf("src=%s\n", A);

    for(bo = 0; bo < 4; bo++)
    {
        for(ao = 0; ao < 4; ao++)
        {
            printf("so=%d, do=%d\n", ao, bo);
            for(len = 4; len <= L + 1; len = 2 * len + len / 2)
            {
                a = A + ao;
                b = B + bo;
                memset(B, 0, sizeof(B));
                memcpy_f9(b, a, len);
                if(len != strlen(b)) err = 1, printf("ERROR len=%d (strlen=%d), str=%s\n", len, strlen(b), b);
                else printf("len=%d, str=%s\n", len, b);
            }
        }
    }

    if(err) printf("Test failed!\n");
    else printf("Test OK!\n");
}

void test_memcpy_cm3(void)
{
    strcpy(A, "0123456789Jedan jako dugacak string, treba da bude bar 33 bajta dug kako bi se moglo sve istestirati");
    size_t len, L = strlen(A);
    int ao, bo, err = 0;
    char *a, *b;

    printf("src=%s, len=%d\n", A, (int)L);

    for(bo = 0; bo < 4; bo++)
    {
        for(ao = 0; ao < 4; ao++)
        {
            printf("so=%d, do=%d\n", ao, bo);
            for(len = 4; len <= L + 1 - bo; len = 2 * len + len / 2)
            {
                int alignment;
                a = A + ao;
                b = B + bo;
                memset(B, 0, sizeof(B));
                alignment = (int) memcpy_cm3(b, a, len);
                if(len != strlen(b)) err = 1, printf("ERROR len=%d (strlen=%d), str=%s\n", (int)len, (int)strlen(b), b);
                else printf("len=%d, str=%s\n", (int)len, b);
            }
        }
    }

    if(err) printf("Test failed!\n");
    else printf("Test OK!\n");
}


int main(void)
{
    sys_init();

    //  wdtInit();			// initialize the watchdog timer

    // free running timer
    lpc_timer_time_init(LPC_TIM0, PCLK / LPC_TIMER_CLK, 0);
    _systime_tick_init((unsigned int(*)(void))lpc_timer_time_get, 32, 1);
    _systime_time_init(MSEC2TIM(1));


    // mora se malo sacekati da reset kolo za 5V otpusti reset liniju
    // naime, 3v3 reset kolo prije dobije stabilan napon pa prije izlazi
    // iz reseta sto opet ima za posljedicu da procesor pocinje da radi
    // dok je 5V reset kolo jos u resetu

    // za reset kola MCP120/130 trajanje reset impulsa je 150 do 700 ms, a za
    // reset kola ADM1811/1813 je 100 do 250 ms, Da bi program radio sa obe
    // vrste reset kola mora se ugraditi kasnjenje od bar 600ms
    //pause(MSEC2TIM(700));
    msDelay(700);


    // UART0, TX=P0.2, RX=P0.3

    #define UART0_PINSEL_MASK ((3 << 4) | (3 << 6))
    #define UART0_PINSEL_VAL  ((1 << 4) | (1 << 6))

    // SSP0 on P0.15, P0.17, P0.18

    #define SSP0_SCK_MASK   (3 << 30)
    #define SSP0_SCK_VAL    (2 << 30)

    PINSEL0 = (PINSEL0 & ~(UART0_PINSEL_MASK | SSP0_SCK_MASK)) | UART0_PINSEL_VAL | SSP0_SCK_VAL;

    #define SSP0_MISO_MASK  (3 << 2)
    #define SSP0_MISO_VAL   (2 << 2)

    #define SSP0_MOSI_MASK  (3 << 4)
    #define SSP0_MOSI_VAL   (2 << 4)

    #define SSP0_PINSEL_MASK (SSP0_MISO_MASK | SSP0_MOSI_MASK)
    #define SSP0_PINSEL_VAL  (SSP0_MISO_VAL | SSP0_MOSI_VAL)

    // UART3, TX=P0.25, RX=P0.26, for SAS
    #define UART3_PINSEL_MASK ((3 << 18) | (3 << 20))
    #define UART3_PINSEL_VAL  ((3 << 18) | (3 << 20))

    PINSEL1 = (PINSEL1 & ~(SSP0_PINSEL_MASK | UART3_PINSEL_MASK) ) | SSP0_PINSEL_VAL | UART3_PINSEL_VAL;
    //PINSEL1 = (PINSEL1 & ~(SSP0_PINSEL_MASK | 0) ) | SSP0_PINSEL_VAL | 0;


    uart0_init(UART_BAUD(115200), UART_8N2, UART_FIFO_14); // setup the UART0

    init_tfp_printf((void(*)(char))uart0_putchar);
    set_eh_output(uart0_putchar);
    enable_div0_and_unaligned_fault();


    DEBUG(INFO, "\r\n\r\nSTARTING\r\n");

    crt0_enable_irq();


	// test libc funkcije
	test_memcpy();
	//memcpy_cm3(A + 0, B + 1, 300);
	//test_memcpy_cm3();




    strobe_led_timer = _systime_ms();

    DEBUG(INFO, "Entering FOR LOOP()\r\n");
    msDelay(100);

	for(;;)
	{
	    main_loop_count++;

		// LEDSTROBE
		if((_systime_ms() - strobe_led_timer) > STROBE_LED_TIME)
		{
			strobe_led_timer = __systime_curr_ms;
			wdt_strobe_led();
        }
	} // for

	return 0;
}



void wdt_strobe_led(void)
{
	static char strled;

	if(strled)
	{
		STROBELED_ON();
		strled = 0;
	}
	else
	{
		STROBELED_OFF();
		strled = 1;
	}
}


void default_isr(void) { ; }




void __aeabi_unwind_cpp_pr0(void){}
void __aeabi_unwind_cpp_pr1(void){}
void __aeabi_unwind_cpp_pr2(void){}
