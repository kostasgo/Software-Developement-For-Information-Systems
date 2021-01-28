#ifndef sparse_vectors_h
#define sparse_vectors_h

typedef struct SparseVType {
  double* data;
  int* index;
  int size;
}SparseV;

SparseV* createSparseV(double*, int, int);
void deleteSparseV(SparseV*);
SparseV* concatSparseV(SparseV*, SparseV*, int);
double getPosValue(SparseV* x, int pos);
void printSparseV(SparseV*);
#endif
