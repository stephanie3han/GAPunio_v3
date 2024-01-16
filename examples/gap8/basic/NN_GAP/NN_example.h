#include <stdlib.h>
#include <math.h>

// define neural network architecture
#define INPUT_SIZE 2 //5
#define OUTPUT_SIZE 1
#define LEARNING_RATE 0.01
#define EPOCHS 10000
#define alpha 0.05


// define weights and bias
double weights1[2][INPUT_SIZE];   ///////////////////////add a neuron
double weights2[2];
double bias[2];

long time=0;
long SEED_time=0;
long time1=0;
int random_value=0;

double training_data[][INPUT_SIZE] = {{0.01, 0.12}, {1.01, 0.9}, {2.1,1.98}, {2.89, 3.02}};
double targets[] = {0, 1, 2, 3};
    
double test_data[][INPUT_SIZE] ={{0.02, 0.01},{0.89,0.99},{1.85, 1.78},{1.11, 0.99}};
double test_targets[] = {0,1,2,1};


void delay_time(int counter_delay){

	for(int i=0,j=0;i<counter_delay;i++){
		j=j+1;
	}
}

//define ReLU activation function
double activation(double x) {
    return (x > 0) ? x : 0.0;
}


// define a random number fuction
int custom_rand(long time) {
    static unsigned int seed = 12345;  // seed for random
    seed = seed * time+ 12345;
    return (seed / 65536) % 32768;
}


void get_time(uint32_t core_id){
	printf("function on core %d to get time:\t", core_id);
	long time_=rt_time_get_us();
	SEED_time=(time_-time)*alpha;
	printf("SEED_time=%d\n",SEED_time);
}


// initialize NN paramaters
void initialize() {
   for(int j=0;j<2;j++){
    	 for (int i = 0; i < INPUT_SIZE; i++) {
        	weights1[j][i] = ((double)custom_rand(time) / time) * 2 - 1; // initialize random weights 
        	printf("weight1[%d][%d]_init = %d\n",j,i,weights1);
    	}
    }
    for (int i = 0; i < INPUT_SIZE; i++) {
        	weights2[i] = ((double)custom_rand(time) / time) * 2 - 1; // initialize random weights 
        	printf("weight2[%d]_init = %d\n",i,weights2);
    	}
    for(int k=0;k<2;k++){
    bias[k] = ((double)custom_rand(time) / time) * 2 - 1; // initialize random bias 
    printf("bias[%d]_init = %d\n",k,bias);
    }
}


// neural network forward propagation
double predict(double inputs[]) {
    double output1[] = {0,0};
    double output=0;
    for (int j=0;j<2;j++){  ///j th neuron in the first layer
    	for (int i = 0; i < INPUT_SIZE; i++) {  // 1 st and 2 nd weight in the j th neuron
        	output1[i] += weights1[j][i] * inputs[i];
        	output1[i] +=bias[0];
        	output1[i]=activation(output1[i]);
		}
	}
    for (int i=0; i<2;i++){
    	output += weights2[i] * output1[i];
        output +=bias[1];
        output=activation(output);
    }
    return output;
}

// Train a neural network
void train(double inputs[], double target) {
    double prediction = predict(inputs);
    double error = target - prediction;
    for (int j=0;j<2;j++){ 
    	for (int i = 0; i < INPUT_SIZE; i++) {
        	weights1[j][i] += LEARNING_RATE * error;
    	}
    }
    for(int k=0;k<2;k++){
    	bias[k] += LEARNING_RATE * error;
    }
}



void func1(uint32_t core_id){
	printf("function on core %d\t", core_id);
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


