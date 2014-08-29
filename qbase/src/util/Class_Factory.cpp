#include "QBaseStdAfx.h"
#include "QBase.h"

std::map<std::string, q::util::Class_Factory::create_instance_function> q::util::Class_Factory::m_name_registry;
std::map<int, q::util::Class_Factory::create_instance_function> q::util::Class_Factory::m_id_registry;