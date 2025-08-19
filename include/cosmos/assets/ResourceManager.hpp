#pragma once
// ==
// Standard Library
// ==
#include <unordered_map>
#include <memory>
#include <string>
#include <typeindex>
#include <functional>
#include <mutex>

// ==
// Third Party
// ==


// ==
// Cosmos
// ==

namespace cosmos::assets {

class ResourceManager {
public:
    template <typename T>
    std::shared_ptr<T> get_or_load(const std::string& key,
                                   std::function<std::shared_ptr<T>()> loader)
    {
        const std::type_index tid = std::type_index(typeid(T));
        std::scoped_lock lk(mutex_);

        auto& map = caches_[tid];
        if(auto it = map.find(key); it != map.end()) {
            if(auto sp = std::static_pointer_cast<T>(it->second.lock())) {
                return sp; // cache hit
            }
        }

        // cache miss -> load
        auto sp = loader();
        map[key] = sp;
        return sp;
    }

    // Force replacement (hot-reload)
    template <typename T>
    std::shared_ptr<T> reload(const std::string& key,
                              std::function<std::shared_ptr<T>()> loader)
    {
        const std::type_index tid = std::type_index(typeid(T));
        std::scoped_lock lk(mutex_);
        auto sp = loader();
        caches_[tid][key] = sp;
        return sp;
    }

    template <typename T>
    void drop(const std::string& key) {
        const std::type_index tid = std::type_index(typeid(T));
        std::scoped_lock lk(mutex_);
        auto it_tid = caches_.find(tid);
        if (it_tid != caches_.end()) it_tid->second.erase(key);
    }

    void clear() {
        std::scoped_lock lk(mutex_);
        caches_.clear();
    }


private:
    std::mutex mutex_;

    // type -> (key -> weak_ptr<void>)
    std::unordered_map<std::type_index, std::unordered_map<std::string, std::weak_ptr<void>>> caches_;
};
}
