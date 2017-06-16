#pragma once

#include "tic.hpp"

#include <QMainWindow>

class QCheckBox;
class QComboBox;
class QGridLayout;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QPushButton;
class QRadioButton;
class QScrollBar;
class QShortcut;
class QSpinBox;
class QVBoxLayout;

class main_controller;

class main_window : public QMainWindow
{
  Q_OBJECT

public:
  main_window(QWidget *parent = 0);
  
  /** Stores a pointer to the main_controller object so that we can report events. **/
  void set_controller(main_controller * controller);
  
  /** This causes the window to call the controller's update() function
   * periodically, on the same thread as everything else.
   *
   * interval_ms is the amount of time between updates, in milliseconds.
   */
  void start_update_timer(uint32_t interval_ms);
  
  void show_error_message(std::string const & message);
  void show_warning_message(std::string const & message);
  void show_info_message(std::string const & message);
  
  /** Show an OK/Cancel dialog, return true if the user selects OK. */
  bool confirm(std::string const & question);
  
  void set_device_list_contents(std::vector<tic::device> const & device_list);
  void set_device_list_selected(tic::device const & device);  
  
  /** Sets the label that shows the connection status/error. */
  void set_connection_status(std::string const & status, bool error);
  
  /** Controls whether the main controls of the application are enabled or
   * disabled. **/
  void set_main_boxes_enabled(bool enabled);
  
  void set_manual_target_box_enabled(bool enabled);

  /** Controls whether the apply settings action/button is enabled or
   * disabled. */
  void set_apply_settings_enabled(bool enabled);

  /** Controls whether the disconnect action is enabled or disabled. */
  void set_disconnect_enabled(bool enabled);

  /** Controls whether the reload settings from device action is enabled. */
  void set_reload_settings_enabled(bool enabled);

  /** Controls whether the restore defaults option is enabled. */
  void set_restore_defaults_enabled(bool enabled);
    
  void set_device_name(std::string const & name, bool link_enabled);
  void set_serial_number(std::string const & serial_number);
  void set_firmware_version(std::string const & firmware_version);
  
  void set_vin_voltage(std::string const & vin_voltage);
  void set_target_position(std::string const & target_position);
  void set_target_velocity(std::string const & target_velocity);
  void set_current_position(std::string const & current_position);
  void set_current_velocity(std::string const & current_velocity);
  
  void set_control_mode(uint8_t control_mode);
  void set_manual_target_range(int32_t target_min, int32_t target_max);
  void set_manual_target(int32_t target);
  
  void set_serial_baud_rate(uint32_t serial_baud_rate);
  void set_serial_device_number(uint8_t serial_device_number);
  void set_serial_crc_enabled(bool serial_crc_enabled);
  void set_input_min(uint32_t input_min);
  void set_input_neutral_min(uint32_t input_neutral_min);
  void set_input_neutral_max(uint32_t input_neutral_max);
  void set_input_max(uint32_t input_max);
  void set_output_min(int32_t output_min);
  void set_output_max(int32_t output_max);
  void set_input_averaging_enabled(bool input_averaging_enabled);
  void set_input_hysteresis(uint16_t input_hysteresis);
  void set_encoder_prescaler(uint32_t encoder_prescaler);
  void set_encoder_postscaler(uint32_t encoder_postscaler);
  void set_encoder_unlimited(bool encoder_unlimited);
  void set_speed_max(uint32_t speed_max);
  void set_speed_min(uint32_t speed_min);
  void set_accel_max(uint32_t accel_max);
  void set_decel_max(uint32_t decel_max);
  void set_step_mode(uint8_t step_mode);
  void set_current_limit(uint32_t current_limit);
  void set_decay_mode(uint8_t decay_mode);

private:
  /** Helper method for setting the index of a combo box, given the desired
   * uint8_t item value.  Defaults to using the first entry in the combo box if
   * the specified value is not found. */
  void set_u8_combo_box(QComboBox * combo, uint8_t value);

  void set_spin_box(QSpinBox * box, int value);
  
protected:
  /** This is called by Qt just before the window is shown for the first time,
   * and is also called whenever the window becomes unminimized. */
  void showEvent(QShowEvent *) override;
  
