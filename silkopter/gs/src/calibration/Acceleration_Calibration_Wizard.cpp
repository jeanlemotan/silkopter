#include "Acceleration_Calibration_Wizard.h"
#include <QPushButton>

#include "ui_Acceleration_Calibration_Wizard_Intro.h"
#include "ui_Acceleration_Calibration_Wizard_Reset.h"
#include "ui_Acceleration_Calibration_Wizard_Instructions.h"
#include "ui_Acceleration_Calibration_Wizard_Collect.h"
#include "ui_Acceleration_Calibration_Wizard_Done.h"

constexpr std::chrono::seconds DATA_COLLECTION_DURATION(5);
constexpr double G = 9.80665;

Acceleration_Calibration_Wizard::Acceleration_Calibration_Wizard(silk::Comms& comms,
                                                                 std::string const& node_name,
                                                                 std::string const& stream_name,
                                                                 size_t stream_rate,
                                                                 std::shared_ptr<ts::IVector_Value> points, QWidget* parent)
    : QDialog(parent)
    , m_comms(comms)
    , m_node_name(node_name)
    , m_stream_name(stream_name)
    , m_stream_rate(stream_rate)
    , m_initial_points(points)
{
    //    m_stream = std::static_pointer_cast<silk::stream::gs::Acceleration>(m_output.stream);

    {
        auto result = m_comms.set_stream_telemetry_enabled(m_stream_name, true);
        if (result != ts::success)
        {
            QMessageBox::critical(this, "Error", ("Cannot activate telemetry:\n" + result.error().what()).c_str());
        }
    }

    m_crt_points = m_initial_points->get_specialized_type()->create_specialized_value();
    auto result = m_crt_points->construct();
    QASSERT(result == ts::success);

    //    m_initial_calibration = get_calibration_points();
    //    set_calibration_points(sz::calibration::Acceleration_Points());

    m_step = Step::INTRO;

    setLayout(new QVBoxLayout(this));
    layout()->setMargin(0);
    layout()->setSpacing(0);
    layout()->setContentsMargins(4, 4, 4, 4);

    prepare_step();
}

Acceleration_Calibration_Wizard::~Acceleration_Calibration_Wizard()
{
    auto result = m_comms.set_stream_telemetry_enabled(m_stream_name, false);
    QASSERT(result == ts::success);
}

void Acceleration_Calibration_Wizard::advance()
{
    if (m_step == Step::INTRO)
    {
        m_step = Step::RESET;
    }
    else if (m_step == Step::RESET)
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
        m_collect_data_step++;
        m_step = m_collect_data_step < 6 ? Step::SHOW_INSTRUCTIONS : Step::SET_VALUE;
    }
    else if (m_step == Step::SET_VALUE)
    {
        m_step = Step::DONE;
    }

    prepare_step();
}

void Acceleration_Calibration_Wizard::cancel()
{
    m_connection.disconnect();

    //auto result = m_points->copy_assign(*m_initial_points);
    //QASSERT(result == ts::success);
    //set_calibration_points(m_initial_calibration);

    close();
}

