#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "NN_example.h"

/*
// 定义神经网络结构
#define INPUT_SIZE 2
#define OUTPUT_SIZE 1
#define LEARNING_RATE 0.01
#define EPOCHS 10000

// 定义ReLU激活函数
double activation(double x) {
    return (x > 0) ? x : 0.0;
}

// 定义神经网络权重和偏置
double weights[INPUT_SIZE];
double bias;
*/
// 初始化神经网络参数
void initialize() {
    for (int i = 0; i < INPUT_SIZE; i++) {
        weights[i] = ((double)rand() / RAND_MAX) * 2 - 1; // 随机初始化权重
    }
    bias = ((double)rand() / RAND_MAX) * 2 - 1; // 随机初始化偏置
}

// 神经网络前向传播
double predict(double inputs[]) {
    double output = 0;
    for (int i = 0; i < INPUT_SIZE; i++) {
        output += weights[i] * inputs[i];
    }
    output += bias;
    return activation(output);
}

// 训练神经网络
void train(double inputs[], double target) {
    double prediction = predict(inputs);
    double error = target - prediction;
    for (int i = 0; i < INPUT_SIZE; i++) {
        weights[i] += LEARNING_RATE * error * inputs[i];
    }
    bias += LEARNING_RATE * error;
}

int main() {
    // 初始化神经网络
    initialize();

    // 训练数据
    double training_data[][INPUT_SIZE] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    double targets[] = {0, 1, 1, 1};

    // 训练神经网络
    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        for (int i = 0; i < sizeof(training_data) / sizeof(training_data[0]); i++) {
            train(training_data[i], targets[i]);
        }
    }

    // 测试神经网络
    for (int i = 0; i < sizeof(training_data) / sizeof(training_data[0]); i++) {
        double prediction = predict(training_data[i]);
        printf("Input: [%lf, %lf], Target: %lf, Prediction: %lf\n", 
               training_data[i][0], training_data[i][1], targets[i], prediction);
    }

    return 0;
}

