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
        void setData(const std::string& str_buff) {
            std::lock_guard lock(mutex_);
            str_buff_ = str_buff;
            mStartSending = true;
        }
        void setDataAsync(const std::string& str_buff);
        void convertBuff();
        void convertRecievedPath(const std::string& str_path);
        void start();

    private:
        void Run();
        bool mIsStop;
        bool mStartSending;
        std::string str_buff_;
        std::string str_buff_convert_;
        std::unique_ptr<std::thread> mThread;
        std::mutex mutex_;
        NetworkClientBMD networkClient_;
        
};
#endif //PATH_FINDER_H