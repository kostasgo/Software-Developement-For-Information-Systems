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

double costFunctionDerivative(double* theta, double** x, int size, int* y, int j, int m, double learning_rate){
  double sumErrors = 0;
  for(int i=0; i<m; i++){
    double *xi= x[i];
    double xij =xi[j];
    double hi= hypothesis(theta,x[i], size);
    double error = (hi - y[i])*xij;
    sumErrors += error;
  }
  double ret= (double)(sumErrors * (double)(learning_rate/(double)m));
  return ret;
}


double* gradientDescent(double* theta, double** x, int size, int* y, double learning_rate, int m){
  /*
  In this function we update the weights. Since the updated weights
  are the same minus the gradient times the learning rate we just need
  the gradient, which for a single y is (h(x)-y)*x(i)
  */


  for(int j=0; j<size; j++){
    double CFDerivative = costFunctionDerivative(theta, x, size, y, j, m, learning_rate);
    theta[j]=theta[j]-CFDerivative;
  }
  return theta;

}

double* singleGradient(double* theta, double* x, int size, int y, double learning_rate){
  double h = hypothesis(theta, x, size);
  double error= h-y;
  for(int j=0;j<size;j++){
    error=error*x[j];
    theta[j]=theta[j]-learning_rate*error;
  }
  return theta;
}


double* logisticRegression(Classifier* logReg, double** x, int* y, int iterations, int m){
  /*
  We simply run gradientDescent() a number of given iterations
  to upadte the weight.
  */

  // weights to be returned

  double *theta=(double*)malloc(sizeof(double)*(logReg->size));
  //start them with what we got
  for(int i=0; i<logReg->size; i++){
    theta[i]=logReg->w[i];
  }
   //renew for number of iterations
  for(int i = 0; i < iterations; i++){
    if(m!=1){
      theta=gradientDescent(theta, x, logReg->size, y, logReg->lr, m);
    }
    else{
      theta=singleGradient(theta, *x, logReg->size, *y, logReg->lr);
    }

  }

  return theta;

}
