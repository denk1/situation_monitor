#include <future>
#include <bit>
#include "PathFinder.h"
#include "ConfigMonitor.h"

using namespace std::chrono_literals;

template <class To, class From>
std::enable_if_t<
    sizeof(To) == sizeof(From) &&
        std::is_trivially_copyable_v<From> &&
        std::is_trivially_copyable_v<To>,
    To>
// constexpr support needs compiler magic
bit_cast(const From &src) noexcept
{
    static_assert(std::is_trivially_constructible_v<To>,
                  "This implementation additionally requires "
                  "destination type to be trivially constructible");

    To dst;
    std::memcpy(&dst, &src, sizeof(To));
    return dst;
}

PathFinder::PathFinder(float width, float height): x_roi(width), y_roi(height), mIsStop(), mStartSending()
{
}

PathFinder::~PathFinder()
{
    mIsStop = true;
    mThread->join();
}

void PathFinder::start()
{
    mThread = std::make_unique<std::thread>(&PathFinder::Run, this);
}

void PathFinder::Run()
{
    networkClient_.connect(ConfigMonitor::getSingleton().mConfigItems["BMP_SERVER_HOST"], ConfigMonitor::getSingleton().mConfigItems["BMP_SERVER_PORT"], 10);
    std::this_thread::sleep_for(2ms);

    while (!mIsStop)
    {
        if (!mStartSending)
            continue;
        convertBuff();
        std::string path_str = networkClient_.getDataFromServer(str_buff_convert_);
        std::cout << path_str;
        convertRecievedPath(path_str);
    }
}

void PathFinder::setDataAsync(const std::string &str_buff)
{
    auto a = std::async(std::launch::async, &PathFinder::setData, this, str_buff);
}

void PathFinder::convertBuff()
{
    std::lock_guard lock(mutex_);
    std::stringstream ss;
    ss << (byte)0x44 << (byte)0x47;
    str_buff_convert_ = ss.str();
    const float width = x_roi, height = y_roi, step = 2;
    const float x_goal = 0, y_goal = 0, z_goal = 99.0f;
    const int center = width / step / 2 + (height / step - 1) * (width / step);
    // test
    const size_t n = 3;
    size_t test_s = sizeof(n);
    const byte *w = reinterpret_cast<const byte *>(&width);
    const byte *h = reinterpret_cast<const byte *>(&height);
    const byte *s = reinterpret_cast<const byte *>(&step);
    const byte *c_b = reinterpret_cast<const byte *>(&center);
    const byte *x_g = reinterpret_cast<const byte *>(&x_goal);
    const byte *y_g = reinterpret_cast<const byte *>(&y_goal);
    const byte *z_g = reinterpret_cast<const byte *>(&z_goal);
    const byte *n_b = reinterpret_cast<const byte *>(&n);

    str_buff_convert_.append(w, w + 4);
    str_buff_convert_.append(h, h + 4);
    str_buff_convert_.append(s, s + 4);
    str_buff_convert_.append(c_b, c_b + 4);
    str_buff_convert_.append(x_g, x_g + 4);
    str_buff_convert_.append(y_g, y_g + 4);
    str_buff_convert_.append(z_g, z_g + 4);
    // test
    // str_buff_convert_.append(n_b, n_b + 8);
    str_buff_convert_.append(str_buff_.begin() + 2, str_buff_.end());
}

void PathFinder::convertRecievedPath(const std::string &str_path)
{
    if (!str_path.empty())
    {
        std::string str_pure_path;
        u_char n_in_char[4];
        if ((u_char)str_path[0] == 0x44 && (u_char)str_path[1] == 0x48)
        {
            std::cout << "the path is OK" << std::endl;
            std::copy(str_path.begin() + 2, str_path.begin() + 6, n_in_char);
            str_pure_path.append(str_path.begin() + 6, str_path.end());
            const int n_in_int = bit_cast<int>(n_in_char);
            setTargetPath(n_in_int, str_pure_path);
            std::cout << "the quantity of the path items is " << n_in_int << std::endl;
        }
        else
        {
            std::cout << "the path message has not been reconized" << std::endl;
        }
    }
}

Path PathFinder::getTargetPath()
{
    std::lock_guard lock(mutex1_);
    return mTargetPath;
}

void PathFinder::setTargetPath(size_t n, const std::string &str_path)
{
    std::lock_guard lock(mutex1_);
    char float_in_char[4];
    mTargetPath.n = n;
    mTargetPath.v_path_value.clear();
    for (size_t i = 0; i < n; ++i)
    {
        std::copy(str_path.begin() + i * 4, str_path.begin() + i * 4 + 4, float_in_char);
        mTargetPath.v_path_value.push_back(bit_cast<float>(float_in_char));
    }
}