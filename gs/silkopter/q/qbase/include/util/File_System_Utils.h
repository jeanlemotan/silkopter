#pragma once

#include "Path.h"

namespace q
{
namespace util
{
namespace fs
{

	auto get_file_time(Path const& path) -> Clock::time_point;
	auto set_file_time(Path const& path, Clock::time_point time) -> bool;
	auto get_file_size(Path const& path) -> size_t;
	auto delete_file(Path const& path) -> bool;

	auto get_current_folder() -> Path;
	auto set_current_folder(Path const& path) -> bool;
	auto get_absolute_folder(Path const& path) -> Path;
	auto get_disk_id(Path const& path) -> String;
	auto are_on_same_disk(Path const& path1, Path const& path2) -> bool;

	auto create_folder(Path const& path) -> bool;
	auto delete_folder(Path const& path) -> bool;

	auto is_folder(Path const& path) -> bool;
	auto is_file(Path const& path) -> bool;
	auto exists(Path const& path) -> bool;

}
}
}