#include "logistic_regression.h"
#include "stdlib.h"
#include "math.h"

Classifier* createClassifier(int size, double learning_rate){

  Classifier* logReg= (Classifier*)malloc(sizeof(Classifier));

  logReg->size=size;
  logReg->lr=learning_rate;
  logReg->y=0;

  logReg->w = (double*)malloc(sizeof(double)*size);
  for(int i=0;i<size;i++){

    logReg->w[i]=0;
  }

  return logReg;
}

double sigmoid(double z){
  return (double)(1.0/(double)(1.0+exp(-1.0*z)));
}

double hypothesis(double* w, double* x, int size){
  double z=0;
  for(int i=0;i<size;i++){
    z += x[i]*w[i];
  }

  return sigmoid(z);

}


void gradientDescent(double* w, double* x, int size, int y, double learning_rate){
  double error = hypothesis(w, x, size) - y;
  for(int i=0; i<size; i++){
    double gradient=error*x[i];
    w[i]=w[i]-learning_rate*gradient;
  }
}

void logisticRegression(Classifier* logReg, double* x, int y, int iterations){
  for(int i=0; i<iterations; i++){
    gradientDescent(logReg->w, x, logReg->size, y, logReg->lr);
  }
}
