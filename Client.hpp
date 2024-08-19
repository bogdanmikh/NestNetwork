//
// Created by Bogdan on 19.08.2024.
//

#include <enet.h>

namespace Nest {

    class Client {
    public:
        void onAttach();
        void onUpdate();
        void onDetach();
    private:
        ENetHost *m_client = nullptr;
        ENetPeer* m_peer = nullptr;
        ENetEvent m_event;
        bool m_disconnected = false;
    };

}
