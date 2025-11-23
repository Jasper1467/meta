#pragma once

#include <algorithm>
#include <functional>
#include <vector>

namespace meta
{
    template <typename... Args> class Signal
    {
    public:
        using SlotType = std::function<void(Args...)>;

        class Connection
        {
        public:
            Connection(Signal* sig, size_t id) : signal(sig), id(id)
            {
            }
            void disconnect()
            {
                if (signal)
                    signal->disconnect(id);
            }

        private:
            Signal* signal = nullptr;
            size_t id = 0;
        };

        // Connect a callable to this signal
        Connection connect(SlotType&& slot)
        {
            size_t id = nextId++;
            slots.emplace_back(id, std::move(slot));
            return Connection(this, id);
        }

        // Emit the signal to all connected slots
        void emit(Args... args)
        {
            for (auto& [id, slot] : slots)
            {
                if (slot)
                    slot(args...);
            }
        }

        void operator()(Args... args)
        {
            emit(args...);
        }

    private:
        void disconnect(size_t id)
        {
            auto it = std::remove_if(slots.begin(), slots.end(), [id](const auto& pair) { return pair.first == id; });
            slots.erase(it, slots.end());
        }

        std::vector<std::pair<size_t, SlotType>> slots;
        size_t nextId = 1;
    };

    template <typename SignalType> class ScopedConnection
    {
    public:
        ScopedConnection(SignalType::Connection conn) : connection(std::move(conn))
        {
        }
        ~ScopedConnection()
        {
            connection.disconnect();
        }

    private:
        typename SignalType::Connection connection;
    };

} // namespace meta
