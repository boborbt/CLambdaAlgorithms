#ifndef __DOUBLE_CONTAINER_H_HK28XKP30__
#define __DOUBLE_CONTAINER_H_HK28XKP30__

typedef struct _DoubleContainer* DoubleContainer;

DoubleContainer DoubleContainer_new(double value);
void DoubleContainer_free(const DoubleContainer dc);
double DoubleContainer_get(const DoubleContainer dc);

#endif
