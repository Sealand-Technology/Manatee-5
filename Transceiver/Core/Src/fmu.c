#include "fmu.h"

mavlink_heartbeat_t heartbeat_msg;
mavlink_command_long_t cmd_long_msg;
mavlink_manual_control_t manual_control_msg;

void stm32_Mavlink_MsgRxCpltCallback(mavlink_message_t *msg)
{
	// printf("stm32_Mavlink_MsgRxCpltCallback Start\r\n");
  switch (msg->msgid)
  {
    case MAVLINK_MSG_ID_HEARTBEAT:
      // printf("Got one heartbeat frame\r\n");
      mavlink_msg_heartbeat_decode(msg, &heartbeat_msg);
      // FMU_Receive_Heartbeat_Msg();
      break;

    case MAVLINK_MSG_ID_GPS_RAW_INT:
      // mavlink_msg_gps_raw_int_decode(&msg, &gps_raw_int);
      // gnss_fix_type = gps_raw_int.fix_type;
      // gnss_sat_num = gps_raw_int.satellites_visible;
      // gnss_hdop = gps_raw_int.eph;
      break;

    case MAVLINK_MSG_ID_ATTITUDE:
      // mavlink_msg_attitude_decode(&msg, &attitude);
      // roll_deg = degrees(attitude.roll);
      // pitch_deg = degrees(attitude.pitch);
      // yaw_deg = degrees(attitude.yaw);
      break;

    case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
      // mavlink_msg_global_position_int_decode(&msg, &global_position_int);
      // latitude = global_position_int.lat;
      // longitude = global_position_int.lon;
      // alt_msl_mm = global_position_int.alt;
      // alt_above_home_mm = global_position_int.relative_alt;
      // speed_north_cm_s = global_position_int.vx;
      // speed_east_cm_s = global_position_int.vy;
      // speed_down_cm_s = global_position_int.vz;
      break;
    default:
      break;
  }
}

void FMU_MAVLink_Init(UART_HandleTypeDef *huart, mavlink_channel_t chan)
{
  stm32_Mavlink_BindChannel(huart, chan);
  stm32_Mavlink_StartReceiveIT(chan);
}

// Reboot and Arm cmd need to be sent 2 times!...
void FMU_Send_Reboot_Msg(mavlink_channel_t chan)
{
  cmd_long_msg.target_system = 1;
  cmd_long_msg.target_component = 1;
  cmd_long_msg.command = MAV_CMD_PREFLIGHT_REBOOT_SHUTDOWN;
  cmd_long_msg.confirmation = 0;
  cmd_long_msg.param1 = 1.0f;
  cmd_long_msg.param2 = 0.0f;
  cmd_long_msg.param3 = 0.0f;
  cmd_long_msg.param4 = 0.0f;

  mavlink_msg_command_long_send_struct(chan, &cmd_long_msg);
}

void FMU_Send_Arm_Msg(mavlink_channel_t chan)
{
  cmd_long_msg.target_system = 1;
  cmd_long_msg.target_component = 1;
  cmd_long_msg.command = MAV_CMD_COMPONENT_ARM_DISARM;
  cmd_long_msg.confirmation = 0;
  cmd_long_msg.param1 = (float)true;
  cmd_long_msg.param2 = 0;
  // 0: arm-disarm unless prevented by safety checks (i.e. when landed)
  // 21196: force arming/disarming (e.g. allow arming to override preflight checks and disarming in flight)

  mavlink_msg_command_long_send_struct(chan, &cmd_long_msg);
}

/**
 * @brief  Send the manual control message based on joystick input.
 * @param  chan: MAVLink channel to use for transmission.
 * @param  js_axes_in: Pointer to the array of 8 joystick axis values.
 * @param  js_buttons_in: Pointer to the joystick buttons input.
 * @retval None
 */
void FMU_Send_Manual_Control_Msg(mavlink_channel_t chan, int16_t *js_axes_in, uint16_t *js_buttons_in)
{
  // Map joystick axis values using the macro
  manual_control_msg.x = map_value(js_axes_in[0] + js_axes_trim_offset[0], JS_AXES_IN_MIN, JS_AXES_IN_MAX, JS_AXES_OUT_MIN, JS_AXES_OUT_MAX);
  manual_control_msg.y = map_value(js_axes_in[1] + js_axes_trim_offset[1], JS_AXES_IN_MIN, JS_AXES_IN_MAX, JS_AXES_OUT_MIN, JS_AXES_OUT_MAX);
  manual_control_msg.z = map_value(js_axes_in[2] + js_axes_trim_offset[2], JS_AXES_IN_MIN, JS_AXES_IN_MAX, JS_AXES_OUT_MIN, JS_AXES_OUT_MAX) + 500;
  manual_control_msg.r = map_value(js_axes_in[3] + js_axes_trim_offset[3], JS_AXES_IN_MIN, JS_AXES_IN_MAX, JS_AXES_OUT_MIN, JS_AXES_OUT_MAX);
  manual_control_msg.rx = map_value(js_axes_in[4] + js_axes_trim_offset[4], JS_AXES_IN_MIN, JS_AXES_IN_MAX, JS_AXES_OUT_MIN, JS_AXES_OUT_MAX);
  manual_control_msg.ry = map_value(js_axes_in[5] + js_axes_trim_offset[5], JS_AXES_IN_MIN, JS_AXES_IN_MAX, JS_AXES_OUT_MIN, JS_AXES_OUT_MAX);
  manual_control_msg.rz = map_value(js_axes_in[6] + js_axes_trim_offset[6], JS_AXES_IN_MIN, JS_AXES_IN_MAX, JS_AXES_OUT_MIN, JS_AXES_OUT_MAX);
  manual_control_msg.rr = map_value(js_axes_in[7] + js_axes_trim_offset[7], JS_AXES_IN_MIN, JS_AXES_IN_MAX, JS_AXES_OUT_MIN, JS_AXES_OUT_MAX);
	
//  manual_control_msg.x = 0;
//  manual_control_msg.y = 0;
//  manual_control_msg.z = 1500;
//  manual_control_msg.r = 0;
//  manual_control_msg.rx = 0;
//  manual_control_msg.ry = 0;
//  manual_control_msg.rz = 0;
//  manual_control_msg.rr = 1000;
	
  // Assign joystick buttons input to manual control message
  manual_control_msg.buttons = *js_buttons_in;

  // Set the target ID (e.g., 1 for default)
  manual_control_msg.target = 1;

  mavlink_msg_manual_control_send_struct(chan, &manual_control_msg);
}

void FMU_Receive_Heartbeat_Msg(void)
{
  printf("mode=%d\r\n", heartbeat_msg.custom_mode);
  printf("type=%d\r\n", heartbeat_msg.type);
  printf("apm_type=%d\r\n", heartbeat_msg.autopilot);
  printf("base_mode=%d\r\n", heartbeat_msg.base_mode);
  printf("status=%d\r\n", heartbeat_msg.system_status);
  printf("mavlink_ver=%d\r\n", heartbeat_msg.mavlink_version);
}
