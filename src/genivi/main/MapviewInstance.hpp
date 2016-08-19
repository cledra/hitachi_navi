#ifndef MAPVIEWINSTANCE_HPP
#define MAPVIEWINSTANCE_HPP

class MapviewInstance
{
    public:
        MapviewInstance(uint32_t handle_, std::string client_, int32_t ViewType_, int16_t w_, int16_t h_):
            handle(handle_),
            ViewType(ViewType_),
            w(w_), h(h_) {
                client = std::string(client_);
            }

        uint32_t handle;
        int32_t ViewType;
        int16_t w;
        int16_t h;
        std::string client;
};

#endif
