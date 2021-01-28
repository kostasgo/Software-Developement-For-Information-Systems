#include "logistic_regression.h"
#include "sparse_vectors.h"
#include <stdlib.h>
#include <stdio.h>
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
  //printf("z: %lf\n",z);
  double temp=-1.0 * z;
  double ret= (1.0 / (double)(1.0 + exp(temp)));
  //printf("sigmoid(z): %lf\n\n",ret);
  return ret;
}


double hypothesis(double* w, SparseV* x){
  /*
  f(x) = sigmoid(b + x1w1 + x2w2 + ... + xnwn)
  */
  double z = 0;

  for(int i = 0; i < x->size; i++){
  //  printf("%d: x: %lf w: %lf at index: %d\n",i,x->data[i], w[x->index[i]], x->index[i]);
    z += x->data[i] * w[x->index[i]];
  }
//  printf("h done!!\n");

  return sigmoid(z);

}

double costFunctionDerivative(double* theta, SparseV** x, int* y, int j, int m, double learning_rate){
  double sumErrors = 0;
  //printf("m is: %d\n",m);
  for(int i=0; i<m; i++){
    double xij= getPosValue(x[i], j);
    //printf("x[i][j] is %lf\n",xij);
  //  double xij =xi[j];
    double hi= hypothesis(theta,x[i]);
  //  printf("%d: h OK: %lf\n",i,hi);
    double error = (hi - y[i])*xij;
    sumErrors += error;
    //printf("sumerrors: %lf\n",sumErrors);
  }
  double constant = learning_rate/m;

  double ret= (double)(sumErrors * constant);
  //printf("summerrors %lf constant %lf ret %lf\n",sumErrors, constant, ret);
  return ret;
}


double* gradientDescent(double* theta, SparseV** x,int size, int* y, double learning_rate, int m){
  /*
  In this function we update the weights. Since the updated weights
  are the same minus the gradient times the learning rate we just need
  the gradient, which for a single y is (h(x)-y)*x(i)
  */


  for(int j=0; j<size; j++){
  //  printf("%d\n",j);
    double CFDerivative = costFunctionDerivative(theta, x, y, j, m, learning_rate);
    theta[j]=theta[j]-CFDerivative;
  //  printf("new theta[%d]: %lf\n\n",j,theta[j]);
  }
  return theta;

}

double* singleGradient(double* theta, SparseV* x, int y){
  double h = hypothesis(theta, x);

  double learning_rate=0.01;
  for(int j=0;j<x->size;j++){
    double error= h-y;
    error=error*x->data[j];
    //printf("%d: error: %lf\n",j,error);
    //printf("theta at %d before: %lf",x->index[j],theta[x->index[j]]);
    theta[x->index[j]]=theta[x->index[j]]-learning_rate*error;
    //printf("  after: %lf\n",theta[x->index[j]]);
  }
  return theta;
}


double* logisticRegression(Classifier* logReg, SparseV** x, int* y, int iterations, int m){
  /*
  We simply run gradientDescent() a number of given iterations
  to upadte the weight.
  */

  // weights to be returned

  double *theta=(double*)malloc(sizeof(double)*(logReg->size));
  //start them with what we got
  for(int i=0; i<logReg->size; i++){
    theta[i]=logReg->w[i];
    //printf("%d, %lf\n",i,theta[i]);
  }
  //printf("\n\n");
   //renew for number of iterations
  for(int i = 0; i < iterations; i++){
  //  printf("Iteration: %d\n\n",i);
    if(m!=1){
      theta=gradientDescent(theta, x, logReg->size, y, logReg->lr, m);
    }
    else{
      theta=singleGradient(theta, *x, *y);
    }

  }

  return theta;

}
