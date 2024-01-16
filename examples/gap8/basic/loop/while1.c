#include <stdio.h>
/* PMSIS includes */
#include "pmsis.h"

/* Task executed by cluster cores. */
void cluster_while1(void *arg)
{
    uint32_t core_id = pi_core_id(), cluster_id = pi_cluster_id();
    printf("[%d %d] Hello World!\n", cluster_id, core_id); //[0 0] core_id
}

/* Cluster main entry, executed by core 0. */
void cluster_delegate(void *arg)
{
    printf("Cluster master core entry\n");
    /* Task dispatch to cluster cores. */
    pi_cl_team_fork(1, cluster_while1, arg);  ////  cluster_delegate() // pi_cl_cluster_nb_cores()
    printf("Cluster master core exit\n");
}

void while1(void)
{
    printf("Entering main controller\n");

    uint32_t errors = 0;
    uint32_t core_id = pi_core_id(), cluster_id = pi_cluster_id();
    printf("[0x%x %d] Hello World!\n", cluster_id, core_id);  //FC default cluster ID is 0x20, and it's core ID by defaut is 0x00

    struct pi_device cluster_dev;
    struct pi_cluster_conf cl_conf;

    /* Init cluster configuration structure. */
    pi_cluster_conf_init(&cl_conf);
    //printf("before\n");
    cl_conf.id = 0;////0x20;                /* Set cluster ID. */
    //printf("after\n");
    /* Configure & open cluster. */
    pi_open_from_conf(&cluster_dev, &cl_conf);
   //printf("after\n");
    pi_perf_conf(1 << PI_PERF_CYCLES | 1 << PI_PERF_ACTIVE_CYCLES| 1 << PI_PERF_LD| 1 <<PI_PERF_ST );
    pi_perf_reset();
    pi_perf_start();
    //printf("after\n");

    if (pi_cluster_open(&cluster_dev))
    {
        printf("Cluster open failed !\n");
        pmsis_exit(-1);
    }

    pi_perf_stop();
    uint32_t cycles = pi_perf_read(PI_PERF_ACTIVE_CYCLES);//PI_PERF_ACTIVE_CYCLES 表示启用活动周期数计数器，该计数器用于测量*处理器*的活动周期数，即处理器在执行指令时的周期数，排除了空闲周期。
    uint32_t tim_cycles = pi_perf_read(PI_PERF_CYCLES);//PI_PERF_CYCLES表示启用周期数计数器，该计数器用于测量代码执行所花费的时钟周期数。
    uint32_t load_times = pi_perf_read(PI_PERF_LD);//测量加载操作的数量
    uint32_t storage_times=pi_perf_read(PI_PERF_ST);//测量存储操作的数量
    printf(" %d cycles for processor\n %d cycles for code execution\n", cycles, tim_cycles);  //
    printf(" %d for loading operation\n %d for store operation\n", load_times, storage_times); 
    /* Prepare cluster task and send it to cluster. */
    struct pi_cluster_task cl_task;

    pi_cluster_send_task_to_cl(&cluster_dev, pi_cluster_task(&cl_task, cluster_delegate, NULL));

    pi_cluster_close(&cluster_dev);

    printf("Test success !\n");

    pmsis_exit(errors);
}



/* Program Entry. */
int main(void)
{
    printf("\n\n\t *** PMSIS HelloWorld ***\n\n");
    return pmsis_kickoff((void *) while1);
}



/*
void cluster_while1() {
    // 无限循环
    while (1) {
        // 在这里执行你的任务，可以是任何你想要测试功耗的代码

        // 例如，递增计数器
        static int counter = 0;
        printf("Counter: %d\n", counter);

        // 在这里插入外部操作，用于测量功耗
        // 例如，可以在这里接入测量功耗的导线或执行其他与功耗相关的操作

        // 模拟任务执行时间
        for (int i = 0; i < 1000000; ++i) {
            // 延迟，模拟任务执行时间
        }

        // 增加计数器
        counter++;
    }

    return 0;
}
*/
