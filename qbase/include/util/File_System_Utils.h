#pragma once

#include "Path.h"

namespace q
{
namespace util
{
namespace fs
{

    auto get_file_time(Path const& path) -> time_t;
    auto set_file_time(Path const& path, time_t time) -> bool;
	auto get_file_size(Path const& path) -> size_t;
	auto delete_file(Path const& path) -> bool;

	auto get_current_folder() -> Path;
	auto set_current_folder(Path const& path) -> bool;
	auto get_absolute_folder(Path const& path) -> Path;

	auto create_folder(Path const& path) -> bool;
	auto delete_folder(Path const& path) -> bool;

	auto is_folder(Path const& path) -> bool;
	auto is_file(Path const& path) -> bool;
	auto exists(Path const& path) -> bool;

}
}
}
