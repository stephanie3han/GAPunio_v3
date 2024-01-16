/* PMSIS includes */
#include "pmsis.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void counter(int counter_gap){
    while (1) {
        
        static int counter = 0;
        printf("Counter: %d\n", counter);
        for (int i = 0; i < counter_gap; ++i) {  
        }
        counter++;
    }
}

void compte()
{
int i,j;
float a,b;

a = 0.786;
b = 0.897;


for (i=0;i<100000;i++)
          for (j=0;j<100000;j++)
          	{
          	b = a * b;
          	a = a * 0.89;
               }

printf("End %f %f\n",a,b);
}

/*
void func0(uint32_t core_id){
	printf("function on core %d\n", core_id);
}

void func1(uint32_t core_id){
	printf("function on core %d\n", core_id);
}

void func2(uint32_t core_id){
	printf("function on core %d\n", core_id);
}

void func3(uint32_t core_id){
	printf("function on core %d\n", core_id);
}

void func4(uint32_t core_id){
	printf("function on core %d\n", core_id);
}

void func5(uint32_t core_id){
	printf("function on core %d\n", core_id);
}

void func6(uint32_t core_id){
	printf("function on core %d\n", core_id);
}

void func7(uint32_t core_id){
	printf("function on core %d\n", core_id);
}
*/

/* Task executed by cluster cores. */
void cluster_loop(void *arg)
{
    uint32_t core_id = pi_core_id(), cluster_id = pi_cluster_id();
    printf("[cluster_id core_id] = [%d %d] \n", cluster_id, core_id); //[0 0] core_id
    compte();
    //counter(1000);
                      // choose different function on each core
    /*switch (core_id){
    case 0:
    	printf("core_id=0\n");
    	//counter(1);
    	func0(0);
    	break;
    case 1:
    	printf("core_id=1\n");
    	//counter(1000000);
    	func1(1);
    	break;
    case 2:
    	printf("core_id=2\n");
    	//counter(1000000);
    	func2(2);
    	break;
    case 3:
    	printf("core_id=3\n");
    	//counter(1000000);
    	func3(3);
    	break;
    case 4:
    	printf("core_id=4\n");
    	//counter(1000000);
    	func4(4);
    	break;
    case 5:
    	printf("core_id=5\n");
    	//counter(1000000);
    	func5(5);
    	break;
    case 6:
    	printf("core_id=6\n");
    	//counter(1000000);
    	func6(6);
    	break;
    case 7:
    	printf("core_id=7\n");
    	//counter(1000000);
    	func7(7);
    	break;
    	
    }
    */
}

/* Cluster main entry, executed by core 0. */
void cluster_delegate(void *arg)
{
    printf("Cluster master core entry\n");
    /* Task dispatch to cluster cores. */
    pi_cl_team_fork(1, cluster_loop, arg);  ////  cluster_delegate()  pi_cl_cluster_nb_cores()   return to cluster core number //define how many cluster core will be activated
    printf("Cluster master core exit\n");
}

void loop(void)
{
    printf("Entering main controller\n");

    uint32_t errors = 0;
    uint32_t core_id = pi_core_id(), cluster_id = pi_cluster_id();
    printf("FC cluster_id, core_id [0x%x %d] \n", cluster_id, core_id);  //FC default cluster ID is 0x20, and it's core ID by defaut is 0x00

    struct pi_device cluster_dev;
    struct pi_cluster_conf cl_conf;

    /* Init cluster configuration structure. */
    pi_cluster_conf_init(&cl_conf);

    cl_conf.id = 0;////0x20;                /* Set cluster ID. */

    /* Configure & open cluster. */
    pi_open_from_conf(&cluster_dev, &cl_conf);

    pi_perf_conf(1 << PI_PERF_CYCLES | 1 << PI_PERF_ACTIVE_CYCLES| 1 << PI_PERF_LD| 1 <<PI_PERF_ST );
    pi_perf_reset();
    pi_perf_start();


    if (pi_cluster_open(&cluster_dev))
    {
        printf("Cluster open failed !\n");
        pmsis_exit(-1);
    }

    pi_perf_stop();
    uint32_t cycles = pi_perf_read(PI_PERF_ACTIVE_CYCLES);//PI_PERF_ACTIVE_CYCLES :processor activated period number, no free clock period
    uint32_t tim_cycles = pi_perf_read(PI_PERF_CYCLES);//PI_PERF_CYCLES :counter whole clock number when excute the code
    uint32_t load_times = pi_perf_read(PI_PERF_LD);//loaded operation number
    uint32_t storage_times=pi_perf_read(PI_PERF_ST);//storage number
    printf("%d cycles for processor\n%d cycles for code execution\n", cycles, tim_cycles);  //
    printf("%d for loading operation\n%d for store operation\n", load_times, storage_times); 
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
    return pmsis_kickoff((void *) loop);
}

