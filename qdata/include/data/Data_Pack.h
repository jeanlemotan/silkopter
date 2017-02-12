#pragma once

namespace q
{
namespace data
{
class Data_Pack;
DECLARE_CLASS_PTR(Data_Pack);

class Data_Pack : public util::Noncopyable
{
public:
    virtual ~Data_Pack() {}

    virtual auto get_count() const -> size_t = 0;
    virtual auto get_path(size_t idx) const -> Path const& = 0;
    virtual auto find_path_by_name(std::string const& name) const -> Path const& = 0;
    virtual auto find_all_paths_by_name(std::string const& name) const -> std::vector<Path> = 0;
    virtual auto find_idx_by_path(Path const& path) const -> int = 0;
    virtual auto open(Path const& path) const -> Source_uptr = 0;
};

}
}
