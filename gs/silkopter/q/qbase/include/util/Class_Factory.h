#pragma once

namespace q
{
namespace util
{
	class Class_Factory
	{
	public:
		 template <class T>
		 static void register_class()
		 {
			 auto* ctor = &create_class_instance_function<T>;
			 register_class(ctor, rtti::get_class_name<T>());
			 register_class(ctor, rtti::get_class_id<T>());
		 }
		 template <class T>
		 static void register_class(std::string const& className, int classId)
		 {
			 auto* ctor = &create_class_instance_function<T>;
			 register_class(ctor, className);
			 register_class(ctor, classId);
		 }

		 template <class Base>
		 static auto create_instance(std::string const& className) -> std::unique_ptr<Base>
		 {
			 auto it = m_name_registry.find(className);
			 return std::unique_ptr<Base>(it == m_name_registry.end() ? nullptr : reinterpret_cast<Base*>((*it->second)()));
		 }
		 template <class Base>
		 static auto create_instance(int classId) -> std::unique_ptr<Base>
		 {
			 auto it = m_id_registry.find(classId);
			 return std::unique_ptr<Base>(it == m_id_registry.end() ? nullptr : reinterpret_cast<Base*>((*it->second)()));
		 }

	private:
		typedef void* (*create_instance_function)();

		static std::map<std::string, create_instance_function> m_name_registry;
		static std::map<int, create_instance_function> m_id_registry;
		
		template <class T>
		static auto create_class_instance_function() -> void*
		{
			return reinterpret_cast<void*>(new T());
		}
		static void register_class(create_instance_function creator, std::string const& class_name)
		{
			if (class_name.empty())
			{
				return;
			}
			auto it = m_name_registry.find(class_name);
			if (it != m_name_registry.end())
			{
				QLOG_ERR("ClassFctory", "Error: class '{}' already defined.", class_name);
				return;
			}
			m_name_registry[class_name] = creator;
		}
		static void register_class(create_instance_function creator, int classId)
		{
			if (classId == 0)
			{
				return;
			}
			auto it = m_id_registry.find(classId);
			if (it != m_id_registry.end())
			{
				QLOG_ERR("ClassFactory", "Error: class '{}' already defined.", classId);
				return;
			}
			m_id_registry[classId] = creator;
		}
	};

}
}
