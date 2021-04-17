#ifndef BUFFERED_CHANNEL_H_
#define BUFFERED_CHANNEL_H_

#include <mutex>
#include <condition_variable>
#include <queue>

template<class T>
class BufferedChannel {
public:
    explicit BufferedChannel(int size):size(size),isOpen(true) {
    
    }

    void Send(T value) {
        if (!isOpen) {
            throw std::runtime_error("Closed!");
        }
        lock.lock();
        conditional.wait(lock, [this] {
                                   return !(buffer.size() == size);
                               }
        );
        buffer.push(value);
        lock.unlock();
        conditional.notify_all();
    }

    std::pair<T, bool> Recv() {
        lock.lock()
        conditional.wait(lock, [this] {
            return !buffer.empty();
            }
        );
        T element= buffer.front();
        buffer.pop();
        lock.unlock();
        conditional.notify_all();
        return std::make_pair(element, isOpen);
    }

    void Close() {
        lock.lock();
        isOpen = false;
        lock.unlock();
        conditional.notify_all();
    }

private:
    std::mutex lock;
    int size;
    bool isOpen;
    std::queue<T> buffer;
    std::mutex g_mutex;
    std::vector<std::exception_ptr> g_exceptions;
    std::condition_variable conditional;
};

#endif // BUFFERED_CHANNEL_H_