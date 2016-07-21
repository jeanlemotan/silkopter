#include "calibration/Magnetic_Field_Calibration_Wizard.h"
#include <QPushButton>

#include "ui_Magnetic_Field_Calibration_Wizard_Reset.h"
#include "ui_Magnetic_Field_Calibration_Wizard_Instructions.h"
#include "ui_Magnetic_Field_Calibration_Wizard_Collect.h"
#include "ui_Magnetic_Field_Calibration_Wizard_Done.h"

constexpr std::chrono::seconds DATA_COLLECTION_DURATION(60);

Magnetic_Field_Calibration_Wizard::Magnetic_Field_Calibration_Wizard(silk::Comms& comms,
                                                                 std::string const& node_name,
                                                                 std::string const& stream_name,
                                                                 size_t stream_rate,
                                                                 std::shared_ptr<ts::IStruct_Value> config,
                                                                 std::shared_ptr<ts::IVector_Value> points,
                                                                 QWidget* parent)
    : QDialog(parent)
    , m_comms(comms)
    , m_node_name(node_name)
    , m_stream_name(stream_name)
    , m_stream_rate(stream_rate)
    , m_config(config)
    , m_points(points)
{
//    m_stream = std::static_pointer_cast<silk::stream::gs::Magnetic_Field>(m_output.stream);

    {
        auto result = m_comms.set_stream_telemetry_enabled(m_stream_name, true);
        if (result != ts::success)
        {
            QMessageBox::critical(this, "Error", ("Cannot activate telemetry:\n" + result.error().what()).c_str());
        }
    }

    //make a clone of the initial points
    {
        m_initial_points = points->get_specialized_type()->create_specialized_value();
        auto result = m_initial_points->copy_construct(*points);
        QASSERT(result == ts::success);
    }

    //create the new points - empty
    {
        m_new_points = m_initial_points->get_specialized_type()->create_specialized_value();
        auto result = m_new_points->construct();
        QASSERT(result == ts::success);
    }

    //clear the current points
    {
        m_points->clear();
        auto result = m_comms.set_node_config(node_name, m_config);
        if (result != ts::success)
        {
            QMessageBox::critical(this, "Error", ("Cannot reset calibration data for node '" + node_name + "':\n" + result.error().what()).c_str());
        }
    }

    m_step = Step::RESET;

    setLayout(new QVBoxLayout(this));
    layout()->setMargin(0);
    layout()->setSpacing(0);
    layout()->setContentsMargins(4, 4, 4, 4);

    prepare_step();
}

Magnetic_Field_Calibration_Wizard::~Magnetic_Field_Calibration_Wizard()
{
    auto result = m_comms.set_stream_telemetry_enabled(m_stream_name, false);
    if (result != ts::success)
    {
        QLOGE("Failed to disable stream '{}' telemetry: {}", m_stream_name, result.error().what());
    }
}

void Magnetic_Field_Calibration_Wizard::advance()
{
    if (m_step == Step::RESET)
    {
        m_step = Step::SHOW_INSTRUCTIONS;
    }
    else if (m_step == Step::SHOW_INSTRUCTIONS)
    {
        m_step = Step::COLLECT;
    }
    else if (m_step == Step::COLLECT)
    {
        m_connection.disconnect();
        m_step = Step::SET_VALUE;
    }
    else if (m_step == Step::SET_VALUE)
    {
        m_step = Step::DONE;
    }

    prepare_step();
}

void Magnetic_Field_Calibration_Wizard::cancel()
{
    m_connection.disconnect();

    auto result = m_points->copy_assign(*m_initial_points);
    QASSERT(result == ts::success);

    close();
}

