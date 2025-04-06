// Signal K application template file.
//
// This application demonstrates core SensESP concepts in a very
// concise manner. You can build and upload the application as is
// and observe the value changes on the serial port monitor.
//
// You can use this source file as a basis for your own projects.
// Remove the parts that are not relevant to you, and add your own code
// for external hardware libraries.

#include <memory>

#include "sensesp.h"
#include "sensesp/sensors/digital_input.h"
#include "sensesp/sensors/sensor.h"
#include "sensesp/signalk/signalk_output.h"
#include "sensesp/system/lambda_consumer.h"
#include "sensesp_app_builder.h"

#include "sensesp/signalk/signalk_value_listener.h"
#include <pwmWrite.h>

#define SEATHEATER_1_PIN D10
#define SEATHEATER_2_PIN D9
#define SEATHEATER_3_PIN D8
#define SEATHEATER_4_PIN D7

// Pwm initialization
Pwm pwm = Pwm();

using namespace sensesp;

// The setup function performs one-time application initialization.
void setup() {
  SetupLogging(ESP_LOG_DEBUG);

  // Construct the global SensESPApp() object
  SensESPAppBuilder builder;
  sensesp_app = (&builder)
                    // Set a custom hostname for the app.
                    ->set_hostname("SensESP-BB2")
                    // Optionally, hard-code the WiFi and Signal K server
                    // settings. This is normally not needed.
                    //->set_wifi_client("OpenPlotter", "12345678")
                    //->set_wifi_access_point("SensESP-BB2", "my_ap_password")
                    //->set_sk_server("192.168.10.3", 80)
                    //->enable_ota("SensESP-BB2")
                    ->enable_ip_address_sensor()
                    ->get_app();

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Seatheater
const uint16_t freq = 40000;
pwm.setFrequency(SEATHEATER_1_PIN, freq);
pwm.setFrequency(SEATHEATER_2_PIN, freq);
pwm.setFrequency(SEATHEATER_3_PIN, freq);
pwm.setFrequency(SEATHEATER_4_PIN, freq);

//Starting Bug
auto sh1 = new SKValueListener<float>("electrical.inside.salon.port.seatheater.1.value", CHANGE, "Seatheater 1");
auto* sh1_consumer = new LambdaConsumer<float>([](float input) {
    pwm.write(SEATHEATER_1_PIN, input);
    debugI("SeatHeater 1: %f", input);
});
sh1->connect_to(sh1_consumer);

auto sh2 = new SKValueListener<float>("electrical.inside.salon.port.seatheater.2.value", CHANGE, "Seatheater 2");
auto* sh2_consumer = new LambdaConsumer<float>([](float input) {
    pwm.write(SEATHEATER_2_PIN, input);
    debugI("SeatHeater 2: %f", input);
});
sh2->connect_to(sh2_consumer);

auto sh3 = new SKValueListener<float>("electrical.inside.salon.port.seatheater.3.value", CHANGE, "Seatheater 3");
auto* sh3_consumer = new LambdaConsumer<float>([](float input) {
    pwm.write(SEATHEATER_3_PIN, input);
    debugI("SeatHeater 3: %f", input);
});
sh3->connect_to(sh3_consumer);

auto sh4 = new SKValueListener<float>("electrical.inside.salon.port.seatheater.4.value", CHANGE, "Seatheater 4");
auto* sh4_consumer = new LambdaConsumer<float>([](float input) {
    pwm.write(SEATHEATER_4_PIN, input);
    debugI("SeatHeater 4: %f", input);
});
sh4->connect_to(sh4_consumer);

  // To avoid garbage collecting all shared pointers created in setup(),
  // loop from here.
  while (true) {
    loop();
  }
}

void loop() { event_loop()->tick(); }
