/**
 * PSEUDOCODE
 * my_printf(string, ...)
 *      size = 0
 *      list ap;
 *      char *result = malloc(1);
 *      realloc
 *      va_start(ap, string)
 *      // Process
 *      for i in range(string)
 *              if string[i] == '%'
 *                      if (i == strlen(string) - 1) printf -> free -> break
 *                      switch(string[i + 1]) (!break)
 *                      case: convert() -> temp -> my_write(char *temp, strlen(temp)) -> size += strlen(string)
 *              else
 *                      my_write(char *result, strlen(result)) -> size = strlen(result)
 *     
 *      // container (!realloc) / step by step (!write)
 * 
 *      my_wrtite(char *str, int n)
 *              write(1, str, n)
 *      va_end(ap)
 *      return size
 *
 * implementation of itoa() functions for d,o,u,x,p;
 *
 * implementation of other 
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

typedef unsigned int ui;
typedef unsigned long ul;

int my_printf(const char *, ...);
char *convert(va_list *, char);
char *int_to_str(int);
char *octal_to_str(ui);
char *uint_to_str(ui);
char *hexa_to_str(ui);
char *hexa_to_str_p(ul n);
char *ptr_to_str(void *);
size_t my_strlen(const char *ch);
char *my_strcpy(char *param_1, char *param_2);
char *my_strcat(char *s1, const char *s2);

int main()
{
    char *text = "Pointer";
    int a = -123;

    // tests:
    printf("Total symbols: %d\n", my_printf("%d - %d - %d!\n", 2048, 0, -1337)); 
    // printf("Total symbols: %d\n", my_printf("%p!", a)); 
    // printf("Total symbols: %d\n", my_printf("%p: Good job %s", NULL, "Peer. "));
    // printf("Total symbols: %d\n", my_printf("NULL STRING %s!", (char *)NULL)); 
    // printf("Total symbols: %d\n", printf("NULL STRING %s!", (char *)NULL)); 
    return 0;
}

int my_printf(const char *format, ...)
{
    char *temp;
    int size = 0;

    va_list ap;
    va_start(ap, format);

    int len = my_strlen(format);
    // int * ptr = va_arg(ap, int *);

    if (format[len - 1] == '%')
    {
        char error[] = "\nError occured: \"\%%\"";
        write(1, error, my_strlen(error));
        return 0;
    }
    for (int i = 0; i < len; i++)
    {
        if (format[i] == '%')
        {
            i += 1;
            temp = convert(&ap, format[i]);
            write(1, temp, my_strlen(temp));
            size += my_strlen(temp);
            free(temp);
        }
        else
        {
            size += 1;
            write(1, &format[i], 1);
        }
    }
    va_end(ap);
    return size;
}

char *convert(va_list *ap, char type)
{
    char *str;
    char *result;

    switch (type)
    {
    case 'd':
        result = int_to_str(va_arg(*ap, int));
        break;
    case 'o':
        result = octal_to_str(va_arg(*ap, ui));
        break;
    case 'u':
        result = uint_to_str(va_arg(*ap, ui));
        break;
    case 'x':
        result = hexa_to_str(va_arg(*ap, ui));
        break;
    case 'p':
        result = ptr_to_str(va_arg(*ap, void *));
        break;
    case 's':
        str = va_arg(*ap, char *);
        if (str == NULL)
        {
            result = malloc(8);
            my_strcpy(result, "(null)");
        }
        else
        {
            result = malloc(my_strlen(str) + 1);
            result = strcpy(result, str);
            result[my_strlen(str)] = 0;
        }
        break;
    case 'c':
        result = malloc(2);
        result[0] = va_arg(*ap, int);
        result[1] = 0;
        break;
    default: // if there is other than above cases it will return error message:
        printf("\nError with passed type!\n");
        return NULL;
        break;
    }
    return result;
}

// function converting into unsigned decimal integer, case 'd':
char *int_to_str(int n)
{
    char *string;
    int size = 0;
    int is_negative = 0;
    if (n < 0)
    {
        n = -n;
        is_negative = 1;
    }
    int copy_n = n;
    if (n == 0)
    {
        n = 0;
        size += 1;
    }
    while (copy_n > 0)
    {
        size += 1;
        copy_n /= 10;
    }
    int len = size + is_negative;
    string = malloc(len + 1);
    copy_n = n;
    if (is_negative)
        string[0] = '-';
    for (int i = len - 1; i >= is_negative; i--)
    {
        string[i] = copy_n % 10 + '0';
        copy_n /= 10;
    }
    string[len] = 0;
    return string;
}

// case 'o': function converting input -> octal integer;
char *octal_to_str(ui n)
{
    char *string;
    int size = 0;
    int is_negative = 0;
    ui copy_n = n;
    while (copy_n > 0)
    {
        size += 1;
        copy_n /= 8;
    }
    int len = size + is_negative;
    string = malloc(len + 1);
    copy_n = n;
    if (is_negative)
        string[0] = '-';
    for (int i = len - 1; i >= is_negative; i--)
    {
        string[i] = copy_n % 8 + '0';
        copy_n /= 8;
    }
    string[len] = 0;
    return string;
}

//case 'u':  convert input -> unsigned decimal; 
char *uint_to_str(ui n)
{
    char *string;
    int size = 0;
    ui copy_n = n;
    while (copy_n > 0)
    {
        size += 1;
        copy_n /= 10;
    }
    int len = size;
    string = malloc(len + 1);
    copy_n = n;
    for (int i = len - 1; i >= 0; i--)
    {
        string[i] = copy_n % 10 + '0';
        copy_n /= 10;
    }
    string[len] = 0;
    return string;
}

//case 'x': function to convert input -> to hexidecimal; 
char *hexa_to_str(ui n)
{
    char *string;
    int size = 0;
    int is_negative = 0;
    ui copy_n = n;
    while (copy_n > 0)
    {
        size += 1;
        copy_n /= 16;
    }
    int len = size + is_negative;
    string = malloc(len + 1);
    copy_n = n;
    if (is_negative)
        string[0] = '-';
    for (int i = len - 1; i >= is_negative; i--)
    {
        if (copy_n % 16 < 10)
        {
            string[i] = copy_n % 16 + '0';
        }
        else
            string[i] = copy_n % 16 - 10 + 'a';
        copy_n /= 16;
    }
    string[len] = 0;
    return string;
}

//case 'p': function to return a pointer; 
char *ptr_to_str(void *ptr)
{
    ul address = (ul)ptr;
    char *hexa = hexa_to_str_p(address);
    char *string = malloc(strlen(hexa) + 3);
    string[0] = 0;

    if (my_strlen(hexa) != 0)
    {
        string = my_strcat(string, "0x");
        string = my_strcat(string, hexa);
    }
    else
    {
        string = realloc(string, 6);
        string = my_strcpy(string, "(nil)");
    }
    free(hexa);
    return string;
}

char *hexa_to_str_p(ul n)
{
    char *string;
    int size = 0;
    int is_negative = 0;
    ul copy_n = n;
    while (copy_n > 0)
    {
        size += 1;
        copy_n /= 16;
    }
    int len = size + is_negative;
    string = malloc(len + 1);
    copy_n = n;
    if (is_negative)
        string[0] = '-';
    for (int i = len - 1; i >= is_negative; i--)
    {
        if (copy_n % 16 < 10)
        {
            string[i] = copy_n % 16 + '0';
        }
        else
            string[i] = copy_n % 16 - 10 + 'a';
        copy_n /= 16;
    }
    string[len] = 0;
    return string;
}

// strlen() implementation:
size_t my_strlen(const char *ch)
{
    int count = 0;
    if (ch[0] == '\0')
    {
        return 0;
    }
    while (*ch)
    {
        count++;
        ch++;
    }
    return count;
}

// strcpy() implementation:
char *my_strcpy(char *param_1, char *param_2)
{
    int i = 0;
    while (param_2[i] != '\0')
    {
        param_1[i] = param_2[i];
        i++;
    }
    param_1[i] = '\0';
    return param_1;
}

// strcat() implementation:
char *my_strcat(char *s1, const char *s2)
{
    if ((s1 == NULL) && (s2 == NULL))
        return NULL;

    char *start = s1;

    while (*start != '\0')
    {
        start++;
    }

    while (*s2 != '\0')
    {
        *start++ = *s2++;
    }

    *start = '\0';
    return s1;
}
