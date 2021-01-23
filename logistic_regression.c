#include "logistic_regression.h"
#include <stdlib.h>
#include <math.h>


Classifier* createClassifier(int size, double learning_rate){
  /*
  Creates a classifier of given size and learning rate.
  Should be deleted by deleteClassifier()
  */

  Classifier* logReg = (Classifier*)malloc(sizeof(Classifier));

  logReg->size = size;
  logReg->lr = learning_rate;

  logReg->w = (double*)malloc(sizeof(double)*size);

  for(int i = 0; i < size; i++){

    logReg->w[i] = 0;
  }

  return logReg;

}

void deleteClassifier(Classifier* logReg){
  /*
  Deletes a classifier struct.
  */
  free(logReg->w);
  free(logReg);
}

double sigmoid(double z){
  /*
  The sigmoid function.
  */
  double temp=-1.0 * z;
  return (1.0 / (double)(1.0 + exp(temp)));

}


double hypothesis(double* w, double* x, int size){
  /*
  f(x) = sigmoid(b + x1w1 + x2w2 + ... + xnwn)
  */
  double z = 0;

  for(int i = 0; i < size; i++){
    z += x[i] * w[i];
  }

  return sigmoid(z);

}

costFunctionDerivative(double* w, double** x, int size, int* y, int j, int batchSize, double learning_rate){
  double sumErrors = 0;
  for(int i=0; i<batchSize; i++){
    double *xi= x[i];
  }
}


void gradientDescent(double* w, double** x, int size, int* y, double learning_rate, int batchSize){
  /*
  In this function we update the weights. Since the updated weights
  are the same minus the gradient times the learning rate we just need
  the gradient, which for a single y is (h(x)-y)*x(i)
  */

  double error = hypothesis(w, x, size) - y;

  for(int i = 0; i < size; i++){
    double gradient = error * x[i];
    w[i] = w[i] - learning_rate*gradient;
  }

}


void logisticRegression(Classifier* logReg, double** x, int* y, int iterations, int batchSize){
  /*
  We simply run gradientDescent() a number of given iterations
  to upadte the weight.
  */

  for(int i = 0; i < iterations; i++){
    gradientDescent(logReg->w, x, logReg->size, y, logReg->lr);
  }

}
