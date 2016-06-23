#pragma once

namespace silk
{

class IUAV_Properties
{
public:
    ~IUAV_Properties() = default;

    virtual float get_mass() const = 0;
    virtual float get_moment_of_inertia() const = 0;
};


}
