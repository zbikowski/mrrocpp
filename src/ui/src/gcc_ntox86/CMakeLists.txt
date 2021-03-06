include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(ui
	../MAM_wnd_manual_moves_automatic_measure.cc
	../abmain.cc
	../fun.cc
	../fun_r_conveyor.cc
	../fun_r_irp6_mechatronika.cc
	../fun_r_irp6_on_track.cc
	../fun_r_irp6_postument.cc
	../fun_r_irp6ot_tfg.cc
	../fun_r_irp6p_tfg.cc
	../fun_r_sarkofag.cc
	../fun_r_bird_hand.cc
	../fun_r_polycrank.cc
	../fun_r_shead.cc
	../fun_r_smb.cc
	../fun_r_speaker.cc
	../fun_r_spkm.cc
	../timer.cc
	../ui.cc
	../ui_ecp_r_common.cc
	../ui_ecp_r_irp6_common.cc
	../speaker/ui_ecp_r_speaker.cc
	../ui_ecp_r_tfg_and_conv.cc
	../ui_ecp_r_data_port.cc
	../bird_hand/ui_ecp_r_bird_hand.cc
	../bird_hand/wnd_bird_hand_command_and_status.cc
	../bird_hand/wnd_bird_hand_configuration.cc
	../ui_robot.cc
	../bird_hand/ui_r_bird_hand.cc
	../irp6ot_m/ui_r_irp6ot_m.cc
	../irp6ot_tfg/ui_r_irp6ot_tfg.cc
	../irp6p_m/ui_r_irp6p_m.cc
	../irp6p_tfg/ui_r_irp6p_tfg.cc
	../sarkofag/ui_r_sarkofag.cc
	../irp6m_m/ui_r_irp6m_m.cc
	../polycrank/ui_r_polycrank.cc
	../conveyor/ui_r_conveyor.cc
	../speaker/ui_r_speaker.cc
	../spkm/ui_ecp_r_spkm.cc
	../spkm/ui_r_spkm.cc
	../spkm/wnd_spkm_inc.cc
	../spkm/wnd_spkm_int.cc
	../spkm/wnd_spkm_external.cc
	../smb/ui_ecp_r_smb.cc
	../smb/ui_r_smb.cc
	../shead/ui_ecp_r_shead.cc
	../shead/ui_r_shead.cc
	../ui_init.cc
	../ui_class.cc
	../ui_ecp.cc
	../wnd_base.cc
	../ui_sr.cc
	../wndFileLocationEvents.cc
	../wndForceControlEvents.cc
	../wndTrajectoryReproduceEvents.cc
)

find_library(PHEX_LIBRARY phexlib $ENV{QNX_TARGET}/x86/usr/lib)
find_library(AP_LIBRARY Ap $ENV{QNX_TARGET}/x86/usr/lib)
find_library(PH_LIBRARY ph $ENV{QNX_TARGET}/x86/usr/lib)
set(PHOTON_LIBRARIES ${PHEX_LIBRARY} ${AP_LIBRARY} ${PH_LIBRARY})

target_link_libraries(ui
	ecp_robots ecp_mp
	${PHOTON_LIBRARIES}
	${COMMON_LIBRARIES}
)

set(ABMOD Icon.wgti MAM_wnd_manual_moves_automatic_measures.wgtw base.wgtw
	file_selection_window.wgtw task_config_window.wgtw
	teaching_window.wgtw wndFileLocation.wgtw wndForceControl.wgtw
	wndTrajectoryReproduce.wgtw wnd_choose_option.wgtw
	wnd_conveyor_moves.wgtw wnd_conveyor_servo_algorithm.wgtw
	wnd_help_about.wgtw wnd_input_double.wgtw wnd_input_integer.wgtw
	wnd_irp6_mechatronika_inc.wgtw wnd_irp6_mechatronika_int.wgtw
	wnd_irp6_mechatronika_kinematic.wgtw
	wnd_irp6_mechatronika_servo_algorithm.wgtw
	wnd_irp6_mechatronika_xyz_angle_axis.wgtw
	wnd_irp6_mechatronika_xyz_angle_axis_ts.wgtw
	wnd_irp6_mechatronika_xyz_euler_zyz.wgtw
	wnd_irp6_mechatronika_xyz_euler_zyz_ts.wgtw wnd_irp6_on_track_inc.wgtw
	wnd_irp6_on_track_int.wgtw wnd_irp6_on_track_kinematic.wgtw
	wnd_irp6_on_track_servo_algorithm.wgtw
	wnd_irp6_on_track_xyz_angle_axis.wgtw
	wnd_irp6_on_track_xyz_angle_axis_relative.wgtw
	wnd_irp6_on_track_xyz_angle_axis_ts.wgtw
	wnd_irp6_on_track_xyz_euler_zyz.wgtw
	wnd_irp6_on_track_xyz_euler_zyz_ts.wgtw wnd_irp6_postument_inc.wgtw
	wnd_irp6_postument_int.wgtw wnd_irp6_postument_kinematic.wgtw
	wnd_irp6_postument_servo_algorithm.wgtw
	wnd_irp6_postument_xyz_angle_axis.wgtw
	wnd_irp6_postument_xyz_angle_axis_relative.wgtw
	wnd_irp6_postument_xyz_angle_axis_ts.wgtw
	wnd_irp6_postument_xyz_euler_zyz.wgtw
	wnd_irp6_postument_xyz_euler_zyz_ts.wgtw wnd_irp6ot_tfg_moves.wgtw
	wnd_irp6ot_tfg_servo_algorithm.wgtw wnd_irp6p_tfg_moves.wgtw
	wnd_irp6p_tfg_servo_algorithm.wgtw wnd_sarkofag_moves.wgtw
	wnd_sarkofag_servo_algorithm.wgtw wnd_message.wgtw
	wnd_bird_hand_command_and_status.wgtw wnd_bird_hand_configuration.wgtw
	wnd_spkm_inc.wgtw
	wnd_spkm_int.wgtw
	wnd_spkm_external.wgtw
	wnd_polycrank_int.wgtw
	wnd_processes_control.wgtw wnd_speaker_play.wgtw yes_no_window.wgtw)

ADD_CUSTOM_TARGET(ui_photon_application ALL
	COMMAND $ENV{QNX_HOST}/usr/bin/usemsg ui ${CMAKE_CURRENT_SOURCE_DIR}/../Usemsg
	COMMAND cmake -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../../wgt ${CMAKE_CURRENT_BINARY_DIR}/../../wgt
	COMMAND $ENV{QNX_HOST}/usr/bin/phabbind ui ${ABMOD} 
	DEPENDS ui ${CMAKE_CURRENT_SOURCE_DIR}/../Usemsg
)

install(TARGETS ui DESTINATION bin)
