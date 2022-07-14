#ifndef PATH_FINDER_H
#define PATH_FINDER_H
#include <memory>
#include <thread>
#include <mutex>
#include <chrono>
#include "NetworkClientBMD.h"


using byte = unsigned char;

struct Path {
    size_t n = 0;
    std::vector<float> v_path_value;
};

class PathFinder {
    public:
        PathFinder(float width, float height);
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
        Path getTargetPath();
        

    private:
        void Run();
        void setTargetPath(size_t n, const std::string& str_path);
        bool mIsStop;
        bool mStartSending;
        std::string str_buff_;
        std::string str_buff_convert_;
        std::unique_ptr<std::thread> mThread;
        std::mutex mutex_;
        std::mutex mutex1_;
        NetworkClientBMD networkClient_;
        Path mTargetPath;
        const float x_roi, y_roi;
        
};
#endif //PATH_FINDER_H