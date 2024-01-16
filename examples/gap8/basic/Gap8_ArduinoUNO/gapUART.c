/* PMSIS includes */
#include "pmsis.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "Gap.h"
#define BUFFER_SIZE 10

char *myInteger = "0123456789";
char data[BUFFER_SIZE];
struct pi_device uart;
void UART_Read(void)
{
    char receivedString[BUFFER_SIZE];
    int stringIndex = 0;
    pi_uart_write(&uart, myInteger, strlen(myInteger));
    pi_uart_read(&uart, data, BUFFER_SIZE); 

}

void write_data_to_file(const char *filename, const char *data, size_t size)
{
    FILE *file = fopen(filename, "a"); // 打开文件进行追加
    if (file != NULL)
    {
        fwrite(data, 1, size, file); // 写数据到文件
        fclose(file); // 关闭文件
    }
    else
    {
        printf("Unable to open file for writing\n");
    }
}
   
void test_uart(void)
{
    printf("Entering main controller\n");
    uint32_t errors = 0;

    struct pi_uart_conf conf;
    pi_uart_conf_init(&conf);
    conf.enable_tx = 1;
    conf.enable_rx = 1;
    conf.word_size = PI_UART_WORD_SIZE_8_BITS; // 设置数据位为8位
    conf.stop_bit_count = PI_UART_STOP_BITS_ONE; // 设置停止位为1位
    conf.parity_mode = PI_UART_PARITY_DISABLE; // 禁用奇偶校验
    conf.baudrate_bps = 115200;
    pi_open_from_conf(&uart, &conf);
    if (pi_uart_open(&uart))
    {
        printf("Uart open failed !\n");
        pmsis_exit(-1);
    }


    /* Write though uart. */
while(1){
    UART_Read();
    printf("Received: "); // Print the string
    scanf("%s",data);
    printf("%s\n", data);
    write_data_to_file("uart_data.txt", data, strlen(data)); // 写数据到文件
}
    pi_uart_close(&uart);

    pmsis_exit(errors);
}

/* Program Entry. */
int main(void)
{
    printf("\n\n\t *** PMSIS Uart ***\n\n");
    return pmsis_kickoff((void *) test_uart);
}
