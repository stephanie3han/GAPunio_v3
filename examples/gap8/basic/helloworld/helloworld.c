/* PMSIS includes */
#include "pmsis.h"

/* Task executed by cluster cores. */
void cluster_helloworld(void *arg)
{
    uint32_t core_id = pi_core_id(), cluster_id = pi_cluster_id();
    printf("[%d %d] Hello World!\n", cluster_id, core_id); //[0 0] core_id
}

/* Cluster main entry, executed by core 0. */
void cluster_delegate(void *arg)
{
    printf("Cluster master core entry\n");
    /* Task dispatch to cluster cores. */
    pi_cl_team_fork(pi_cl_cluster_nb_cores(), cluster_helloworld, arg);  ////  cluster_delegate()
    printf("Cluster master core exit\n");
}

void helloworld(void)
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
    return pmsis_kickoff((void *) helloworld);
}

