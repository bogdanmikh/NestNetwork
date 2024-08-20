//
// Created by Bogdan on 19.08.2024.
//

#pragma once

#include "BaseNetwork.hpp"

#include "enet.h"

#include <vector>

namespace Nest {

    class Server {
    public:
        void onAttach();
        void onUpdate();
        void onDetach();
    private:
        PushData m_data;
        static const int maxClients = 4;
        void sendData(const void* data, size_t s, ENetPeer *client);
        ENetHost* m_server;
        std::vector<ENetPeer*> m_clients;
    };

}