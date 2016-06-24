#pragma once

#include <memory>
#include <QAbstractItemModel>

#include "def_lang/IStruct_Value.h"
#include "def_lang/IOptional_Value.h"
#include "def_lang/IVariant_Value.h"
#include "def_lang/IVector_Value.h"

class Properties_Model : public QAbstractItemModel
{
public:

    enum ItemDataRole 
	{
        TypeRole = Qt::UserRole + 1,
		ReferenceClickedRole,
		EditorNameRole
    };

    Properties_Model(QWidget* parent = nullptr);
    ~Properties_Model();

    void set_value(std::shared_ptr<ts::IStruct_Value> value);

	//////////////////////////////////////////////////////////////////////////

    QModelIndex get_root_index() const;

    QModelIndex get_index_from_value(ts::IValue const& value) const;
    std::shared_ptr<ts::IValue> get_value_from_index(QModelIndex index) const;
    bool is_index_read_only(QModelIndex index) const;
	virtual QModelIndex parent(QModelIndex const& index) const;

public: //signals
	virtual QModelIndex index(int row, int column, QModelIndex const& parent = QModelIndex()) const;

	virtual int rowCount(QModelIndex const& parent = QModelIndex()) const;
	virtual int columnCount(QModelIndex const& parent = QModelIndex()) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;
	virtual QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const;

protected:
	//read-only 
	virtual Qt::ItemFlags flags(QModelIndex const& index) const;

	//editing
	virtual bool setData(QModelIndex const& index, QVariant const& value, int role = Qt::EditRole);
	virtual bool setHeaderData(int section, Qt::Orientation orientation, QVariant const& value, int role = Qt::EditRole);
	virtual bool insertColumns(int position, int columns, QModelIndex const& parent = QModelIndex());
	virtual bool removeColumns(int position, int columns, QModelIndex const& parent = QModelIndex());
	virtual bool insertRows(int position, int rows, QModelIndex const& parent = QModelIndex());
	virtual bool removeRows(int position, int rows, QModelIndex const& parent = QModelIndex());

	//callbacks for the db
    void on_optional_was_set(std::shared_ptr<ts::IOptional_Value> parent);
    void on_optional_will_be_unset(std::shared_ptr<ts::IOptional_Value> parent);

    void on_variant_type_index_will_be_changed(std::shared_ptr<ts::IVariant_Value> parent);
    void on_variant_type_index_was_changed(std::shared_ptr<ts::IVariant_Value> parent);

    void on_elements_added(std::shared_ptr<ts::IVector_Value> parent, size_t idx, size_t count);
    void on_elements_will_be_removed(std::shared_ptr<ts::IVector_Value> parent, size_t idx, size_t count);
    void on_value_changed(std::shared_ptr<const ts::IValue> value);

    struct Tree_Item : public std::enable_shared_from_this<Tree_Item>
	{
        Tree_Item(Properties_Model* model, const std::string& name);
        ~Tree_Item();
        void build_root(ts::IStruct_Value& struct_value);
        void build(std::shared_ptr<ts::IValue> value);
        void build_children(std::shared_ptr<ts::IValue> value);
        void reset();
        size_t get_row() const;
        void refresh_model_index(size_t row);
        std::shared_ptr<Tree_Item> find_by_primary(ts::IValue const& value) const;
        std::shared_ptr<Tree_Item> find_by_primary_or_secondary(ts::IValue const& value) const;
        size_t deep_get_child_count() const;
        size_t get_child_count() const;
        void add_child(std::shared_ptr<Tree_Item> child);
        void insert_child(size_t off, std::shared_ptr<Tree_Item> child);

        Properties_Model* m_model = nullptr;
        std::weak_ptr<Tree_Item> m_parent;
        std::vector<std::shared_ptr<Tree_Item>> m_children;
        QModelIndex m_model_index;

        std::vector<q::util::Scoped_Connection> m_connections;

        std::string m_name;
        std::shared_ptr<ts::IValue> m_value;

		//some items represent 2 values - a primary one (m_value) and this secondary one.
		//for example an optional<struct> will have both the optional and the struct in the same TreeItem (optional - m_value, struct - m_secondaryValue)
		//  and the children of the struct as children of this TreeItem
        std::shared_ptr<ts::IValue> m_secondary_value;
		bool m_isReadOnly = false;
	};

private:
    void _added(const ts::IValue& parent, const std::vector<std::shared_ptr<ts::IValue>>& elements, size_t start, size_t count);
    void _will_be_removed(const ts::IValue& parent, const std::vector<std::shared_ptr<ts::IValue>>& elements, size_t start, size_t count);

    std::shared_ptr<ts::IStruct_Value> m_rootValue;

    mutable std::recursive_mutex m_tree_mutex;
    std::shared_ptr<Tree_Item> m_root;
    //QModelIndex m_pressed;
};



