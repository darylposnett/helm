/* Copyright 2013-2015 Matt Tytel
 *
 * twytch is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * twytch is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with twytch.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "synthesis_interface.h"

#include <iomanip>
#include "modulation_look_and_feel.h"
#include "synth_gui_interface.h"
#include "text_look_and_feel.h"

SynthesisInterface::SynthesisInterface(mopo::control_map controls) : SynthSection("synthesis") {
  addSubSection(amplitude_envelope_section_ = new EnvelopeSection("AMPLITUDE ENVELOPE", "amp"));
  addSubSection(delay_section_ = new DelaySection("DELAY"));
  addSubSection(feedback_section_ = new FeedbackSection("FEEDBACK"));
  addSubSection(filter_envelope_section_ = new EnvelopeSection("FILTER ENVELOPE", "fil"));
  addSubSection(filter_section_ = new FilterSection("FILTER"));
  addSubSection(formant_section_ = new FormantSection("FORMANT"));
  addSubSection(mono_lfo_1_section_ = new LfoSection("MONO LFO 1", "mono_lfo_1"));
  addSubSection(mono_lfo_2_section_ = new LfoSection("MONO LFO 2", "mono_lfo_2"));
  addSubSection(oscillator_section_ = new OscillatorSection("OSCILLATORS"));
  addSubSection(poly_lfo_section_ = new LfoSection("POLY LFO", "poly_lfo"));
  addSubSection(reverb_section_ = new ReverbSection("REVERB"));
  addSubSection(step_sequencer_section_ = new StepSequencerSection("STEP SEQUENCER"));

  addSlider(polyphony_ = new SynthSlider("polyphony"));
  polyphony_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addSlider(portamento_ = new SynthSlider("portamento"));
  portamento_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addSlider(pitch_bend_range_ = new SynthSlider("pitch_bend_range"));
  pitch_bend_range_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addSlider(portamento_type_ = new SynthSlider("portamento_type"));
  portamento_type_->setSliderStyle(Slider::LinearBar);

  addSlider(volume_ = new SynthSlider("volume"));
  volume_->setSliderStyle(Slider::LinearBar);

  addSlider(velocity_track_ = new SynthSlider("velocity_track"));
  velocity_track_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addSlider(stutter_frequency_ = new SynthSlider("stutter_frequency"));
  stutter_frequency_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addButton(stutter_on_ = new ToggleButton("stutter_on"));
  stutter_on_->setButtonText(String::empty);
  stutter_on_->addListener(this);
  stutter_on_->setToggleState(true, dontSendNotification);
  stutter_on_->setColour(ToggleButton::textColourId, Colour(0xffbbbbbb));

  addSlider(stutter_resample_frequency_ = new SynthSlider("stutter_resample_frequency"));
  stutter_resample_frequency_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addModulationButton(aftertouch_mod_ = new ModulationButton("aftertouch"));
  aftertouch_mod_->setButtonText(String::empty);
  aftertouch_mod_->addListener(this);

  addModulationButton(note_mod_ = new ModulationButton("note"));
  note_mod_->setButtonText(String::empty);
  note_mod_->addListener(this);

  addModulationButton(velocity_mod_ = new ModulationButton("velocity"));
  velocity_mod_->setButtonText(String::empty);
  velocity_mod_->addListener(this);

  addModulationButton(mod_wheel_mod_ = new ModulationButton("mod_wheel"));
  mod_wheel_mod_->setButtonText(String::empty);
  mod_wheel_mod_->addListener(this);

  addModulationButton(pitch_wheel_mod_ = new ModulationButton("pitch_wheel"));
  pitch_wheel_mod_->setButtonText(String::empty);
  pitch_wheel_mod_->addListener(this);

  addButton(legato_ = new ToggleButton("legato"));
  legato_->setButtonText(String::empty);
  legato_->addListener(this);
  legato_->setColour(ToggleButton::textColourId, Colour(0xffbbbbbb));

  setSliderUnits();
  setStyles();

  setDefaultDoubleClickValues();
  setAllValues(controls);
  setOpaque(true);
}

SynthesisInterface::~SynthesisInterface() {
  amplitude_envelope_section_ = nullptr;
  delay_section_ = nullptr;
  feedback_section_ = nullptr;
  filter_envelope_section_ = nullptr;
  filter_section_ = nullptr;
  formant_section_ = nullptr;
  mono_lfo_1_section_ = nullptr;
  mono_lfo_2_section_ = nullptr;
  oscillator_section_ = nullptr;
  poly_lfo_section_ = nullptr;
  reverb_section_ = nullptr;
  step_sequencer_section_ = nullptr;

  polyphony_ = nullptr;
  portamento_ = nullptr;
  pitch_bend_range_ = nullptr;
  portamento_type_ = nullptr;
  volume_ = nullptr;
  velocity_track_ = nullptr;
  stutter_frequency_ = nullptr;
  stutter_on_ = nullptr;
  stutter_resample_frequency_ = nullptr;
  aftertouch_mod_ = nullptr;
  note_mod_ = nullptr;
  velocity_mod_ = nullptr;
  mod_wheel_mod_ = nullptr;
  pitch_wheel_mod_ = nullptr;
  legato_ = nullptr;
}

void SynthesisInterface::paint(Graphics& g) {
  static const DropShadow section_shadow(Colour(0xcc000000), 3, Point<int>(0, 1));
  static const DropShadow component_shadow(Colour(0xcc000000), 5, Point<int>(0, 1));
  static Font roboto_reg(Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf,
                                                           BinaryData::RobotoRegular_ttfSize));
  static Font roboto_light(Typeface::createSystemTypefaceFor(BinaryData::RobotoLight_ttf,
                                                             BinaryData::RobotoLight_ttfSize));

  g.setColour(Colour(0xff212121));
  g.fillRect(getLocalBounds());

  section_shadow.drawForRectangle(g, amplitude_envelope_section_->getBounds());
  section_shadow.drawForRectangle(g, delay_section_->getBounds());
  section_shadow.drawForRectangle(g, feedback_section_->getBounds());
  section_shadow.drawForRectangle(g, filter_envelope_section_->getBounds());
  section_shadow.drawForRectangle(g, filter_section_->getBounds());
  section_shadow.drawForRectangle(g, formant_section_->getBounds());
  section_shadow.drawForRectangle(g, mono_lfo_1_section_->getBounds());
  section_shadow.drawForRectangle(g, mono_lfo_2_section_->getBounds());
  section_shadow.drawForRectangle(g, oscillator_section_->getBounds());
  section_shadow.drawForRectangle(g, poly_lfo_section_->getBounds());
  section_shadow.drawForRectangle(g, reverb_section_->getBounds());
  section_shadow.drawForRectangle(g, step_sequencer_section_->getBounds());

  section_shadow.drawForRectangle(g, Rectangle<int>(8, 630 - (44 / 2), 722, 44));
  section_shadow.drawForRectangle(g, Rectangle<int>(270 - (116 / 2), 482, 116, 118));
  section_shadow.drawForRectangle(g, Rectangle<int>(604, 416, 126, 58));
  section_shadow.drawForRectangle(g, Rectangle<int>(604, 4, 126, 220));

  g.setColour(Colour(0xff303030));
  g.fillRoundedRectangle(604.0f, 4.0f, 126.0f, 220.0f, 3.000f);

  g.setGradientFill(ColourGradient(Colour(0x00000000),
                                   static_cast<float>(proportionOfWidth(0.0000f)), 22.0f,
                                   Colour(0x77000000),
                                   static_cast<float>(proportionOfWidth(0.0000f)), 26.0f,
                                   false));
  g.fillRect(604, 4, 126, 20);

  g.setColour(Colour(0xff303030));
  g.fillRoundedRectangle(8.0f, static_cast<float>(630 - (44 / 2)), 722.0f, 44.0f, 3.000f);

  g.setColour(Colour(0xff303030));
  g.fillRoundedRectangle(604.0f, 416.0f, 126.0f, 58.0f, 3.000f);

  g.setColour(Colour(0xff303030));
  g.fillRoundedRectangle(static_cast<float>(270 - (116 / 2)), 482.0f, 116.0f, 118.0f, 3.000f);

  g.setColour(Colour(0xff999999));
  g.setFont(roboto_reg.withPointHeight(13.40f).withExtraKerningFactor(0.05f));
  g.drawText(TRANS("VOLUME"),
             667 - (60 / 2), 416, 60, 20,
             Justification::centred, true);

  g.setColour(Colour(0xff999999));
  g.setFont(roboto_reg.withPointHeight(13.40f).withExtraKerningFactor(0.05f));
  g.drawText(TRANS("ARTICULATION"),
             667 - (126 / 2), 4, 126, 20,
             Justification::centred, true);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  g.drawText(TRANS("POLYPHONY"),
             634 - (60 / 2), 86, 60, 10,
             Justification::centred, true);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  g.drawText(TRANS("PORTA"),
             634 - (46 / 2), 154, 46, 10,
             Justification::centred, true);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  g.drawText(TRANS("LEGATO"),
             700 - (60 / 2), 204, 60, 10,
             Justification::centred, true);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  g.drawText(TRANS("PORT TYPE"),
             634 - (60 / 2), 204, 60, 10,
             Justification::centred, true);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  g.drawText(TRANS("PITCH BEND"),
             700 - (60 / 2), 86, 60, 10,
             Justification::centred, true);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  g.drawText(TRANS("PITCH WHEEL"),
             62, 630 - (12 / 2), 80, 12,
             Justification::centredLeft, true);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  g.drawText(TRANS("MOD WHEEL"),
             215, 630 - (12 / 2), 60, 12,
             Justification::centredLeft, true);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  g.drawText(TRANS("NOTE"),
             388, 630 - (12 / 2), 40, 12,
             Justification::centredLeft, true);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  g.drawText(TRANS("VELOCITY"),
             522, 630 - (12 / 2), 60, 12,
             Justification::centredLeft, true);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  g.drawText(TRANS("AFTERTOUCH"),
             656, 630 - (12 / 2), 70, 12,
             Justification::centredLeft, true);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  g.drawText(TRANS("VEL TRACK"),
             700 - (60 / 2), 154, 60, 10,
             Justification::centred, true);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  g.drawText(TRANS("FREQ"),
             241 - (40 / 2), 570, 40, 10,
             Justification::centred, true);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  g.drawText(TRANS("RESAMPLE"),
             297 - (50 / 2), 570, 50, 10,
             Justification::centred, true);

  g.setColour(Colour(0xff999999));
  g.setFont(roboto_reg.withPointHeight(13.40f).withExtraKerningFactor(0.05f));
  g.drawText(TRANS("STUTTER"),
             270 - (84 / 2), 482, 84, 20,
             Justification::centred, true);

  g.setGradientFill(ColourGradient(Colour(0x00000000),
                                   static_cast<float>(proportionOfWidth(0.0000f)), 500.0f,
                                   Colour(0x77000000),
                                   static_cast<float>(proportionOfWidth(0.0000f)), 504.0f,
                                   false));
  g.fillRect(212, 482, 116, 20);

  g.setGradientFill(ColourGradient(Colour(0x00000000),
                                   static_cast<float>(proportionOfWidth(0.0000f)), 434.0f,
                                   Colour(0x77000000),
                                   static_cast<float>(proportionOfWidth(0.0000f)), 438.0f,
                                   false));
  g.fillRect(604, 416, 126, 20);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(8.0f));
  g.drawText(TRANS("OFF"),
             614 - (28 / 2), 172, 28, 12,
             Justification::centred, true);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(8.0f));
  g.drawText(TRANS("AUTO"),
             634 - (28 / 2), 172, 28, 12,
             Justification::centred, true);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(8.0f));
  g.drawText(TRANS("ON"),
             654 - (28 / 2), 172, 28, 12,
             Justification::centred, true);

  for (auto slider : slider_lookup_)
    slider.second->drawShadow(g);
}

void SynthesisInterface::resized() {
  amplitude_envelope_section_->setBounds(336.0f, 160.0f, 260.0f, 148.0f);
  delay_section_->setBounds(604.0f, 232.0f, 126.0f, 84.0f);
  feedback_section_->setBounds(8.0f, 216.0f, 320.0f, 92.0f);
  filter_envelope_section_->setBounds(336.0f, 4.0f, 260.0f, 148.0f);
  filter_section_->setBounds(8.0f, 316.0f, 320.0f, 158.0f);
  formant_section_->setBounds(8.0f, 482.0f, 196.0f, 118.0f);
  mono_lfo_1_section_->setBounds(336.0f, 482.0f, 126.0f, 118.0f);
  mono_lfo_2_section_->setBounds(470.0f, 482.0f, 126.0f, 118.0f);
  oscillator_section_->setBounds(8.0f, 4.0f, 320.0f, 204.0f);
  poly_lfo_section_->setBounds(604.0f, 482.0f, 126.0f, 118.0f);
  reverb_section_->setBounds(604.0f, 324.0f, 126.0f, 84.0f);
  step_sequencer_section_->setBounds(336.0f, 316.0f, 260.0f, 158.0f);

  polyphony_->setBounds(634 - (40 / 2), 42, 40, 40);
  portamento_->setBounds(634 - (40 / 2), 110, 40, 40);
  pitch_bend_range_->setBounds(700 - (40 / 2), 42, 40, 40);
  portamento_type_->setBounds(634 - (45 / 2), 186, 45, 16);
  volume_->setBounds(604, 436, 126, 38);
  velocity_track_->setBounds(700 - (40 / 2), 110, 40, 40);
  stutter_frequency_->setBounds(241 - (40 / 2), 524, 40, 40);
  stutter_on_->setBounds(214, 482, 20, 20);
  stutter_resample_frequency_->setBounds(297 - (40 / 2), 524, 40, 40);
  aftertouch_mod_->setBounds(614, 614, 32, 32);
  note_mod_->setBounds(346, 614, 32, 32);
  velocity_mod_->setBounds(480, 614, 32, 32);
  mod_wheel_mod_->setBounds(173, 614, 32, 32);
  pitch_wheel_mod_->setBounds(20, 614, 32, 32);
  legato_->setBounds(684, 186, 32, 16);
}

void SynthesisInterface::sliderValueChanged(Slider* sliderThatWasMoved) {
  std::string name = sliderThatWasMoved->getName().toStdString();
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  parent->valueChanged(name, sliderThatWasMoved->getValue());
}

void SynthesisInterface::buttonClicked(Button* buttonThatWasClicked) {
  std::string name = buttonThatWasClicked->getName().toStdString();
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  if (parent == nullptr)
    return;

  if (buttonThatWasClicked == stutter_on_)
    parent->valueChanged(name, buttonThatWasClicked->getToggleState() ? 1.0 : 0.0);
  else if (buttonThatWasClicked == legato_)
    parent->valueChanged(name, buttonThatWasClicked->getToggleState() ? 1.0 : 0.0);
}

void SynthesisInterface::setSliderUnits() {
  portamento_->setUnits("secs/semitone");
  portamento_->setScalingType(mopo::ValueDetails::kExponential);
  pitch_bend_range_->setUnits("semitones");

  polyphony_->setUnits("voices");

  velocity_track_->setUnits("%");
  velocity_track_->setPostMultiply(100.0);
  portamento_type_->setStringLookup(mopo::strings::off_auto_on);
}

void SynthesisInterface::setDefaultDoubleClickValues() {
  portamento_->setDoubleClickReturnValue(true, -7.0f);
  pitch_bend_range_->setDoubleClickReturnValue(true, 2.0f);

  velocity_track_->setDoubleClickReturnValue(true, 0.0f);
}

void SynthesisInterface::setStyles() {
  legato_->setLookAndFeel(TextLookAndFeel::instance());

  aftertouch_mod_->setLookAndFeel(ModulationLookAndFeel::instance());
  note_mod_->setLookAndFeel(ModulationLookAndFeel::instance());
  velocity_mod_->setLookAndFeel(ModulationLookAndFeel::instance());
  mod_wheel_mod_->setLookAndFeel(ModulationLookAndFeel::instance());
  pitch_wheel_mod_->setLookAndFeel(ModulationLookAndFeel::instance());
}

void SynthesisInterface::setValue(std::string name, mopo::mopo_float value,
                                  NotificationType notification) {
  if (slider_lookup_.count(name))
    slider_lookup_[name]->setValue(value, notification);
}