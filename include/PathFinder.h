#ifndef PATH_FINDER_H
#define PATH_FINDER_H
#include <memory>
#include <thread>
#include <mutex>
#include <chrono>
#include "NetworkClientBMD.h"


using byte = unsigned char;

class PathFinder {
    public:
        PathFinder();
        ~PathFinder();
        void setData(std::string& str_buff) {
            std::lock_guard lock(mutex_);
            str_buff_ = str_buff;
        }

    private:
        void Run();
        void convertBuff();
        bool mIsStop;
        std::string str_buff_;
        std::string str_buff_convert_;
        std::unique_ptr<std::thread> mThread;
        std::mutex mutex_;
        NetworkClientBMD networkClient_;
        
};
#endif //PATH_FINDER_H