void Magnetic_Field_Calibration_Wizard::prepare_step()
{
    delete m_content;
    m_content = nullptr;

    if (m_step == Step::RESET)
    {
        m_content = new QWidget(this);
        layout()->addWidget(m_content);

        Ui::Magnetic_Field_Calibration_Wizard_Reset ui;
        ui.setupUi(m_content);
        if (m_initial_points->get_value_count() > 0)
        {
            ui.info->setText(q::util::format<std::string>("There are currently {} calibration points.\n"
                                                           "Do you want to clear these points or keep them?", m_initial_points->get_value_count()).c_str());
            auto* clear = ui.buttonBox->addButton("Clear", QDialogButtonBox::ResetRole);
            QObject::connect(clear, &QPushButton::released, [this]() { advance(); });

            auto* keep = ui.buttonBox->addButton("Keep", QDialogButtonBox::AcceptRole);
            QObject::connect(keep, &QPushButton::released, [this]()
            {
                auto result = m_new_points->copy_assign(*m_initial_points);
                QASSERT(result == ts::success);
                advance();
            });
        }
        else
        {
            ui.info->setText("There are no existing calibration data points.\nLet's add one");
            auto* ok = ui.buttonBox->addButton("Ok", QDialogButtonBox::AcceptRole);
            QObject::connect(ok, &QPushButton::released, [this]() { advance(); });
        }


        QObject::connect(ui.buttonBox, &QDialogButtonBox::rejected, [this]() { cancel(); });
    }
    else if (m_step == Step::SHOW_INSTRUCTIONS)
    {
        m_content = new QWidget(this);
        layout()->addWidget(m_content);

        Ui::Magnetic_Field_Calibration_Wizard_Instructions ui;
        ui.setupUi(m_content);
        ui.instructions->setText(q::util::format<std::string>(
                                    "Please rotate the UAV along all its axis for {} seconds.\n"
                                    "\n"
                                    "Ready?", DATA_COLLECTION_DURATION).c_str());

        QObject::connect(ui.buttonBox, &QDialogButtonBox::accepted, [this]() { advance(); });
        QObject::connect(ui.buttonBox, &QDialogButtonBox::rejected, [this]() { cancel(); });
    }
    else if (m_step == Step::COLLECT)
    {
        m_content = new QWidget(this);
        layout()->addWidget(m_content);

        Ui::Magnetic_Field_Calibration_Wizard_Collect ui;
        ui.setupUi(m_content);
        auto* info = ui.info;
        auto* progress = ui.progressBar;
        auto* bias = ui.bias;
        auto* scale = ui.scale;

        m_samples.clear();

        QObject::connect(ui.buttonBox, &QDialogButtonBox::rejected, [this]() { cancel(); });

        m_connection = m_comms.sig_telemetry_samples_available.connect([this, info, progress, bias, scale](silk::Comms::ITelemetry_Stream const& _stream)
        {
            if (_stream.stream_path == m_stream_name)
            {
                auto const* stream = dynamic_cast<silk::Comms::Telemetry_Stream<silk::stream::IMagnetic_Field> const*>(&_stream);
                if (stream)
                {
                    on_samples_received(stream->samples);
                    info->setText(q::util::format<std::string>("Collected {} samples...", m_samples.size()).c_str());
                    size_t needed_samples = std::chrono::seconds(DATA_COLLECTION_DURATION).count() * m_stream_rate;
                    progress->setValue(float(m_samples.size() * 100.f) / float(needed_samples));
                    bias->setText(q::util::format<std::string>("{}", m_box.get_center()).c_str());

                    auto extent = math::max(m_box.get_extent(), math::vec3f(1.f));
                    float avg = (extent.x + extent.y + extent.z) / 3.f;

                    scale->setText(q::util::format<std::string>("{}", avg / extent).c_str());

                    if (m_samples.size() >= needed_samples)
                    {
                        advance();
                    }
                }
            }
        });
    }
    else if (m_step == Step::SET_VALUE)
    {
        math::vec3f bias = m_box.get_center();

        auto extent = math::max(m_box.get_extent(), math::vec3f(1.f));
        float avg = (extent.x + extent.y + extent.z) / 3.f;
        math::vec3f scale = avg / extent;

        m_content = new QWidget(this);
        layout()->addWidget(m_content);

        Ui::Magnetic_Field_Calibration_Wizard_Done ui;
        ui.setupUi(m_content);

        ui.info->setText("Done!\n"
                         "The new Bias & Scale are:");
        ui.bias->setText(q::util::format<std::string>("{}", bias).c_str());
        ui.scale->setText(q::util::format<std::string>("{}", scale).c_str());

        ts::Result<std::shared_ptr<ts::IValue>> result = m_new_points->insert_default_value(m_new_points->get_value_count());
        if (result != ts::success)
        {
            QMessageBox::critical(this, "Error", ("Cannot insert calibration value: " + result.error().what()).c_str());
            return;
        }

        std::shared_ptr<ts::IValue> point = result.extract_payload();

        std::shared_ptr<ts::IFloat_Value> temperature_value = point->select_specialized<ts::IFloat_Value>("temperature");
        if (!temperature_value)
        {
            QMessageBox::critical(this, "Error", "Invalid calibration point type: missing float 'temperature' member value");
            return;
        }
        auto set_result = temperature_value->set_value(0);
        QASSERT(set_result == ts::success);

        std::shared_ptr<ts::IVec3f_Value> bias_value = point->select_specialized<ts::IVec3f_Value>("bias");
        if (!bias_value)
        {
            QMessageBox::critical(this, "Error", "Invalid calibration point type: missing vec3f 'bias' member value");
            return;
        }
        set_result = bias_value->set_value(ts::vec3f(bias.x, bias.y, bias.z));
        QASSERT(set_result == ts::success);

        std::shared_ptr<ts::IVec3f_Value> scale_value = point->select_specialized<ts::IVec3f_Value>("scale");
        if (!scale_value)
        {
            QMessageBox::critical(this, "Error", "Invalid calibration point type: missing vec3f 'scale' member value");
            return;
        }
        set_result = scale_value->set_value(ts::vec3f(scale.x, scale.y, scale.z));
        QASSERT(set_result == ts::success);

        QObject::connect(ui.temperature, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [this, temperature_value](double value)
        {
            auto set_result = temperature_value->set_value(static_cast<float>(value));
            QASSERT(set_result == ts::success);
        });

        QObject::connect(ui.buttonBox, &QDialogButtonBox::accepted, [this]()
        {
            auto result = m_points->copy_assign(*m_new_points);
            QASSERT(result == ts::success);
            advance();
        });
        QObject::connect(ui.buttonBox, &QDialogButtonBox::rejected, [this]() { cancel(); });
    }
    else if (m_step == Step::DONE)
    {
        this->accept();
    }
}