void Acceleration_Calibration_Wizard::prepare_step()
{
    delete m_content;
    m_content = nullptr;

    if (m_step == Step::INTRO)
    {
        m_content = new QWidget(this);
        layout()->addWidget(m_content);

        Ui::Acceleration_Calibration_Wizard_Intro ui;
        ui.setupUi(m_content);
        ui.description->setText(q::util::format<std::string>(
                                    "This wizard will calibrate the acceleration stream of node {}.\n"
                                    "It will ask you to hold the UAV still in various positions and will take about 1 minute.\n"
                                    "\n"
                                    "Ready?", m_node_name).c_str());

        QObject::connect(ui.buttonBox, &QDialogButtonBox::accepted, [this]() { advance(); });
        QObject::connect(ui.buttonBox, &QDialogButtonBox::rejected, [this]() { cancel(); });
    }
    else if (m_step == Step::RESET)
    {
        m_content = new QWidget(this);
        layout()->addWidget(m_content);

        Ui::Acceleration_Calibration_Wizard_Reset ui;
        ui.setupUi(m_content);
        if (m_initial_points->get_value_count() > 0)
        {
            ui.info->setText(q::util::format<std::string>("There are currently {} calibration points.\n"
                                                          "Do you want to clear these points or keep them?", m_initial_points->get_value_count()).c_str());
            QPushButton* clear = ui.buttonBox->addButton("Clear", QDialogButtonBox::ResetRole);
            QObject::connect(clear, &QPushButton::released, [this]() { advance(); });

            QPushButton* keep = ui.buttonBox->addButton("Keep", QDialogButtonBox::AcceptRole);
            QObject::connect(keep, &QPushButton::released, [this]()
            {
                auto result = m_crt_points->copy_assign(*m_initial_points);
                QASSERT(result == ts::success);
                advance();
            });
        }
        else
        {
            ui.info->setText("There are no existing calibration data points.\nLet's add one");
            QPushButton* ok = ui.buttonBox->addButton("Ok", QDialogButtonBox::AcceptRole);
            QObject::connect(ok, &QPushButton::released, [this]() { advance(); });
        }


        QObject::connect(ui.buttonBox, &QDialogButtonBox::rejected, [this]() { cancel(); });
    }
    else if (m_step == Step::SHOW_INSTRUCTIONS)
    {
        static char const* direction[] =
        {
            "level",
            "on its LEFT side",
            "on its RIGHT side",
            "nose DOWN",
            "nose UP",
            "on its BACK",
        };

        m_content = new QWidget(this);
        layout()->addWidget(m_content);

        Ui::Acceleration_Calibration_Wizard_Instructions ui;
        ui.setupUi(m_content);
        ui.instructions->setText(q::util::format<std::string>(
                                     "Please place the UAV {} for {} seconds.\n"
                                     "\n"
                                     "Done?", direction[m_collect_data_step], DATA_COLLECTION_DURATION).c_str());

        QObject::connect(ui.buttonBox, &QDialogButtonBox::accepted, [this]() { advance(); });
        QObject::connect(ui.buttonBox, &QDialogButtonBox::rejected, [this]() { cancel(); });
    }
    else if (m_step == Step::COLLECT)
    {
        m_content = new QWidget(this);
        layout()->addWidget(m_content);

        Ui::Acceleration_Calibration_Wizard_Collect ui;
        ui.setupUi(m_content);
        auto* info = ui.info;
        QProgressBar* progress = ui.progressBar;

        m_samples.clear();

        QObject::connect(ui.buttonBox, &QDialogButtonBox::rejected, [this]() { cancel(); });

        m_connection = m_comms.sig_telemetry_samples_available.connect([this, info, progress](silk::Comms::ITelemetry_Stream const& _stream)
        {
            if (_stream.stream_path == m_stream_name)
            {
                auto const* stream = dynamic_cast<silk::Comms::Telemetry_Stream<silk::stream::IAcceleration> const*>(&_stream);
                if (stream)
                {
                    on_samples_received(stream->samples);
                    info->setText(q::util::format<std::string>("Collected {} samples...", m_samples.size()).c_str());
                    size_t needed_samples = std::chrono::seconds(DATA_COLLECTION_DURATION).count() * m_stream_rate;
                    progress->setValue(float(m_samples.size() * 100.f) / float(needed_samples));
                    if (m_samples.size() >= needed_samples)
                    {
                        m_averages[m_collect_data_step].set(std::accumulate(m_samples.begin(), m_samples.end(), math::vec3f()));
                        m_averages[m_collect_data_step] /= static_cast<double>(m_samples.size());

                        advance();
                    }
                }
            }
        });
    }
    else if (m_step == Step::SET_VALUE)
    {
        math::vec3<double> bias, scale;
        compute_calibration_data(m_averages, bias, scale);

        m_content = new QWidget(this);
        layout()->addWidget(m_content);

        Ui::Acceleration_Calibration_Wizard_Done ui;
        ui.setupUi(m_content);

        ui.info->setText("Done!\n"
                         "The new Bias & Scale are:");
        ui.bias->setText(q::util::format<std::string>("{}", bias).c_str());
        ui.scale->setText(q::util::format<std::string>("{}", scale).c_str());

        //sz::calibration::Acceleration point;
        ts::Result<std::shared_ptr<ts::IValue>> result = m_crt_points->insert_default_value(m_crt_points->get_value_count());
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
            auto result = m_initial_points->copy_assign(*m_crt_points);
            QASSERT(result == ts::success);
            //set_calibration_points(m_crt_calibration);
            this->accept();
        });
        QObject::connect(ui.buttonBox, &QDialogButtonBox::rejected, [this]() { cancel(); });
    }
    else if (m_step == Step::DONE)
    {
        //m_comms.
        this->accept();
    }}

//void Acceleration_Calibration_Wizard::set_calibration_points(sz::calibration::Acceleration_Points const& data)
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

//auto Acceleration_Calibration_Wizard::get_calibration_points() const -> sz::calibration::Acceleration_Points
//{
//    q::Path path("Calibration");
//    path += m_output.name;
//    auto const* calibrationj = jsonutil::find_value(m_node->config, path);
//    if (!calibrationj)
//    {
//        QASSERT(0);
//        return sz::calibration::Acceleration_Points();
//    }
//    sz::calibration::Acceleration_Points calibration;
//    autojsoncxx::error::ErrorStack result;
//    if (!autojsoncxx::from_value(calibration, *calibrationj, result))
//    {
//        QASSERT(0);
//        return sz::calibration::Acceleration_Points();
//    }
//    return calibration;
//}


