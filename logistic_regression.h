#ifndef logistic_regression_h
#define logistic_regression_h

typedef struct ClassifierType{

  int size;
  int y;
  double* w;
  double lr;

}Classifier;

Classifier* createClassifier(int, double);
double sigmoid(double);
double hypothesis(double*, double*, int);
void gradientDescent(double*, double*, int, int, double);
void logisticRegression(Classifier*, double*, int, int);
#endif
