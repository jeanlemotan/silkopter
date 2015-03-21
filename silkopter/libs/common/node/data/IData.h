#pragma once

#include "rapidjson/document.h"

namespace silk
{
namespace node
{
namespace data
{

//for static data that rarely changes
class IData : q::util::Noncopyable
{
public:
    virtual ~IData() {}
};
DECLARE_CLASS_PTR(IData);

}
}
}
