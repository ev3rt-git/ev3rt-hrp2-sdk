/**
 * Benchmark for EV3RT based on EV3RT C++ API.
 *
 * Author: Jaroslav Páral (jarekparal)
 */

#include "ev3cxx.h"
#include "app.h"

#include <array>
#include <vector>
#include <algorithm>
#include <cmath>
#include <utility>
#include <functional>
#include <string>
#include <fstream>

struct Display : ev3cxx::detail::Display {
    Display(int line_length = 4)
        : ev3cxx::detail::Display(ev3cxx::display),
          m_line_length(line_length)
    {
        setTextLine(0);
    }
    void write(char ch) {
        if (ch == '\r') {
            setTextLine(getTextLine(), 0, false);
            clearTextLine(getTextLine(), white, 0, m_line_length);
        } else {
            ev3cxx::detail::Display::write(ch);
        }
    }
    void setLineLength(int line_length) { m_line_length = line_length; }
private:
    int m_line_length;
};

struct StringWriter {
    StringWriter(std::string& str) : str(str) {}
    void write(char c) { str += c; }
    std::string& str;
};

struct FileEmulator {
    FileEmulator()
        : m_buff(),
          m_opened(false)
    {}
    void open(const char *filename = "", const char *mode = "w+") { m_opened = true; }
    void close() { m_opened = false; }
    bool isOpen() const { return m_opened; }
    void write(char c) { if (m_opened) m_buff += c; }
    std::string& buffer() { return m_buff; }
private:
    std::string m_buff;
    bool m_opened;
};

std::string make_valid_identifier(std::string str) {
    for (char& c: str) {
        switch(c) {
            case ' ': c = '_'; break;
            default : c =  c ; break;
        }
    }
    return str;
}

template <class ForwardIterator>
std::string to_py_list(std::string name, ForwardIterator first, ForwardIterator last) {
    if (!name.empty())
        name += " = ";
    name += "[ ";
    if (first == last) {
        name += "]";
    } else {
        StringWriter str(name);
        for (; first != last; ++first)
            ev3cxx::format(str, "% , ") % *first;
        name[name.length()-2] = ' ';
        name[name.length()-1] = ']';
    }
    return name;
}

template <typename T, typename Stream>
void add_item(Stream& stream, const std::string& name, const T& value, size_t indent = 1, bool first = false) {
    std::string Indent(indent * 4 + (first ? 1 : 2), ' ');
    if (first) {
        Indent[0] = '\n';
    } else {
        Indent[0] = ',';
        Indent[1] = '\n';
    }
    ev3cxx::format(stream, "% '% ': % ") % Indent % name % value;
}

template <typename Stream>
void add_item(Stream& stream, const std::string& name, const std::string& value, size_t indent = 1, bool first = false) {
    add_item<std::string>(stream, name, '\'' + value + '\'', indent, first);
}

template <typename Stream>
void add_item(Stream& stream, const std::string& name, const char* value, size_t indent = 1, bool first = false) {
    add_item<std::string>(stream, name, '\'' + std::string(value) + '\'', indent, first);
}

template <class ForwardIterator, typename Stream>
void add_list(Stream& stream, const std::string& name, ForwardIterator first, ForwardIterator last, size_t indent = 1, bool is_first = false) {
    std::string Indent(indent * 4 + (is_first ? 1 : 2), ' ');
    if (is_first) {
        Indent[0] = '\n';
    } else {
        Indent[0] = ',';
        Indent[1] = '\n';
    }
    ev3cxx::format(stream, "% '% ': [ ") % Indent % name;
    if (first == last) {
        stream.write(']');
    } else {
        ev3cxx::format(stream, "% ") % *first++;
        for (; first != last; ++first)
            ev3cxx::format(stream, ", % ") % *first;
        ev3cxx::format(stream, " ]");
    }
}

template <typename sum_type, class ForwardIterator>
sum_type avg(ForwardIterator first, ForwardIterator last) {
    if (first == last)
        return 0;
    sum_type len = (last - first);
    sum_type sum = *first;
    while(++first != last)
        sum += *first;
    return sum / len;
}

template <class ForwardIterator>
auto mean(ForwardIterator first, ForwardIterator last) -> decltype(first) {
    if (first == last)
        return decltype(first)(nullptr);
    size_t len = (last - first) / 2;
    std::sort(first, last);
    return first + len;
}

struct ofstream : std::ofstream {
    ofstream(const char* name)
        : std::ofstream(name)
    {}
    void write(char c) { put(c); }
};

typedef ev3cxx::StopWatch::time_type Time_t;

struct test_t {
    typedef std::function<void(uint32_t)> function_type;
    typedef std::function<void()> init_function_type;
    typedef std::function<void()> clean_function_type;

