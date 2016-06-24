#include "value_editors/IValue_Editor.h"
#include "value_editors/Value_Editor_Factory.h"
#include "value_editors/Empty_Value_Editor.h"
//#include "value_editors/Vector_Element_Wrapper_Editor.h"

#include "Properties_Delegate.h"
#include "Properties_Model.h"
#include "Properties_Browser.h"

#include <QDoubleSpinBox>
#include <QToolTip>
#include <QtEvents>
#include <QApplication>
#include <QHBoxLayout>
#include <QPushButton>

//////////////////////////////////////////////////////////////////////////

Properties_Delegate::Properties_Delegate(QObject* parent, std::shared_ptr<Value_Editor_Factory> editor_factory)
    : QStyledItemDelegate(parent)
    , m_editor_factory(editor_factory)
{
}

//////////////////////////////////////////////////////////////////////////

Properties_Delegate::~Properties_Delegate()
{
}

//////////////////////////////////////////////////////////////////////////

QWidget* Properties_Delegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Properties_Model* model = (Properties_Model*)index.model();
    if (!model)
    {
        return nullptr;
    }

    std::shared_ptr<ts::IValue> value = model->get_value_from_index(index);
    QASSERT(value);

    m_editor_data.reset();

    std::shared_ptr<ts::IValue> backup_value;

    ts::Result<std::shared_ptr<ts::IValue>> backup_result = value->get_type()->create_value();
    if (backup_result == ts::success)
    {
        backup_value = backup_result.extract_payload();
        ts::Result<void> construct_result = backup_value->copy_construct(*value);
        if (construct_result != ts::success)
        {
            QASSERT_MSG(false, "Cannot initialize backup value");
            //initialization failed, so this is not a proper backup value!
            backup_value = nullptr;
        }
    }

    if (!backup_value)
    {
        QASSERT_MSG(false, "Cannot create backup value");
        return nullptr;
    }

    std::shared_ptr<IValue_Editor> value_editor = m_editor_factory->create_editor(value);
    if (!value_editor)
    {
        value_editor = std::make_shared<Empty_Value_Editor>(value);
    }

    m_editor_data = std::make_shared<Editor_Data>(std::move(value_editor), std::move(backup_value));

    bool is_read_only = model->is_index_read_only(index);
    if (is_read_only)
    {
        m_editor_data->editor->set_read_only_override(true);
    }

    //m_editor_data->editingFinishedConnection = m_editor_data->editor->sig_editingFinished.connect(&Properties_Delegate::FinishEditing, const_cast<Properties_Delegate*>(this));

//    if (std::dynamic_pointer_cast<ts::IVector_Value>(value->GetValueContainer()))
//    {
//        m_editor_data->editor = jtl::make_unique<VectorElementWrapperEditor>(std::move(m_editor_data->editor));
//    }
//    if (value->IsSynchronizationEnabled())
//    {
//        m_editor_data->editor = jtl::make_unique<SynchronizationWrapperEditor>(std::move(m_editor_data->editor));
//    }

    QWidget* editor_widget = m_editor_data->editor->get_widget();

    editor_widget->setParent(parent);
    m_states[index] = QStyle::State_Raised;
    updateEditorGeometry(editor_widget, option, index);

    return editor_widget;
}

//////////////////////////////////////////////////////////////////////////

void Properties_Delegate::finish_editing()
{
    if (m_editor_data)
    {
        Q_EMIT commitData(m_editor_data->editor->get_widget());
    }
}

//////////////////////////////////////////////////////////////////////////

void Properties_Delegate::paint(QPainter* painter, const QStyleOptionViewItem& defaultOption, const QModelIndex& index) const
{
    QStyleOptionViewItem option = defaultOption;

    QStyle* style = qApp->style();

    if (index.column() == 1) // Value
    {
        Properties_Model* model = (Properties_Model*)index.model();
        std::shared_ptr<ts::IValue> value = model->get_value_from_index(index);
        std::shared_ptr<ts::IOptional_Value> v = std::dynamic_pointer_cast<ts::IOptional_Value>(value);
        if (v)
        {
            if (v->is_set())
            {
                option.font.setBold(true);
            }
            else
            {
                //option.text = v->GetEmptyString().c_str();
                QStyledItemDelegate::paint(painter, option, index);
                return;
            }
        }

    }
    QStyledItemDelegate::paint(painter, option, index);
}

//////////////////////////////////////////////////////////////////////////

void Properties_Delegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    if (m_editor_data)
    {
        m_editor_data->editor->refresh_editor();
    }
}

//////////////////////////////////////////////////////////////////////////