  /** This is called by Qt when the "close" slot is triggered, meaning that
   * the user wants to close the window. */
  void closeEvent(QCloseEvent *) override;
    
private slots:
  void on_disconnect_action_triggered();
  void on_reload_settings_action_triggered();
  void on_restore_defaults_action_triggered();
  void on_update_timer_timeout();
  void on_device_name_value_linkActivated();
  void on_documentation_action_triggered();
  void on_about_action_triggered();
  
  void on_device_list_value_currentIndexChanged(int index);

  void on_manual_target_position_mode_radio_toggled(bool checked);
  void on_manual_target_scroll_bar_valueChanged(int value);
  void on_manual_target_scroll_bar_sliderReleased();
  void on_manual_target_entry_value_valueChanged(int value);
  void on_manual_target_return_key_shortcut_activated();
  void on_set_target_button_clicked();
  void on_auto_set_target_checkbox_stateChanged(int state);
  
  /** This is called by Qt when the user wants to apply settings. */
  void on_apply_settings_action_triggered();
  
  void on_control_mode_value_currentIndexChanged(int index);
  void on_serial_baud_rate_value_valueChanged(int value);
  void on_serial_baud_rate_value_editingFinished();
  void on_serial_device_number_value_valueChanged(int value);
  void on_serial_crc_enabled_checkbox_stateChanged(int state);
  void on_input_min_value_valueChanged(int value);
  void on_input_neutral_min_value_valueChanged(int value);
  void on_input_neutral_max_value_valueChanged(int value);
  void on_input_max_value_valueChanged(int value);
  void on_output_min_value_valueChanged(int value);
  void on_output_max_value_valueChanged(int value);
  void on_input_averaging_enabled_checkbox_stateChanged(int state);
  void on_input_hysteresis_value_valueChanged(int value);
  void on_encoder_prescaler_value_valueChanged(int value);
  void on_encoder_postscaler_value_valueChanged(int value);
  void on_encoder_unlimited_checkbox_stateChanged(int state);
  void on_speed_max_value_valueChanged(int value);
  void on_speed_min_value_valueChanged(int value);
  void on_accel_max_value_valueChanged(int value);
  void on_decel_max_value_valueChanged(int value);
  void on_step_mode_value_currentIndexChanged(int index);
  void on_current_limit_value_valueChanged(int value);
  void on_current_limit_value_editingFinished();
  void on_decay_mode_value_currentIndexChanged(int index);
  
private:  
  bool start_event_reported = false;
  
  /* We set this to true temporarily when programmatically setting the value
   * of an input in order to suppress sending a spurious user-input event to
   * the rest of the program. */
  bool suppress_events = false;
  
  QTimer * update_timer;

  // These are low-level functions called in the constructor that set up the
  // GUI elements.
  void setup_window();
  void setup_menu_bar();
  QLayout * setup_header();
  QWidget * setup_main_boxes_widget();
  QLayout * setup_left_column();
  QLayout * setup_right_column();
  QWidget * setup_device_info_box();
  QWidget * setup_status_box();
  QWidget * setup_control_mode_widget();
  QWidget * setup_manual_target_box();
  QLayout * setup_manual_target_mode_layout();
  QWidget * setup_manual_target_entry_widget();
  QWidget * setup_serial_settings_box();
  QWidget * setup_scaling_settings_box();
  QWidget * setup_motor_settings_box();
  QLayout * setup_footer();
  QWidget * setup_apply_button();
  void retranslate();
    
  QIcon program_icon;
    
  QMenuBar * menu_bar;
  QMenu * file_menu;
  QAction * exit_action;
  QMenu * device_menu;
  QAction * disconnect_action;
  QAction * reload_settings_action;
  QAction * restore_defaults_action;
  QAction * apply_settings_action;
  QMenu * help_menu;
  QAction * documentation_action;
  QAction * about_action;
  
  QWidget * central_widget;
  QVBoxLayout * central_widget_layout;

  QHBoxLayout * header_layout;
  QLabel * device_list_label;
  QComboBox * device_list_value;
  QLabel * connection_status_value;
  
