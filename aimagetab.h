#ifndef AIMAGETAB_H
#define AIMAGETAB_H

#include <QObject>
#include <QWidget>
#include <QSharedPointer>
#include <QTabWidget>

class AImageTab : public QObject {
    Q_OBJECT
public:
    typedef QSet<QString> UniquePaths;

    AImageTab(QSharedPointer<UniquePaths>& pathsPtr)
        : mPathsPtr{pathsPtr} {

    }

    virtual ~AImageTab() = default;

public slots:
    virtual void startProcessing() = 0;
    virtual void onTabChange(int index) = 0;
    virtual void onPathsUpdate() = 0;

signals:
    void processingDone();

protected:
    const QSharedPointer<UniquePaths> mPathsPtr;
};

#endif // AIMAGETAB_H
