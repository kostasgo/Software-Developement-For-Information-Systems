#include "logistic_regression.h"
#include "stdlib.h"

LogisticRegression* createLogisticRegression(int size){
  LogisticRegression* logReg= (LogisticRegression*)malloc(sizeof(LogisticRegression));
  logReg->e=2.71828;
  logReg->lr=0.001;
  logReg->y=0;
  for(int i=0;i<size;i++){
    logReg->x[i]=0;
    logReg->w[i]=0;
  }
}
