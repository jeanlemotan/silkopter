#pragma once

namespace silk
{

class UAV_Config
{
public:
    virtual ~UAV_Config() = default;

    enum class Type : uint8_t
    {
        MULTIROTOR,
        ROVER,
        BOAT,
        PLANE
    };

    UAV_Config(Type type)
        : type(type)
    {}

    Type get_type() const { return type; }

    std::string name;
    float mass = 0; //Kg
    float moment_of_inertia; //computed
private:
    Type type;
};

}