  QWidget * main_boxes_widget;
  QHBoxLayout * main_boxes_widget_layout;
  QVBoxLayout * left_column_layout;
  QVBoxLayout * right_column_layout;

  QGroupBox * device_info_box;
  QGridLayout * device_info_box_layout;
  QLabel * device_name_label;
  QLabel * device_name_value;
  QLabel * serial_number_label;
  QLabel * serial_number_value;
  QLabel * firmware_version_label;
  QLabel * firmware_version_value;
    
  QGroupBox * status_box;
  QGridLayout * status_box_layout;
  QLabel * vin_voltage_label;
  QLabel * vin_voltage_value;
  QLabel * target_label;
  QLabel * target_value;
  QLabel * current_position_label;
  QLabel * current_position_value;
  QLabel * current_velocity_label;
  QLabel * current_velocity_value;
  
  QGroupBox * manual_target_box;
  QVBoxLayout * manual_target_box_layout;
  QHBoxLayout * manual_target_mode_layout;
  QWidget * manual_target_entry_widget;
  QGridLayout * manual_target_entry_widget_layout;
  QRadioButton * manual_target_position_mode_radio;
  QRadioButton * manual_target_speed_mode_radio;
  QScrollBar * manual_target_scroll_bar;
  QLabel * manual_target_min_label;
  QLabel * manual_target_max_label;
  QSpinBox * manual_target_entry_value;
  QShortcut * manual_target_return_key_shortcut;
  QShortcut * manual_target_enter_key_shortcut;
  QPushButton * set_target_button;
  QCheckBox * auto_set_target_checkbox;
  QCheckBox * auto_zero_target_checkbox;
  
  // [all-settings]
  QWidget * control_mode_widget;
  QGridLayout * control_mode_widget_layout;
  QLabel * control_mode_label;
  QComboBox * control_mode_value;
  
  QGroupBox * serial_settings_box;
  QGridLayout * serial_settings_box_layout;
  QLabel * serial_baud_rate_label;
  QSpinBox * serial_baud_rate_value;
  QLabel * serial_device_number_label;
  QSpinBox * serial_device_number_value;
  QCheckBox * serial_crc_enabled_checkbox;
  
  QGroupBox * scaling_settings_box;
  QGridLayout * scaling_settings_box_layout;
  QLabel * scaling_input_label;
  QLabel * scaling_target_label;
  QLabel * scaling_min_label;
  QLabel * scaling_neutral_min_label;
  QLabel * scaling_neutral_max_label;
  QLabel * scaling_max_label;
  QSpinBox * input_min_value;
  QSpinBox * input_neutral_min_value;
  QSpinBox * input_neutral_max_value;
  QSpinBox * input_max_value;
  QSpinBox * output_min_value;
  QSpinBox * output_max_value;
  QCheckBox * input_averaging_enabled_checkbox;
  QLabel * input_hysteresis_label;
  QSpinBox * input_hysteresis_value;
  QLabel * encoder_prescaler_label;
  QSpinBox * encoder_prescaler_value;
  QLabel * encoder_postscaler_label;
  QSpinBox * encoder_postscaler_value;
  QCheckBox * encoder_unlimited_checkbox;
  
  QGroupBox * motor_settings_box;
  QGridLayout * motor_settings_box_layout;
  QLabel * speed_max_label;
  QSpinBox * speed_max_value;
  QLabel * speed_max_value_pretty;
  QLabel * speed_min_label;
  QSpinBox * speed_min_value;
  QLabel * speed_min_value_pretty;
  QLabel * accel_max_label;
  QSpinBox * accel_max_value;
  QLabel * accel_max_value_pretty;
  QLabel * decel_max_label;
  QSpinBox * decel_max_value;
  QLabel * decel_max_value_pretty;
  QLabel * step_mode_label;
  QComboBox * step_mode_value;
  QLabel * current_limit_label;
  QSpinBox * current_limit_value;
  QLabel * decay_mode_label;
  QComboBox * decay_mode_value;
  
  QHBoxLayout * footer_layout;
  QPushButton * apply_settings_button;
  
  main_controller * controller;
};