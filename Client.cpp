//
// Created by Bogdan on 19.08.2024.
//

#include "Client.hpp"
#include <iostream>

namespace Nest {

    void Client::onAttach() {
        if (enet_initialize() != 0) {
            fprintf(stderr, "An error occurred while initializing ENet.\n");
            return;
        }

        m_client = nullptr;
        m_client = enet_host_create(nullptr /* create a client host */,
                                  1 /* only allow 1 outgoing connection */,
                                  2 /* allow up 2 channels to be used, 0 and 1 */,
                                  0 /* assume any amount of incoming bandwidth */,
                                  0 /* assume any amount of outgoing bandwidth */);
        if (m_client == nullptr) {
            fprintf(stderr,
                    "An error occurred while trying to create an ENet client host.\n");
            exit(EXIT_FAILURE);
        }

        ENetAddress address = { 0 };
        m_event.channelID = 0;
        /* Connect to some.server.net:1234. */

        enet_address_set_host(&address, "127.0.0.1");
        address.port = 7777;
        /* Initiate the connection, allocating the two channels 0 and 1. */
        m_peer = enet_host_connect(m_client, &address, 2, 0);
        if (m_peer == nullptr) {
            fprintf(stderr,
                    "No available peers for initiating an ENet connection.\n");
            exit(EXIT_FAILURE);
        }
    }

    void Client::onUpdate() {
        while (enet_host_service(m_client, &m_event, 10) > 0) {
            if (m_event.type == ENET_EVENT_TYPE_CONNECT) {
                m_disconnected = false;
                std::cout << "Connected to server!" << std::endl;
                // Отправляем пакет "Hello from client"
                ENetPacket* packet = enet_packet_create("Hello from client", strlen("Hello from client") + 1, ENET_PACKET_FLAG_RELIABLE);
                std::cout << "Packet data: " << (char*)packet->data;
                enet_peer_send(m_peer, 0, packet);
                enet_host_flush(m_client);
            } else if (m_event.type == ENET_EVENT_TYPE_RECEIVE) {
                std::cout << "Received packet: " << m_event.packet->data << std::endl;
                enet_packet_destroy(m_event.packet);
            } else if (m_event.type == ENET_EVENT_TYPE_DISCONNECT) {
                puts("Disconnection succeeded.");
                m_disconnected = true;
            }
        }
    }

    void Client::onDetach() {
        if (!m_disconnected) {
            enet_peer_reset(m_peer);
        }
        enet_host_destroy(m_client);
        enet_deinitialize();
    }
}