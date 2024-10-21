#ifndef BENCHMARKTABMODEL_H
#define BENCHMARKTABMODEL_H

#include <QObject>
#include <dlfcn.h>

using namespace std::string_literals;

class BenchmarkTabModel {
public:
    BenchmarkTabModel() {

    }

    BenchmarkTabModel(QString dllPath) {

        mDllHandle = dlopen(dllPath.toUtf8(), RTLD_NOW | RTLD_LOCAL);
        if(mDllHandle == nullptr) {
            throw std::runtime_error("Failed to dlopen '"s + dllPath.toStdString() + "': " + dlerror());
        }

        const char* symbolName = "test";
        void* foundSymbol = dlsym(mDllHandle, symbolName);
        if(foundSymbol == nullptr) {
            throw std::runtime_error("Failed to find symbol '"s
                                     + symbolName
                                     +"' in library '"
                                     + dllPath.toStdString()
                                     + "': "
                                     + dlerror());
        }

        mFunction = reinterpret_cast<void(*)()>(foundSymbol);

        assert(isInitialized());
    }

    BenchmarkTabModel(BenchmarkTabModel&& other)
        : mDllHandle{other.mDllHandle}, mFunction{other.mFunction} {

        other.mDllHandle = nullptr;
        other.mFunction = nullptr;
    }

    ~BenchmarkTabModel() {
        if(isInitialized() == false) return;

        dlclose(mDllHandle);
    }

    bool isInitialized() const {
        return mDllHandle && mFunction;
    }

    size_t startBenchmark(QSet<QString>& paths, size_t repeatCount = 10'000) {
        assert(isInitialized());

        return 1337;
    }

private:

    void* mDllHandle = nullptr;
    std::function<void()> mFunction = nullptr;
};

#endif // BENCHMARKTABMODEL_H
