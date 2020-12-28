#ifndef logistic_regression_h
#define logistic_regression_h

typedef struct LogisticRegressionType{

  int y;
  double* w;
  double* x;
  double lr;
  
}LogisticRegression;

LogisticRegression* createLogisticRegression(int);

#endif