    std::string name;
    uint32_t operations_per_sample;
    uint32_t samples;
    function_type function;
    init_function_type init_function;
    clean_function_type clean_function;

    test_t(
        std::string Name,
        uint32_t Operations_per_sample,
        uint32_t Samples,
        function_type Function,
        init_function_type Init_function = [](){},
        clean_function_type Clean_function = [](){}
    ) : name(Name),
        operations_per_sample(Operations_per_sample),
        samples(Samples),
        function(Function),
        init_function(Init_function),
        clean_function(Clean_function)
    {}

    operator std::string() { return make_valid_identifier(this->name); }
};

std::vector<Time_t> results;
std::vector<Time_t> times;

Display display;

ev3cxx::BrickButton next_btn(ev3cxx::BrickButtons::ENTER);
ev3cxx::BrickButton skip_btn(ev3cxx::BrickButtons::BACK);

ev3cxx::Motor tst_MediumMotor(ev3cxx::MotorPort::A, ev3cxx::MotorType::MEDIUM);
ev3cxx::MotorTank tst_MotorsTank(ev3cxx::MotorPort::B, ev3cxx::MotorPort::C);

ev3cxx::BrickButton tst_BrickButton(ev3cxx::BrickButtons::UP);

ev3cxx::TouchSensor tst_TouchSensor(ev3cxx::SensorPort::S1);
ev3cxx::ColorSensor tst_ColorSensor(ev3cxx::SensorPort::S2);
ev3cxx::UltrasonicSensor tst_UltrasonicSensor(ev3cxx::SensorPort::S3);
ev3cxx::GyroSensor tst_GyroSensor(ev3cxx::SensorPort::S4);

volatile int    ia = 42;
volatile int    ib = 25;
volatile int    ic = 0;
volatile float  fa = 42;
volatile float  fb = 25;
volatile float  fc = 0;
volatile double da = 42;
volatile double db = 25;
volatile double dc = 0;
volatile ev3cxx::StatusLightColor c = ev3cxx::StatusLightColor::GREEN;
volatile colorid_t cid;

constexpr uint32_t short_operations_per_sample = 100000;
constexpr uint32_t middle_operations_per_sample = 1000;
constexpr uint32_t long_operations_per_sample = 100;
constexpr uint32_t xlong_operations_per_sample = 1;
uint32_t samples = 1000;

