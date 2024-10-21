#ifndef BENCHMARKTABVIEW_H
#define BENCHMARKTABVIEW_H

#include <QTabWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QMessageBox>

namespace {

QString humanReadableSize(qint64 size) {
    const QStringList units = {"B", "KB", "MB", "GB", "TB"};
    int unitIndex = 0;

    double sizeInUnits = static_cast<double>(size);
    while (sizeInUnits >= 1024 && unitIndex < units.size() - 1) {
        sizeInUnits /= 1024;
        unitIndex++;
    }

    return QString::number(sizeInUnits, 'f', 2) + " " + units[unitIndex];
}

} // namespace

class BenchmarkTable {
public:

    BenchmarkTable(QWidget* parent) {

        mTable = new QTableWidget(parent);
        mTable->setColumnCount(3);

        mTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        mTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        mTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        mTable->setHorizontalHeaderLabels({"Path", "Resolution", "Size"});
        mTable->setSortingEnabled(true);
    }

    ~BenchmarkTable() {
        mTable->deleteLater();
    }

    QTableWidget* getPointer() const {
        return mTable;
    }

private:
    QTableWidget* mTable;
};

class BenchmarkTabView : public QWidget
{
    Q_OBJECT

    BenchmarkTabView() = delete;

public:

    BenchmarkTabView(BenchmarkTabView&& other) = delete;

    ~BenchmarkTabView() {
        mParent->removeTab(getTabIndex());
    }

    BenchmarkTabView(QTabWidget* parent)
        : QWidget(parent), mTable(this), mParent{parent} {

        mLayout = new QVBoxLayout(this);
        mLayout->addWidget(mTable.getPointer());
        this->setLayout(mLayout);

        parent->addTab(this, "Benchmark");
    }

    int getTabIndex() {
        auto* tabWidget = mParent;

        int result = tabWidget->indexOf(this);
        assert(result != -1);

        return result;
    }

    void updatePaths(QSet<QString>& paths) {
        auto& table = *mTable.getPointer();
        table.setEnabled(false);
        table.clearContents();
        table.setRowCount(0);

        for(auto& path : paths) {
            const auto image = QImage(path);
            const auto info = QFileInfo(path);
            if(image.isNull()) {
                qDebug() << "Failed to load image: '" << path << "'";
                continue;
            }

            const size_t filesize = info.size();
            const size_t width = image.width();
            const size_t height = image.height();
            const int row = table.rowCount();
            table.insertRow(row);
            table.setItem(row, 0, new QTableWidgetItem(path));
            table.setItem(row, 1, new QTableWidgetItem(QString("%1x%2").arg(width).arg(height)));
            table.setItem(row, 2, new QTableWidgetItem(humanReadableSize(filesize)));
        }

        table.setEnabled(true);
    }

    void displayProcessingTime(size_t nsec) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Benchmark complete");
        msgBox.setText(QString("Benchmark completed in %1 seconds").arg(nsec * 10e-9));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);

        msgBox.exec();
    }

    void displayWarning(QString msg) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setText(msg);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Ok);

        msgBox.exec();
    }

private:
    BenchmarkTable mTable;
    QTabWidget* mParent;
    QVBoxLayout* mLayout;
};

#endif // BENCHMARKTABVIEW_H
