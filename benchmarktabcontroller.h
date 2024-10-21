#ifndef BENCHMARKTABCONTROLLER_H
#define BENCHMARKTABCONTROLLER_H

#include <QObject>
#include <QMutex>
#include "aimagetab.h"
#include "benchmarktabmodel.h"
#include "benchmarktabview.h"
#include <time.h>


class BenchmarkTabController : public AImageTab
{
    Q_OBJECT

public:
    BenchmarkTabController(QTabWidget* parent, QSharedPointer<UniquePaths>& pathsPtr)
        : AImageTab(pathsPtr), mView(parent) {

    }

public slots:
    virtual void startProcessing() override {
        QMutexLocker mTransactionLock(&mTransactionMutex);

        if(mModel.isInitialized() == false) {
            mView.displayWarning("Select DLL before running!");
            return;
        }

        size_t benchmarkTime = mModel.startBenchmark(*mPathsPtr, mBenchmarkRepeatCount);

        mView.displayProcessingTime(benchmarkTime);
    }

    virtual void onTabChange(int index) override {
        bool indexMatches = (mView.getTabIndex() == index);

        QMutexLocker mTransactionLock(&mTransactionMutex);

        // TODO: Can isActive and indexMatches be both true?
        if(isActive && !indexMatches) {
            isActive = false;
            deactivate();
        } else if(!isActive && indexMatches) {
            isActive = true;
            activate();
        }
    }

    virtual void onPathsUpdate() override {
        QMutexLocker mTransactionLock(&mTransactionMutex);

        mView.updatePaths(*mPathsPtr);
    }

private:
    void activate();
    void deactivate();

private:
    BenchmarkTabModel mModel;
    BenchmarkTabView mView;
    QMutex mTransactionMutex;
    bool isActive = false;
    const size_t mBenchmarkRepeatCount = 10'000;
};

#endif // BENCHMARKTABCONTROLLER_H
