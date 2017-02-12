#pragma once



namespace q
{
namespace anim
{

class Animation_Node : public util::Noncopyable
{
public:
    virtual ~Animation_Node() {}

    //Returns a number representing the structure version. If anything in the structure changes, increment the version
    //If data changes no need to increment - data is never cached outside
    virtual uint32_t		get_version() const = 0;

    //Returns the pose this node can handle
    virtual Pose const&		get_pose() const = 0;

    //Updates the info from pose for the time passed. Pose channels that are not found should be ignored
    virtual bool			update_pose(Pose& pose, Duration time) const = 0;

    //Returns the duration of the data.
    //Updating the pose past this duration should be clamped to the end time.
    virtual Duration		get_duration() const = 0;
};


}
}
