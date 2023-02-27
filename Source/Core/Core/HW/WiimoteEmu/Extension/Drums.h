// Copyright 2010 Dolphin Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "Core/HW/WiimoteEmu/Extension/Extension.h"

namespace ControllerEmu
{
class AnalogStick;
class Buttons;
class ControlGroup;
}  // namespace ControllerEmu

namespace WiimoteEmu
{
enum class DrumsGroup
{
  Buttons,
  Pads,
  Stick
};

// The Drums use the "1st-party" extension encryption scheme.
class Drums : public Extension1stParty
{
public:
  struct DesiredState
  {
    u8 stick_x;    // 6 bits
    u8 stick_y;    // 6 bits
    u8 buttons;    // 2 bits
    u8 drum_pads;  // 6 bits
    u8 softness;   // 3 bits
  };

  struct DataFormat
  {
    u8 sx : 6;
    u8 pad1 : 2;  // always 0

    u8 sy : 6;
    u8 pad2 : 2;  // always 0

    u8 pad3 : 1;  // unknown
    u8 which : 5;
    u8 none : 1;
    u8 hhp : 1;

    u8 pad4 : 1;      // unknown
    u8 velocity : 4;  // unknown
    u8 softness : 3;

    u16 bt;  // buttons
  };
  static_assert(sizeof(DataFormat) == 6, "Wrong size");

  Drums();

  void BuildDesiredExtensionState(DesiredExtensionState* target_state) override;
  void Update(const DesiredExtensionState& target_state) override;
  void Reset() override;

  ControllerEmu::ControlGroup* GetGroup(DrumsGroup group);

  enum
  {
    BUTTON_PLUS = 0x04,
    BUTTON_MINUS = 0x10,

    PAD_BASS = 0x0400,
    PAD_BLUE = 0x0800,
    PAD_GREEN = 0x1000,
    PAD_YELLOW = 0x2000,
    PAD_RED = 0x4000,
    PAD_ORANGE = 0x8000,
  };

  static const u8 STICK_CENTER = 0x20;
  static const u8 STICK_RADIUS = 0x1f;

  // TODO: Test real hardware. Is this accurate?
  static const u8 STICK_GATE_RADIUS = 0x16;

private:
  ControllerEmu::Buttons* m_buttons;
  ControllerEmu::Buttons* m_pads;
  ControllerEmu::AnalogStick* m_stick;

  ControllerEmu::SettingValue<double> m_hit_strength_setting;

  // Holds previous user input state to watch for "new" hits.
  u8 m_prev_pad_input = 0;
  // Holds new drum pad hits that still need velocity data to be sent.
  u8 m_new_pad_hits = 0;
  // Holds how many more frames to send each drum-pad bit.
  std::array<u8, 6> m_pad_remaining_frames{};
};
}  // namespace WiimoteEmu
