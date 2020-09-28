#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char A[1024], B[1024];
//void *memcpy_m3 (void *dst, const void *src, size_t count);
//void *memcpy_7m (void *dst, const void *src, size_t count);
void *memcpy_f9 (void *dst, const void *src, size_t count);
void *memcpy_fast (void *dst, const void *src, size_t count);

#if 0
void loop_cm3(int ao. bo, len)
{
    uint32_t, s, e;
    char *a, *b;
    int i;

    for(i = 0, a = A+ao, b = B+bo, s = systime_tick(); i < 1000/10; i++)
    {
        memcpy_m3(a, b, len);
        memcpy_m3(a, b, len);
        memcpy_m3(a, b, len);
        memcpy_m3(a, b, len);
        memcpy_m3(a, b, len);
        memcpy_m3(a, b, len);
        memcpy_m3(a, b, len);
        memcpy_m3(a, b, len);
        memcpy_m3(a, b, len);
        memcpy_m3(a, b, len);
    }
    e = systime_tick();
    printf("1000 * memcpy_m3(%d, %d, %d) = %d x10^-7 sec\r\n", ao, bo, len, e - s);
}

void loop_7m(int ao. bo, len)
{
    uint32_t, s, e;
    char *a, *b;
    int i;

    for(i = 0, a = A+ao, b = B+bo, s = systime_tick(); i < 1000/10; i++)
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
    e = systime_tick();
    printf("1000 * memcpy_7m(%d, %d, %d) = %d x10^-7 sec\r\n", ao, bo, len, e - s);
}


void loop_f9(int ao. bo, len)
{
    uint32_t, s, e;
    char *a, *b;
    int i;

    for(i = 0, a = A+ao, b = B+bo, s = systime_tick(); i < 1000/10; i++)
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
    e = systime_tick();
    printf("1000 * memcpy_f9(%d, %d, %d) = %d x10^-7 sec\r\n", ao, bo, len, e - s);
}



void test_memcpy(void)
{
    int ao bo, len;

    // testing memcpy_m3
    for(len = 4; len < 1024; )
    {
        for(ao = 0, bo = 0; ao < 4; ao++)
        {
            loop_cm3(ao, bo, len);
        }
        for(ao = 0, bo = 0; bo < 4; bo++)
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

    // testing memcpy_7m
    for(len = 4; len < 1024; )
    {
        for(ao = 0, bo = 0; ao < 4; ao++)
        {
            loop_7m(ao, bo, len);
        }
        for(ao = 0, bo = 0; bo < 4; bo++)
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

    // testing memcpy_f9
    for(len = 4; len < 1024; )
    {
        for(ao = 0, bo = 0; ao < 4; ao++)
        {
            loop_f9(ao, bo, len);
        }
        for(ao = 0, bo = 0; bo < 4; bo++)
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
}
#endif


void test_memcpy_fast(void)
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
                memcpy_fast(b, a, len);
                if(len != strlen(b)) err = 1, printf("ERROR len=%d (strlen=%d), str=%s\n", len, strlen(b), b);
                else printf("len=%d, str=%s\n", len, b);
            }
        }
    }

    if(err) printf("Test failed!\n");
    else printf("Test OK!\n");
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

int main()
{
    //test_memcpy_fast();
    test_memcpy_f9();
    return 0;
}
