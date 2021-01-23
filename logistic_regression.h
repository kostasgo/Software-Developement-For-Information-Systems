#ifndef logistic_regression_h
#define logistic_regression_h

typedef struct ClassifierType{
  /*
  The classifier struct that handles the logistic regression.
  It consists of a set of ws (weight/theta) and a learning rate. Y is
  the label of the .csv line and x is the concatenated tf-idf tables of
  both specs, with a 1 added in the end. That last cell represents b or bias
  in the equation and is handled as part of the weights.
  */
  int size;
  double* w;
  double lr;

}Classifier;

Classifier* createClassifier(int, double);
void deleteClassifier(Classifier*);
double sigmoid(double);
double hypothesis(double*, double*, int);
double* gradientDescent(double*, double**, int, int*, double, int);
double* logisticRegression(Classifier*, double**, int*, int, int);
#endif