//void Magnetic_Field_Calibration_Wizard::set_calibration_points(sz::calibration::Magnetic_Field_Points const& data)
//{
//    rapidjson::Document calibrationj;
//    calibrationj.SetObject();
//    autojsoncxx::to_document(data, calibrationj);

//    q::Path path("Calibration");
//    path += m_output.name;

//    rapidjson::Document configj = jsonutil::clone_value(m_node->config);
//    if (!jsonutil::remove_value(configj, path))
//    {
//        QASSERT(0);
//        return;
//    }

//    if (!jsonutil::add_value(configj, path, std::move(calibrationj), configj.GetAllocator()))
//    {
//        QASSERT(0);
//        return;
//    }

//    m_hal.set_node_config(m_node, configj);
//}

//auto Magnetic_Field_Calibration_Wizard::get_calibration_points() const -> sz::calibration::Magnetic_Field_Points
//{
//    q::Path path("Calibration");
//    path += m_output.name;
//    auto const* calibrationj = jsonutil::find_value(m_node->config, path);
//    if (!calibrationj)
//    {
//        QASSERT(0);
//        return sz::calibration::Magnetic_Field_Points();
//    }
//    sz::calibration::Magnetic_Field_Points calibration;
//    autojsoncxx::error::ErrorStack result;
//    if (!autojsoncxx::from_value(calibration, *calibrationj, result))
//    {
//        QASSERT(0);
//        return sz::calibration::Magnetic_Field_Points();
//    }
//    return calibration;
//}

void Magnetic_Field_Calibration_Wizard::on_samples_received(std::vector<silk::stream::IMagnetic_Field::Sample> const& samples)
{
    m_samples.reserve(m_samples.size() + samples.size());
    for (auto const& s: samples)
    {
        if (m_samples.empty())
        {
            m_box.reset(s.value, s.value);
        }
        else
        {
            m_box.add_internal_point(s.value);
        }
        m_samples.push_back(s.value);
    }
}