test_t tests[] = {
    test_t("empty call",     short_operations_per_sample, samples, [&](uint32_t i){} ),
    test_t("int sum",        short_operations_per_sample, samples, [&](uint32_t i){ ic = ib + ia; } ),
    test_t("float sum",      short_operations_per_sample, samples, [&](uint32_t i){ fc = fb + fa; } ),
    test_t("double sum",     short_operations_per_sample, samples, [&](uint32_t i){ dc = db + da; } ),
    test_t("int mult",       short_operations_per_sample, samples, [&](uint32_t i){ ic = ib * ia; } ),
    test_t("float mult",     short_operations_per_sample, samples, [&](uint32_t i){ fc = fb * fa; } ),
    test_t("double mult",    short_operations_per_sample, samples, [&](uint32_t i){ dc = db * da; } ),
    test_t("int div",        short_operations_per_sample, samples, [&](uint32_t i){ ic = ib / ia; } ),
    test_t("float div",     middle_operations_per_sample, samples, [&](uint32_t i){ fc = fb / fa; } ),
    test_t("double div",    middle_operations_per_sample, samples, [&](uint32_t i){ dc = db / da; } ),
    test_t("float pow",       long_operations_per_sample, samples, [&](uint32_t i){ fc = std::pow(fb, fa); } ),
    test_t("double pow",      long_operations_per_sample, samples, [&](uint32_t i){ dc = std::pow(db, da); } ),
    test_t("float sqrt",    middle_operations_per_sample, samples, [&](uint32_t i){ fc = std::sqrt(fa); } ),
    test_t("double sqrt",   middle_operations_per_sample, samples, [&](uint32_t i){ dc = std::sqrt(da); } ),
    test_t("float sin",       long_operations_per_sample, samples, [&](uint32_t i){ fc = std::sin(fa); } ),
    test_t("double sin",      long_operations_per_sample, samples, [&](uint32_t i){ dc = std::sin(da); } ),
    test_t("condition",      short_operations_per_sample, samples, [&](uint32_t i){ c = c == ev3cxx::StatusLightColor::RED ? ev3cxx::StatusLightColor::GREEN : ev3cxx::StatusLightColor::RED; } ),
    test_t("brick button",   short_operations_per_sample, samples, [&](uint32_t i){ ic = tst_BrickButton.isPressed(); } ),
    test_t("brick led",     middle_operations_per_sample, samples, [&](uint32_t i){ c = c == ev3cxx::StatusLightColor::RED ? ev3cxx::StatusLightColor::GREEN : ev3cxx::StatusLightColor::RED; ev3cxx::statusLight.setColor(c); } ),
    test_t("brick display",   long_operations_per_sample, samples, [&](uint32_t i){ display.format("i = % \r") % i; } ),
    test_t("motor power",     long_operations_per_sample, samples, [&](uint32_t i){ tst_MediumMotor.unregulated(20 + (i & 0x1f)); }, [&](){ tst_MediumMotor.unregulated(15); }, [&](){ tst_MediumMotor.off(); } ),
    test_t("motor speed",     long_operations_per_sample, samples, [&](uint32_t i){ tst_MediumMotor.on(20 + (i & 0x1f)); },          [&](){ tst_MediumMotor.on(15); },          [&](){ tst_MediumMotor.off(); } ),
    test_t("motor mode",    middle_operations_per_sample, samples, [&](uint32_t i){ if (i & 1) tst_MediumMotor.on(20); else tst_MediumMotor.unregulated(20); }, [&](){ tst_MediumMotor.on(20); }, [&](){ tst_MediumMotor.off(); } ),
    test_t("motors power",    long_operations_per_sample, samples, [&](uint32_t i){ tst_MotorsTank.unregulated(25 + (i & 0x1f), -25 + (i & 0x1f)); }, [&](){ tst_MotorsTank.unregulated(20, -20); }, [&](){ tst_MotorsTank.off(); } ),
    test_t("motors speed",    long_operations_per_sample, samples, [&](uint32_t i){ tst_MotorsTank.on(-25 + (i & 0x1f), 25 + (i & 0x1f)); },          [&](){ tst_MotorsTank.on(-20, 20); },          [&](){ tst_MotorsTank.off(); } ),
    test_t("motors mode",   middle_operations_per_sample, samples, [&](uint32_t i){ if (i & 1) tst_MotorsTank.on(20, 20); else tst_MotorsTank.unregulated(20, 20); }, [&](){ tst_MotorsTank.on(15, 15); }, [&](){ tst_MotorsTank.off(); } ),
    test_t("touch sensor",   short_operations_per_sample, samples, [&](uint32_t i){ ic = tst_TouchSensor.isPressed(); } ),
    test_t("CS reflected",    long_operations_per_sample, samples, [&](uint32_t i){ ic = tst_ColorSensor.reflected(); }, [&](){ ic = tst_ColorSensor.reflected(); } ),
    test_t("CS ambient",      long_operations_per_sample, samples, [&](uint32_t i){ ic = tst_ColorSensor.ambient(); }, [&](){ ic = tst_ColorSensor.ambient(); } ),
    test_t("CS color",        long_operations_per_sample, samples, [&](uint32_t i){ cid = tst_ColorSensor.color(); }, [&](){ cid = tst_ColorSensor.color(); } ),
    test_t("CS raw rgb",      long_operations_per_sample, samples, [&](uint32_t i){ tst_ColorSensor.reflectedRawRgb(); }, [&](){ tst_ColorSensor.reflectedRawRgb(); } ),
    test_t("CS mode",        xlong_operations_per_sample, samples, [&](uint32_t i){ if (i & 1) tst_ColorSensor.reflectedRawRgb(); else tst_ColorSensor.ambient(); } ),
    test_t("UTS cm",        middle_operations_per_sample, samples, [&](uint32_t i){ ic = tst_UltrasonicSensor.centimeters(); }, [&](){ ic = tst_UltrasonicSensor.centimeters(); } ),
    test_t("UTS mm",        middle_operations_per_sample, samples, [&](uint32_t i){ ic = tst_UltrasonicSensor.millimeters(); }, [&](){ ic = tst_UltrasonicSensor.millimeters(); } ),
    test_t("UTS inch",      middle_operations_per_sample, samples, [&](uint32_t i){ ic = tst_UltrasonicSensor.inches(); },      [&](){ ic = tst_UltrasonicSensor.inches(); } ),
    test_t("UTS inchLine",  middle_operations_per_sample, samples, [&](uint32_t i){ ic = tst_UltrasonicSensor.inchesLine(); },  [&](){ ic = tst_UltrasonicSensor.inchesLine(); } ),
    test_t("UTS listen",    middle_operations_per_sample, samples, [&](uint32_t i){ ic = tst_UltrasonicSensor.listen(); },      [&](){ ic = tst_UltrasonicSensor.listen(); } ),
    test_t("UTS mode",       xlong_operations_per_sample, samples, [&](uint32_t i){ if (i & 1) tst_UltrasonicSensor.listen(); else tst_UltrasonicSensor.centimeters(); } ),
    test_t("GYRO angle",      long_operations_per_sample, samples, [&](uint32_t i){ ic = tst_GyroSensor.angle(); }, [&](){ ic = tst_GyroSensor.angle(); } ),
    test_t("GYRO rate",       long_operations_per_sample, samples, [&](uint32_t i){ ic = tst_GyroSensor.rate(); }, [&](){ ic = tst_GyroSensor.rate(); } ),
    test_t("GYRO reset",      long_operations_per_sample, samples, [&](uint32_t i){ tst_GyroSensor.reset(); }, [&](){ tst_GyroSensor.reset(); } ),
    test_t("GYRO HardRST",   xlong_operations_per_sample, samples, [&](uint32_t i){ tst_GyroSensor.resetHard(); }, [&](){ tst_GyroSensor.resetHard(); } ),
};

