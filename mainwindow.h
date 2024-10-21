#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    typedef QSet<QString> UniquePaths;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onImageSelection();

signals:
    void imagePathsUpdated();

private:
    Ui::MainWindow *ui;

    QSharedPointer<QSet<QString>> mImagePaths;
    int mCurrentTab = 0;
};
#endif // MAINWINDOW_H
