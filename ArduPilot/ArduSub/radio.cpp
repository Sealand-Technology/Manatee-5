#include "Sub.h"

void Sub::init_rc_in()
{
    channel_pitch    = RC_Channels::rc_channel(0);
    channel_roll     = RC_Channels::rc_channel(1);
    channel_throttle = RC_Channels::rc_channel(2);
    channel_yaw      = RC_Channels::rc_channel(3);
    channel_lateral  = RC_Channels::rc_channel(4);
    channel_forward  = RC_Channels::rc_channel(5);
    channel_walkLateral = RC_Channels::rc_channel(6);
    channel_walkForward = RC_Channels::rc_channel(7);

    // set rc channel ranges
    channel_roll->set_angle(ROLL_PITCH_INPUT_MAX);
    channel_pitch->set_angle(ROLL_PITCH_INPUT_MAX);
    channel_yaw->set_angle(ROLL_PITCH_INPUT_MAX);
    channel_throttle->set_range(1000);
    channel_forward->set_angle(ROLL_PITCH_INPUT_MAX);
    channel_lateral->set_angle(ROLL_PITCH_INPUT_MAX);
    channel_walkLateral->set_angle(ROLL_PITCH_INPUT_MAX);
    channel_walkForward->set_angle(ROLL_PITCH_INPUT_MAX);

    // set default dead zones
    channel_roll->set_default_dead_zone(30);
    channel_pitch->set_default_dead_zone(30);
    channel_throttle->set_default_dead_zone(30);
    channel_yaw->set_default_dead_zone(40);
    channel_forward->set_default_dead_zone(30);
    channel_lateral->set_default_dead_zone(30);
    channel_walkLateral->set_default_dead_zone(30);
    channel_walkForward->set_default_dead_zone(30);

#if CONFIG_HAL_BOARD != HAL_BOARD_SITL
    // initialize rc input to 1500 on control channels (rather than 0)
    for (int i = 0; i < 8; i++) {
        RC_Channels::set_override(i, 1500);
    }

    RC_Channels::set_override(9, 1500); // camera pan channel
    RC_Channels::set_override(10, 1500); // camera tilt channel

    RC_Channel* chan = RC_Channels::rc_channel(11);
    uint16_t min = chan->get_radio_min();
    RC_Channels::set_override(11, min); // lights 1 channel

    chan = RC_Channels::rc_channel(12);
    min = chan->get_radio_min();
    RC_Channels::set_override(12, min); // lights 2 channel

    RC_Channels::set_override(13, 1100); // video switch
#endif
}

// init_rc_out -- initialise motors and check if pilot wants to perform ESC calibration
void Sub::init_rc_out()
{
    motors.set_update_rate(g.rc_speed);
    motors.set_loop_rate(scheduler.get_loop_rate_hz());
    motors.init((AP_Motors::motor_frame_class)g.frame_configuration.get(), AP_Motors::motor_frame_type::MOTOR_FRAME_TYPE_PLUS);
    motors.set_throttle_range(channel_throttle->get_radio_min(), channel_throttle->get_radio_max());

    // enable output to motors
    if (arming.rc_calibration_checks(true)) {
        enable_motor_output();
    }

    // refresh auxiliary channel to function map
    SRV_Channels::update_aux_servo_function();
}
