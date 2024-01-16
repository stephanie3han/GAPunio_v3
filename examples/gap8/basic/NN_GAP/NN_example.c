#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "NN_example.h"


// define neural network paramaters
#define INPUT_SIZE 2 //5
#define OUTPUT_SIZE 1
#define LEARNING_RATE 0.01
#define EPOCHS 100000

// define ReLU activated function
double activation(double x) {
    return (x > 0) ? x : 0.0;
}

// define neural network weights and bias
double weights[INPUT_SIZE];
double bias;


// 自定义简单随机数生成函数
int custom_rand() {
    static unsigned int seed = 12345;  // 初始种子
    seed = seed * 1103515245 + 12345;  //////////////multipler current time
    return (seed / 65536) % 32768;
}


// initialize NN paramaters
void initialize() {
    for (int i = 0; i < INPUT_SIZE; i++) {
        weights[i] = ((double)custom_rand() / RAND_MAX) * 2 - 1; // initialize random weights 
        printf("weight_init = %f\n",weights);
    }
    bias = ((double)custom_rand() / RAND_MAX) * 2 - 1; // initialize random bias 
    printf("bias_init = %f\n",bias);
}

// neural network forward propagation
double predict(double inputs[]) {
    double output = 0;
    for (int i = 0; i < INPUT_SIZE; i++) {
        output += weights[i] * inputs[i];
    }
    output += bias;
    return activation(output);
}

// Train a neural network
void train(double inputs[], double target) {
    double prediction = predict(inputs);
    double error = target - prediction;
    for (int i = 0; i < INPUT_SIZE; i++) {
        weights[i] += LEARNING_RATE * error;
    }
    bias += LEARNING_RATE * error;
}
int main() {
    // Initialize the neural network
    initialize();

    // prepare training data
    //double training_data[][INPUT_SIZE] = {{0.1, 0.12, 0.03, -0.08, -0.1}, {1.1, 0.9, 0.98, 1.02, 1.0}, {1.9, 2.1,1.98, 2.05, 2.12}, {2.89, 3.2, 3.01, 2.99, 3.13}};
    double training_data[][INPUT_SIZE] = {{0.1, 0.12}, {1.1, 0.9}, {2.1,1.98}, {2.89, 3.2}};
    //double training_data[][INPUT_SIZE] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    double targets[] = {0, 1, 2, 3};
    
    double test_data[][INPUT_SIZE] ={{0.2, 0.01},{0.89,0.99},{1.95, 1.98},{1.11, 0.99}};
    double test_targets[] = {0,1,2,1};
    // Train a neural network
    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        for (int i = 0; i < sizeof(training_data) / sizeof(training_data[0]); i++) {
            train(training_data[i], targets[i]);
        }
    }

    // Test neural network
    for (int i = 0; i < sizeof(test_data) / sizeof(test_data[0]); i++) {
        double prediction = predict(test_data[i]);
        printf("Input: [%lf, %lf], Target: %lf, Prediction: %lf\n", 
               test_data[i][0], test_data[i][1], test_targets[i], prediction);
    }

    return 0;
}

