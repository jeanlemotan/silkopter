#pragma once

#include <QTreeView>
#include <QSortFilterProxyModel>

class QSettings;

class Properties_Model;
class Properties_Delegate;
class Value_Editor_Factory;

class Properties_Browser : public QTreeView
{
	Q_OBJECT
protected:
	class SignalKey;
public:
    Properties_Browser(QWidget* parent = nullptr);
    ~Properties_Browser();

    void init(std::shared_ptr<Value_Editor_Factory> editor_factory);
    void shutdown();

    std::shared_ptr<Value_Editor_Factory> get_editor_factory();

    void reset();

    void set_value(std::shared_ptr<ts::IStruct_Value> value);

    boost::signals2::signal<void()> sig_value_changed;
	
protected:
	void defaultExpand();
    //void keyReleaseEvent(QKeyEvent * event) override;

private:
    void save_collapse_status();
    void load_collapse_status();

    void setModel(QAbstractItemModel* model); //virtual from QT - DON't USE!!! Call set_properties_model
    void set_tree_view_model(QAbstractItemModel* model);

    void on_range_changed(int rmin, int rmax);
    void on_link_clicked(const ts::IValue& value);

    void root_item_changed(Properties_Model& model);

    struct Connections
    {
        boost::signals2::scoped_connection value_changed_connection;
    } m_connections;

    std::shared_ptr<Properties_Model> m_model;
    std::shared_ptr<Properties_Delegate> m_delegate;
};

