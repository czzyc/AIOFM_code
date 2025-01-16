#ifndef OGIGLO_H
#define OGIGLO_H
#include "ogiworker.h"
#include <QMutex>

#include <QMutexLocker>
extern OGIWorker *ogiworker;
extern double *doubleData ;
extern QMutex m_muteogi;
class OGIglo
{
public:
    OGIglo();
};

#endif // OGIGLO_H