void Properties_Delegate::setModelData(QWidget* editor, QAbstractItemModel* _model, const QModelIndex& index) const
{
    QASSERT(m_editor_data);

    Properties_Model const* model = dynamic_cast<Properties_Model const*>(index.model());
    QVariant test_data = model->data(index);

    QASSERT(model);

    if (m_editor_data)
    {
        m_editor_data->editor->refresh_value();

//        Qualified_Value<ts::IValue> Qualified_Value = m_editor_data->editor->GetQualified_Value();
//        jtl::lent_ptr<ts::IValue> value = Qualified_Value.GetMutableValue();
//        if (value)
//        {
//            ts::ExecutionResult<bool> isNotEqualResult = value->IsNotEqual(jtl::promote_to_ref(m_editor_data->backup));
//            QASSERT_MSG(isNotEqualResult == jtl::success, "{}", isNotEqualResult.error().what());

//            std::string memberName;// = value->GetName();

//            if (index.column() == 1)
//            {
//                QModelIndex memberIndex = model->index(index.row(), index.column() - 1, index.parent());
//                QVariant memberNameVariant = model->data(memberIndex);
//                QString str = memberNameVariant.toString();
//                memberName = str.toStdString().c_str();
//            }

//            if ((isNotEqualResult == jtl::success && isNotEqualResult.payload())
//                || value->IsSynchronizationActive() != m_editor_data->backup->IsSynchronizationActive())
//            {
//                jtl::lent_ptr<tdb::IEntity> entity = model->GetRootHandle().Lock();
//                QASSERT(entity);
								
//                JTL_TODO("Save the entire entity in the undo as now we have the following bug:");
//// 				If you do the following steps :
//// 				1. create static deco “deco”
//// 				2. create material “material”
//// 				3. select a model in “deco”
//// 				4. assign “material” to “deco” submaterial 0
//// 				5. select a different model to “deco”

//                m_undoSystem->Do(jtl::make_unique<tui::AssignValueCmd>(jtl::promote_to_ref(value), std::move(jtl::promote_to_ref(m_editor_data->backup)),
//                    jtl::promote_to_ref(value),
//                    jtl::format("Entity <b>{}</b> changed param <b>{}</b> value from <b>{}</b>  to <b>{}</b> ", entity->GetName(), memberName, m_editor_data->backup->BuildUIString(), value->BuildUIString())));

//                if (m_editor_data->backup->CopyAssign(jtl::promote_to_ref(value)))
//                {
//                    m_editor_data->value_was_commited = true;
//                }
				
//            }
//            else
//            {
//                if (isNotEqualResult.payload())
//                {
//                    JLOG_VERBOSE("isNotEqualResult {}", isNotEqualResult.payload());
//                }

//                JLOG_VERBOSE("Setting value for member {} sync active {} backup {} ", memberName, value->IsSynchronizationActive(), m_editor_data->backup->IsSynchronizationActive());
//            }
//        }
    }
	
}

//////////////////////////////////////////////////////////////////////////

void Properties_Delegate::destroyEditor(QWidget *editor, const QModelIndex &index) const
{
    //cannot check for this as the m_editorData widget is sometimes wrapped in other widgets
    //QASSERT(editor == m_editorData->editor->GetWidget());
    //if the value was not committed but changed we need to revert
    if (!m_editor_data->value_was_commited)
    {
//        Qualified_Value<ts::IValue> Qualified_Value = m_editor_data->editor->GetQualified_Value();
//        jtl::lent_ptr<ts::IValue> value = Qualified_Value.GetMutableValue();
//        if (value->CopyAssign(jtl::promote_to_ref(m_editor_data->backup)))
//        {

//        }
    }
    m_editor_data.reset();
}

//////////////////////////////////////////////////////////////////////////

QSize Properties_Delegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    static QSize size;
    if (size.isEmpty())
    {
        QDoubleSpinBox w;
        size = w.sizeHint();
    }
    return size;
}

//////////////////////////////////////////////////////////////////////////

bool Properties_Delegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
//    const QModelIndex index = m_proxyModel->mapToSource(index);

//    if (event->type() == QEvent::MouseButtonPress)
//    {
//        JTL_TODO("is this doing anything?");
//        EntityModel const* m = dynamic_cast<EntityModel const*>(index.model());
//        QASSERT(m);
//        QAbstractItemView* view = dynamic_cast<QAbstractItemView*>(parent());
//        QMouseEvent* e = dynamic_cast<QMouseEvent*>(event);
//        QString type = index.data(EntityModel::TypeRole).toString();
//    }
//    else if (event->type() == QEvent::MouseMove)
//    {
//        const QString str = index.data().toString();
//        if (index.isValid() && qApp->fontMetrics().width(str) > option.rect.width())
//        {
//            QMouseEvent* me = static_cast<QMouseEvent*>(event);
//            QToolTip::showText(me->globalPos(), index.data().toString());
//            return true;
//        }
//    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

//////////////////////////////////////////////////////////////////////////

void Properties_Delegate::reset()
{
    m_editor_data.reset();
}

//////////////////////////////////////////////////////////////////////////

