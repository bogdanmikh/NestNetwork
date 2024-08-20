//
// Created by Bogdan on 19.08.2024.
//

#include "BaseNetwork.hpp"
#include <enet.h>

namespace Nest {

    class Client {
    public:
        void onAttach();
        void onUpdate();
        void onDetach();
        bool serverValid();
    private:
        void sendData(const void* data, size_t size);

        ENetHost *m_client = nullptr;
        ENetPeer* m_server = nullptr;
        ENetEvent m_event;

        PushData m_data;
        bool m_disconnected = false;
    };

}
