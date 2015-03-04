#pragma once

#include <QtCore/QAbstractItemModel>
#include <memory>
#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/stringbuffer.h"

class JSON_Model : public QAbstractItemModel
{
public:
    JSON_Model(QWidget* parent);
    ~JSON_Model();

    void set_document(std::string const& name, rapidjson::Document* document);

    //////////////////////////////////////////////////////////////////////////

public: //signals
    virtual QModelIndex index(int row, int column, QModelIndex const& parent = QModelIndex()) const;
    virtual QModelIndex parent(QModelIndex const& index) const;

    virtual int rowCount(QModelIndex const& parent = QModelIndex()) const;
    virtual int columnCount(QModelIndex const& parent = QModelIndex()) const;

protected:
    QStringList mimeTypes() const;
    virtual bool dropMimeData(QMimeData const* data, Qt::DropAction action, int row, int column, QModelIndex const& parent);

    //read-only
    virtual QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(QModelIndex const& index) const;

    //editing
    virtual bool setData(QModelIndex const& index, QVariant const& value, int role = Qt::EditRole);
    virtual bool setHeaderData(int section, Qt::Orientation orientation, QVariant const& value, int role = Qt::EditRole);
    virtual bool insertColumns(int position, int columns, QModelIndex const& parent = QModelIndex());
    virtual bool removeColumns(int position, int columns, QModelIndex const& parent = QModelIndex());
    virtual bool insertRows(int position, int rows, QModelIndex const& parent = QModelIndex());
    virtual bool removeRows(int position, int rows, QModelIndex const& parent = QModelIndex());

private:
    struct Tree_Item
    {
        Tree_Item(JSON_Model* model);
        ~Tree_Item();
        void build(Tree_Item* parent, size_t row, std::string const& name, rapidjson::Value* value);
        void reset();
        size_t get_row() const;
        Tree_Item* find(rapidjson::Value* value);

        JSON_Model* m_model = nullptr;
        Tree_Item* m_parent = nullptr;
        std::vector<std::unique_ptr<Tree_Item>> m_children;
        QModelIndex m_model_index;
        rapidjson::Value* m_json = nullptr;
        std::string m_name;
    };

    mutable Tree_Item m_root;
    rapidjson::Document* m_document = nullptr;
};




