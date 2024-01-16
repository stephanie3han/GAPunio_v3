#include <stdlib.h>
#include <math.h>

// 定义神经网络结构
#define INPUT_SIZE 2
#define OUTPUT_SIZE 1
#define LEARNING_RATE 0.01
#define EPOCHS 10000

// 定义激活函数（这里使用简单的阶跃函数）
double activation(double x) {
    return (x > 0) ? 1.0 : 0.0;
}

// 定义神经网络权重和偏置
double weights[INPUT_SIZE];
double bias;
