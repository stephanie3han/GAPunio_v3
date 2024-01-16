/* PMSIS includes */
#include "pmsis.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "NN_example.h"


/* Task executed by cluster cores. */
void cluster_loop(void *arg)
{
    uint32_t core_id = pi_core_id(), cluster_id = pi_cluster_id();
    printf("[cluster_id, core_id]= [%d %d] \n", cluster_id, core_id); //[0 0] core_id
    switch (core_id){
    case 0:
    	printf("core_id=0\t");
    	initialize();
    	 for (int epoch = 0; epoch < EPOCHS; epoch++) {
        for (int i = 0; i < sizeof(training_data) / sizeof(training_data[0]); i++) {
            train(training_data[i], targets[i]);
        	}
    	}
    	for (int i = 0; i < sizeof(test_data) / sizeof(test_data[0]); i++) {
        double prediction = predict(test_data[i]);
        printf("Input: [%lf, %lf], Target: %lf, Prediction: %lf\n", 
               test_data[i][0], test_data[i][1], test_targets[i], prediction);
    }
    	delay_time(1000);
    	break;
    case 1:
    	delay_time(1000);
    	printf("core_id=1\t");
    	
    	// Train a neural network
   
    	delay_time(1000);
    	break;
    case 2:
    	delay_time(1000);
    	printf("core_id=2\t training network");
    	
    	break;
    case 3:
    	delay_time(1000);
    	printf("core_id=3\t test NN ");
    	//func3(3);
    	
    	break;
    case 4:
    	delay_time(1000);
    	printf("core_id=4\n");
    	func4(4);
    	// Test neural network
    	
   
    	break;
    case 5:
    	printf("core_id=5\n");
    	func5(5);
    	break;
    case 6:
    	printf("core_id=6\n");
    	func6(6);
    	break;
    case 7:
    	printf("core_id=7\n");
    	func7(7);
    	break;
    	
    }
    
}

/* Cluster main entry, executed by core 0. */
void cluster_delegate(void *arg)
{
    printf("Cluster master core entry\n");
    /* Task dispatch to cluster cores. */
    pi_cl_team_fork(1, cluster_loop, arg);  ////  cluster_delegate()  pi_cl_cluster_nb_cores() return to cluster core number  // determinate how many cluster core will be used
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
    printf("%d cycles for processor\t%d cycles for code execution\t", cycles, tim_cycles);  //
    printf("%d for loading operation\t%d for store operation\n", load_times, storage_times); 
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
    time1=rt_time_get_us();
    delay_time(1000);
    printf("\n\n\t *** PMSIS HelloWorld ***\n\n");
    time=rt_time_get_us();  //time is a seed for random function
    SEED_time=time-time1;
    printf("randon = %ld\n",custom_rand(SEED_time));
    printf("\n");
    return pmsis_kickoff((void *) loop);
}

