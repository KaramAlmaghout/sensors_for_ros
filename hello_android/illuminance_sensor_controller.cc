#include "imgui.h"

#include "illuminance_sensor_controller.h"

namespace android_ros {
IlluminanceSensorController::IlluminanceSensorController(
  IlluminanceSensor* sensor,
  Publisher<sensor_msgs::msg::Illuminance> publisher)
  : sensor_(sensor), publisher_(std::move(publisher))
{
  sensor->SetListener(
    std::bind(&IlluminanceSensorController::OnIlluminanceChanged, this, std::placeholders::_1));

  // TODO allow publisher to be enabled/disabled from GUI
  publisher_.Enable();

  // TODO allow GUI to change topic and QoS
}

void
IlluminanceSensorController::OnIlluminanceChanged(
    const sensor_msgs::msg::Illuminance& msg)
{
  last_msg_ = msg;
  LOGI("Publishing ROS message %lf lx", msg.illuminance);
  publisher_.Publish(msg);
}

void IlluminanceSensorController::DrawFrame() {
  bool show_dialog = true;
  ImGui::Begin("Illuminace Senosr", &show_dialog,
    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
  if (ImGui::Button("< Back")) {
    LOGI("Asked to go back");
    Emit(event::GuiNavigateBack{});
  }
  ImGui::Text("Illuminance Sensor");
  ImGui::Separator();
  ImGui::Text("Name: %s", sensor_->Descriptor().name);
  ImGui::Text("Vendor: %s", sensor_->Descriptor().vendor);
  ImGui::Separator();
  ImGui::Text("Last measurement: %.2f lx", last_msg_.illuminance);
  ImGui::End();
}
}  // namespace android_ros
