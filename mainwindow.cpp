#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "benchmarktabcontroller.h"

#include <QFileDialog>

namespace {

} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionOpen_image, &QAction::triggered, this, &MainWindow::onImageSelection);

    mImagePaths = QSharedPointer<UniquePaths>(new UniquePaths());

    auto* benchmarkTab = new BenchmarkTabController(ui->tabWidget, mImagePaths);
    connect(ui->actionRun, &QAction::triggered, benchmarkTab, &AImageTab::startProcessing);
    connect(ui->tabWidget, &QTabWidget::currentChanged, benchmarkTab, &AImageTab::onTabChange);
    connect(this, &MainWindow::imagePathsUpdated, benchmarkTab, &AImageTab::onPathsUpdate);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onImageSelection() {
    auto paths = QFileDialog::getOpenFileNames(this,
                                               "Select Images",
                                               "",
                                               "Images (*.png, *.jpg);;");

    auto pathsSet = QSet<QString>(paths.begin(), paths.end());

    mImagePaths->unite(pathsSet);
    emit imagePathsUpdated();
}
