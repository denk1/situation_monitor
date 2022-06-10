#include "PathFinder.h"

using namespace std::chrono_literals;

PathFinder::PathFinder() {
    mThread = std::make_unique<std::thread>(&PathFinder::Run, this);
}

PathFinder::~PathFinder() {
     
}

void PathFinder::Run() {
    std::this_thread::sleep_for(2ms);
    networkClient_.connect("10.91.1.33", "15556", 10);
    while (!mIsStop)
    {
        std::cout << networkClient_.getDataFromServer(str_buff_convert_);
        
    }
}

void PathFinder::convertBuff() {
    std::lock_guard lock(mutex_);
    std::stringstream ss;
    ss << (byte)0x44 << (byte)0x47;
    str_buff_convert_ = ss.str();
    float width = 100, height = 100, step = 1;
    float x_goal = 0, y_goal = 0, z_goal;
    const byte* w = reinterpret_cast<byte*>(&width);
    const byte* h = reinterpret_cast<byte*>(&height);
    const byte* s = reinterpret_cast<byte*>(&step);
    const byte* x_g = reinterpret_cast<byte*>(&x_goal);
    const byte* y_g = reinterpret_cast<byte*>(&y_goal);
    const byte* z_g = reinterpret_cast<byte*>(&z_goal);
    str_buff_convert_.append(w, w + 4);
    str_buff_convert_.append(h, h + 4);
    str_buff_convert_.append(s, s + 4);
    str_buff_convert_.append(x_g, x_g + 4);
    str_buff_convert_.append(y_g, y_g + 4);
    str_buff_convert_.append(z_g, z_g + 4);
    str_buff_convert_.append(str_buff_.begin() + 2, str_buff_.end());


}