void Acceleration_Calibration_Wizard::on_samples_received(std::vector<silk::stream::IAcceleration::Sample> const& samples)
{
    m_samples.reserve(m_samples.size() + samples.size());
    for (auto const& s: samples)
    {
        m_samples.push_back(s.value);
    }
}

auto Acceleration_Calibration_Wizard::compute_calibration_data(std::array<math::vec3<double>, 6> const& samples, math::vec3<double>& bias, math::vec3<double>& scale) -> bool
{
    //code copied from arducopter
    int iteration_count = 0;
    static const double s_eps = 0.000000001;
    static const int s_iterations = 2000;
    double change = 100.0;
    double data[3];
    double beta[6];
    double delta[6];
    double ds[6];
    double JS[6][6];
    bool success = true;

    // reset
    beta[0] = beta[1] = beta[2] = 0;
    beta[3] = beta[4] = beta[5] = 1.0f / G;

    while (iteration_count < s_iterations && change > s_eps)
    {
        iteration_count++;

        calibrate_reset_matrices(ds, JS);

        for (int i = 0; i < 6; i++)
        {
            data[0] = samples[i].x;
            data[1] = samples[i].y;
            data[2] = samples[i].z;
            calibrate_update_matrices(ds, JS, beta, data);
        }

        calibrate_find_delta(ds, JS, delta);

        change = delta[0] * delta[0] +
                delta[0] * delta[0] +
                delta[1] * delta[1] +
                delta[2] * delta[2] +
                delta[3] * delta[3] / (beta[3] * beta[3]) +
                delta[4] * delta[4] / (beta[4] * beta[4]) +
                delta[5] * delta[5] / (beta[5] * beta[5]);

        for (int i = 0; i < 6; i++)
        {
            beta[i] -= delta[i];
        }
    }

    // copy results out
    scale.x = beta[3] * G;
    scale.y = beta[4] * G;
    scale.z = beta[5] * G;
    bias.x = beta[0] * scale.x;
    bias.y = beta[1] * scale.y;
    bias.z = beta[2] * scale.z;

    // sanity check scale
    // 	if (scale.is_nan() || fabsf(scale.x - 1.0f) > 0.1f || fabsf(scale.y - 1.0f) > 0.1f || fabsf(scale.z - 1.0f) > 0.1f)
    // 	{
    // 		success = false;
    // 	}
    // 	// sanity check offsets (3.5 is roughly 3/10th of a G, 5.0 is roughly half a G)
    // 	if (bias.is_nan() || fabsf(bias.x) > 3.5f || fabsf(bias.y) > 3.5f || fabsf(bias.z) > 3.5f)
    // 	{
    // 		success = false;
    // 	}

    // return success or failure
    return success;
}

void Acceleration_Calibration_Wizard::calibrate_update_matrices(double dS[6], double JS[6][6], double beta[6], double data[3])
{
    //code copied from arducopter
    double residual = 1.0;
    double jacobian[6];

    for (int j = 0; j < 3; j++)
    {
        double b = beta[3 + j];
        double dx = data[j] - beta[j];
        residual -= b*b*dx*dx;
        jacobian[j] = 2.0f*b*b*dx;
        jacobian[3 + j] = -2.0f*b*dx*dx;
    }

    for (int j = 0; j < 6; j++)
    {
        dS[j] += jacobian[j] * residual;
        for (int k = 0; k < 6; k++)
        {
            JS[j][k] += jacobian[j] * jacobian[k];
        }
    }
}


// _calibrate_reset_matrices - clears matrices
void Acceleration_Calibration_Wizard::calibrate_reset_matrices(double dS[6], double JS[6][6])
{
    //code copied from arducopter
    for (int j = 0; j < 6; j++)
    {
        dS[j] = 0.0f;
        for (int k = 0; k < 6; k++)
        {
            JS[j][k] = 0.0f;
        }
    }
}

void Acceleration_Calibration_Wizard::calibrate_find_delta(double dS[6], double JS[6][6], double delta[6])
{
    //code copied from arducopter
    //Solve 6-d matrix equation JS*x = dS
    //first put in upper triangular form

    //make upper triangular
    for (int i = 0; i < 6; i++)
    {
        //eliminate all nonzero entries below JS[i][i]
        for (int j = i + 1; j < 6; j++)
        {
            double mu = JS[i][j] / JS[i][i];
            if (mu != 0.0)
            {
                dS[j] -= mu*dS[i];
                for (int k = j; k < 6; k++)
                {
                    JS[k][j] -= mu*JS[k][i];
                }
            }
        }
    }

    //back-substitute
    for (int i = 5; i >= 0; i--)
    {
        dS[i] /= JS[i][i];
        JS[i][i] = 1.0;

        for (int j = 0; j < i; j++)
        {
            double mu = JS[i][j];
            dS[j] -= mu*dS[i];
            JS[i][j] = 0.0;
        }
    }

    for (int i = 0; i < 6; i++)
    {
        delta[i] = dS[i];
    }
}