const int tests_len = sizeof(tests) / sizeof(test_t);

const char file_name[] = "Benchmarks_results.py";

void main_task(intptr_t unused) {
    ev3cxx::StopWatch run_time;

    samples = std::max_element(tests, tests + tests_len, [](test_t& a, test_t& b){ return a.samples < b.samples; })->samples;

    uint8_t digits = std::ceil(std::log10(samples)) + 1;
    if (digits > 9)
        digits = 9;
    display.setLineLength(digits);
    display.setFont(EV3_FONT_MEDIUM);

    char test_progress_format[] = "\r%0";
    test_progress_format[2] += digits;

    digits = std::ceil(std::log10(tests_len));
    if (digits > 9)
        digits = 9;
    char progress_format[] = "%0/%0";
    progress_format[1] += digits;
    progress_format[4] += digits;

    ev3cxx::File file(file_name, "w+");
    file.close();

    int cnt = 0;
    for (auto test: tests) {
        samples = test.samples;
        uint32_t Operations_per_sample = test.operations_per_sample;
        test_t::function_type& function = test.function;

        display.resetScreen();
        display.format("Benchmark ");
        display.format(progress_format) % ++cnt % tests_len;
        display.format("\n % ...\n") % test.name;
        display.format(test_progress_format) % 0;
        display.format(" / % ") % samples;
        const int progress_line = display.getTextLine();
        display.format("\n\tops: % ") % Operations_per_sample;
        
        ev3cxx::StopWatch s;
        auto duration = s();
        auto time = run_time();

        results.clear();
        results.reserve(samples);
        times.clear();
        times.reserve(samples);

        test.init_function();

        for(uint32_t i = 0; i != samples; ++i) {
            display.setTextLine(6, 0, true);

            time = run_time();
            s.reset();
            for(uint32_t j = 0; j != Operations_per_sample; ++j) {
                function(j);
            }
            duration = s();
            results.push_back(duration);
            times.push_back(time);

            display.setTextLine(progress_line, 0, false);
            display.format(test_progress_format) % (i + 1);

            if(skip_btn.isPressed()) {
                skip_btn.waitForRelease();
                samples = i;
                break;
            }
        }

        test.clean_function();

        file.open(file_name, "a");

        ev3cxx::format(file, "%  = {") % make_valid_identifier(test.name);

        add_item(file, "name", test.name, 1, true);

        add_item(file, "samples", samples);
        add_item(file, "operations_per_sample", Operations_per_sample);
        
        add_list(file, "time", times.begin(), times.end());
        add_list(file, "duration", results.begin(), results.end());

        if (Operations_per_sample == 0) {
            display.resetScreen();
            display.format("DIV BY ZERO");
            skip_btn.waitForClick();
            Operations_per_sample = 1;
        }

        auto average = (avg<Time_t>(results.begin(), results.end()) * 1000) / Operations_per_sample;
        auto meanv = (*mean(results.begin(), results.end()) * 1000) / Operations_per_sample;
        auto min = (*results.begin() * 1000) / Operations_per_sample;
        auto max = (*(results.end() - 1) * 1000) / Operations_per_sample;

        add_item(file, "t_min", min);
        add_item(file, "t_max", max);
        add_item(file, "t_avg", average);
        add_item(file, "t_mean", meanv);

        add_item(file, "unit", "ns");
        add_item(file, "raw_unit", "us");

        ev3cxx::format(file, "\n    }\n\n");

        file.close();

        display.setTextLine(3, 0, false);
        display.format("\nmin:  %7 ns") % min;
        display.format("\nmax:  %7 ns") % max;
        display.format("\navg:  %7 ns") % average;
        display.format("\nmean: %7 ns") % meanv;

        auto remaining = ev3cxx::wait(2000, [&](){ return next_btn.isPressed(); }, 0);
        next_btn.waitForRelease();
        if (remaining != 0) {
            display.setTextLine(2, display.width * 0.1, true);
            display.format("PAUSED");
            next_btn.waitForClick();
        }
    }

    file.open(file_name, "a");

    ev3cxx::format(file, "\n% \n") % to_py_list("tests", tests, tests + tests_len);

    file.close();
    display.resetScreen();
    display.format("Benchmark\nfinished.");
}
