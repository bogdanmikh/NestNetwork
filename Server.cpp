//
// Created by Bogdan on 19.08.2024.
//

#include "Server.hpp"
#include <iostream>

namespace Nest {
    void Server::onAttach() {
        if (enet_initialize() != 0) {
            printf("An error occurred while initializing ENet.\n");
            return;
        }

        ENetAddress address = {0};

        address.host = ENET_HOST_ANY;
        address.port = 7777;

        /* create a server */
        m_server = enet_host_create(&address, maxClients, 2, 0, 0);

        if (m_server == nullptr) {
            printf("An error occurred while trying to create an ENet server host.\n");
            return;
        }

        printf("Started a server...\n");
    }

    void Server::onUpdate() {
        ENetEvent event;
        while (enet_host_service(m_server, &event, 1) > 0) {
            if (event.type == ENET_EVENT_TYPE_CONNECT) {
                m_clients.emplace_back(event.peer);
                printf("A new client connected from %x:%u.\n", event.peer->address.host, event.peer->address.port);
                sendMessage("Idi v jopy", strlen("Idi v jopy"), event.peer);
            } else if (event.type == ENET_EVENT_TYPE_RECEIVE) {
                std::cout << "Received packet: " << (char *) (event.packet->data) << std::endl;
                // Отправка данных обратно клиенту

                // Уничтожение полученного пакета
                enet_packet_destroy(event.packet);
            } else if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
                printf("%s disconnected.\n", event.peer->data);
                for (int i = 0; i < m_clients.size(); ++i) {
                    if (event.peer == m_clients[i]) {
                        m_clients.erase(m_clients.begin() + i);
                        break;
                    }
                }
                event.peer->data = nullptr;
            } else if (event.type == ENET_EVENT_TYPE_DISCONNECT_TIMEOUT) {
                printf("%s disconnected due to timeout.\n", event.peer->data);
                /* Reset the peer's client information. */
                event.peer->data = nullptr;
            } else if (event.type == ENET_EVENT_TYPE_NONE) {
                /// ...
            }
        }
    }

    void Server::onDetach() {
        enet_host_destroy(m_server);
        enet_deinitialize();
    }

    void Server::sendMessage(const char *data, size_t s, ENetPeer *client) {
        ENetPacket *packet = enet_packet_create(data, s, ENET_PACKET_FLAG_RELIABLE);

        //the second parameter is the channel id
        enet_peer_send(client, 0, packet);
    }
}