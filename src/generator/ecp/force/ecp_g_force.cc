/*!
 * @file
 * @brief File contains force generators definition
 * @author twiniars <twiniars@ia.pw.edu.pl>, Warsaw University of Technology
 *
 * @ingroup generators
 */

#include <cstdio>
#include <fstream>
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <cmath>
#include <iostream>

#include "base/lib/typedefs.h"

#include "base/lib/sr/srlib.h"
#include "base/ecp/ecp_robot.h"
#include "generator/ecp/force/ecp_g_force.h"

namespace mrrocpp {
namespace ecp {
namespace common {
namespace generator {

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // ///////////////////
//
// 			bias_edp_force_generator
//
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // ///////////////////


bias_edp_force::bias_edp_force(common::task::task& _ecp_task) :
	generator(_ecp_task)
{
}

bool bias_edp_force::first_step()
{
	the_robot->ecp_command.instruction.instruction_type = lib::SET;
	the_robot->ecp_command.instruction.set_type = ROBOT_MODEL_DEFINITION;
	the_robot->ecp_command.instruction.set_robot_model_type = lib::FORCE_BIAS;

	return true;
}

// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
bool bias_edp_force::next_step()
{
	return false;
}

tff_nose_run::tff_nose_run(common::task::task& _ecp_task, int step) :
	generator(_ecp_task), step_no(step)
{
	// domyslnie wszytkie osie podatne a pulse_check nieaktywne
	configure_behaviour(lib::CONTACT, lib::CONTACT, lib::CONTACT, lib::CONTACT, lib::CONTACT, lib::CONTACT);
	configure_pulse_check(false);
	configure_velocity(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	configure_force(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	configure_reciprocal_damping(lib::FORCE_RECIPROCAL_DAMPING, lib::FORCE_RECIPROCAL_DAMPING, lib::FORCE_RECIPROCAL_DAMPING, lib::TORQUE_RECIPROCAL_DAMPING, lib::TORQUE_RECIPROCAL_DAMPING, lib::TORQUE_RECIPROCAL_DAMPING);
	configure_inertia(lib::FORCE_INERTIA, lib::FORCE_INERTIA, lib::FORCE_INERTIA, lib::TORQUE_INERTIA, lib::TORQUE_INERTIA, lib::TORQUE_INERTIA);

	set_force_meassure(false);
}

void tff_nose_run::set_force_meassure(bool fm)
{
	force_meassure = fm;
}

void tff_nose_run::configure_pulse_check(bool pulse_check_activated_l)
{
	pulse_check_activated = pulse_check_activated_l;
}

void tff_nose_run::configure_behaviour(lib::BEHAVIOUR_SPECIFICATION x, lib::BEHAVIOUR_SPECIFICATION y, lib::BEHAVIOUR_SPECIFICATION z, lib::BEHAVIOUR_SPECIFICATION ax, lib::BEHAVIOUR_SPECIFICATION ay, lib::BEHAVIOUR_SPECIFICATION az)
{
	generator_edp_data.next_behaviour[0] = x;
	generator_edp_data.next_behaviour[1] = y;
	generator_edp_data.next_behaviour[2] = z;
	generator_edp_data.next_behaviour[3] = ax;
	generator_edp_data.next_behaviour[4] = ay;
	generator_edp_data.next_behaviour[5] = az;
}

void tff_nose_run::configure_velocity(double x, double y, double z, double ax, double ay, double az)
{
	generator_edp_data.next_velocity[0] = x;
	generator_edp_data.next_velocity[1] = y;
	generator_edp_data.next_velocity[2] = z;
	generator_edp_data.next_velocity[3] = ax;
	generator_edp_data.next_velocity[4] = ay;
	generator_edp_data.next_velocity[5] = az;
}

void tff_nose_run::configure_force(double x, double y, double z, double ax, double ay, double az)
{
	generator_edp_data.next_force_xyz_torque_xyz[0] = x;
	generator_edp_data.next_force_xyz_torque_xyz[1] = y;
	generator_edp_data.next_force_xyz_torque_xyz[2] = z;
	generator_edp_data.next_force_xyz_torque_xyz[3] = ax;
	generator_edp_data.next_force_xyz_torque_xyz[4] = ay;
	generator_edp_data.next_force_xyz_torque_xyz[5] = az;
}

void tff_nose_run::configure_reciprocal_damping(double x, double y, double z, double ax, double ay, double az)
{
	generator_edp_data.next_reciprocal_damping[0] = x;
	generator_edp_data.next_reciprocal_damping[1] = y;
	generator_edp_data.next_reciprocal_damping[2] = z;
	generator_edp_data.next_reciprocal_damping[3] = ax;
	generator_edp_data.next_reciprocal_damping[4] = ay;
	generator_edp_data.next_reciprocal_damping[5] = az;
}

void tff_nose_run::configure_inertia(double x, double y, double z, double ax, double ay, double az)
{
	generator_edp_data.next_inertia[0] = x;
	generator_edp_data.next_inertia[1] = y;
	generator_edp_data.next_inertia[2] = z;
	generator_edp_data.next_inertia[3] = ax;
	generator_edp_data.next_inertia[4] = ay;
	generator_edp_data.next_inertia[5] = az;
}

// ----------------------------------------------------------------------------------------------
// ---------------------------------    metoda	first_step -------------------------------------
// ----------------------------------------------------------------------------------------------

bool tff_nose_run::first_step()
{
	// Generacja trajektorii prostoliniowej o zadany przyrost polozenia i oreintacji
	// Funkcja zwraca false gdy koniec generacji trajektorii
	// Funkcja zwraca true gdy generacja trajektorii bedzie kontynuowana
	// cout << "first_step" << endl;

	td.interpolation_node_no = 1;
	td.internode_step_no = step_no;
	td.value_in_step_no = td.internode_step_no - 2;

	lib::Homog_matrix tool_frame(0.0, 0.0, 0.25);
	tool_frame.get_frame_tab(the_robot->ecp_command.instruction.robot_model.tool_frame_def.tool_frame);

	the_robot->ecp_command.instruction.instruction_type = lib::GET;
	the_robot->ecp_command.instruction.get_type = ARM_DEFINITION; // arm - ORYGINAL
	the_robot->ecp_command.instruction.set_type = ARM_DEFINITION | ROBOT_MODEL_DEFINITION;
	//	the_robot->ecp_command.instruction.set_type = ARM_DEFINITION;
	the_robot->ecp_command.instruction.set_robot_model_type = lib::TOOL_FRAME;
	the_robot->ecp_command.instruction.get_robot_model_type = lib::TOOL_FRAME;
	the_robot->ecp_command.instruction.set_arm_type = lib::PF_VELOCITY;
	the_robot->ecp_command.instruction.get_arm_type = lib::FRAME;
	the_robot->ecp_command.instruction.motion_type = lib::RELATIVE;
	the_robot->ecp_command.instruction.interpolation_type = lib::TCIM;
	the_robot->ecp_command.instruction.motion_steps = td.internode_step_no;
	the_robot->ecp_command.instruction.value_in_step_no = td.value_in_step_no;

	for (int i = 0; i < 6; i++) {
		the_robot->ecp_command.instruction.arm.pf_def.behaviour[i] = generator_edp_data.next_behaviour[i];
		the_robot->ecp_command.instruction.arm.pf_def.arm_coordinates[i] = generator_edp_data.next_velocity[i];
		the_robot->ecp_command.instruction.arm.pf_def.force_xyz_torque_xyz[i]
				= generator_edp_data.next_force_xyz_torque_xyz[i];
		the_robot->ecp_command.instruction.arm.pf_def.reciprocal_damping[i]
				= generator_edp_data.next_reciprocal_damping[i];
		the_robot->ecp_command.instruction.arm.pf_def.inertia[i] = generator_edp_data.next_inertia[i];
	}

	return true;
}

// ----------------------------------------------------------------------------------------------
// -----------------------------------  metoda	next_step -----------------------------------
// ----------------------------------------------------------------------------------------------

bool tff_nose_run::next_step()
{
	// Generacja trajektorii prostoliniowej o zadany przyrost polozenia i orientacji
	// Funkcja zwraca false gdy koniec generacji trajektorii
	// Funkcja zwraca true gdy generacja trajektorii bedzie kontynuowana
	// UWAGA: dzialamy na jednoelementowej liscie robotow
	//std::cout << "next_step" << std::endl;

	if (pulse_check_activated && check_and_null_trigger()) { // Koniec odcinka
		//	ecp_t.set_ecp_reply (lib::TASK_TERMINATED);

		return false;
	}

	// Przygotowanie kroku ruchu - do kolejnego wezla interpolacji
	the_robot->ecp_command.instruction.instruction_type = lib::SET_GET;

	// Obliczenie zadanej pozycji posredniej w tym kroku ruchu


	// wyrzucanie odczytu sil

	if (force_meassure) {
		lib::Homog_matrix current_frame_wo_offset(the_robot->reply_package.arm.pf_def.arm_frame);
		current_frame_wo_offset.remove_translation();

		lib::Ft_v_vector force_torque(the_robot->reply_package.arm.pf_def.force_xyz_torque_xyz);

		std::cout << "force: " << force_torque << std::endl;
	}
	return true;

}

// metoda przeciazona bo nie chcemy rzucac wyjatku wyjscia poza zakres ruchu - UWAGA napisany szkielet skorygowac cialo funkcji


void tff_nose_run::execute_motion(void)
{
	// Zlecenie wykonania ruchu przez robota jest to polecenie dla EDP

	// komunikacja wlasciwa
	the_robot->send();
	if (the_robot->reply_package.reply_type == lib::ERROR) {

		the_robot->query();
		throw common::robot::ECP_error(lib::NON_FATAL_ERROR, EDP_ERROR);

	}
	the_robot->query();

	if (the_robot->reply_package.reply_type == lib::ERROR) {
		switch (the_robot->reply_package.error_no.error0)
		{
			case BEYOND_UPPER_D0_LIMIT:
			case BEYOND_UPPER_THETA1_LIMIT:
			case BEYOND_UPPER_THETA2_LIMIT:
			case BEYOND_UPPER_THETA3_LIMIT:
			case BEYOND_UPPER_THETA4_LIMIT:
			case BEYOND_UPPER_THETA5_LIMIT:
			case BEYOND_UPPER_THETA6_LIMIT:
			case BEYOND_UPPER_THETA7_LIMIT:
			case BEYOND_LOWER_D0_LIMIT:
			case BEYOND_LOWER_THETA1_LIMIT:
			case BEYOND_LOWER_THETA2_LIMIT:
			case BEYOND_LOWER_THETA3_LIMIT:
			case BEYOND_LOWER_THETA4_LIMIT:
			case BEYOND_LOWER_THETA5_LIMIT:
			case BEYOND_LOWER_THETA6_LIMIT:
			case BEYOND_LOWER_THETA7_LIMIT:
				break;
			default:
				throw common::robot::ECP_error(lib::NON_FATAL_ERROR, EDP_ERROR);
				break;

		} /* end: switch */
	}
}

tff_rubik_grab::tff_rubik_grab(common::task::task& _ecp_task, int step) :
	generator(_ecp_task), step_no(step)
{
}

void tff_rubik_grab::configure(double l_goal_position, double l_position_increment, unsigned int l_min_node_counter, bool l_both_axes_running)
{
	goal_position = l_goal_position;
	position_increment = l_position_increment;
	min_node_counter = l_min_node_counter;
	both_axes_running = l_both_axes_running;
}

// ----------------------------------------------------------------------------------------------
// ---------------------------------    metoda	first_step -------------------------------------
// ----------------------------------------------------------------------------------------------

bool tff_rubik_grab::first_step()
{
	// Generacja trajektorii prostoliniowej o zadany przyrost polozenia i oreintacji
	// Funkcja zwraca false gdy koniec generacji trajektorii
	// Funkcja zwraca true gdy generacja trajektorii bedzie kontynuowana
	// cout << "first_step" << endl;

	td.interpolation_node_no = 1;
	td.internode_step_no = step_no;
	td.value_in_step_no = td.internode_step_no - 2;

	lib::Homog_matrix tool_frame(0.0, 0.0, 0.25);
	tool_frame.get_frame_tab(the_robot->ecp_command.instruction.robot_model.tool_frame_def.tool_frame);

	the_robot->ecp_command.instruction.instruction_type = lib::GET;
	the_robot->ecp_command.instruction.get_type = ARM_DEFINITION; // arm - ORYGINAL
	the_robot->ecp_command.instruction.set_type = ARM_DEFINITION | ROBOT_MODEL_DEFINITION;
	//	the_robot->ecp_command.instruction.set_type = ARM_DEFINITION;
	the_robot->ecp_command.instruction.set_robot_model_type = lib::TOOL_FRAME;
	the_robot->ecp_command.instruction.get_robot_model_type = lib::TOOL_FRAME;
	the_robot->ecp_command.instruction.set_arm_type = lib::PF_VELOCITY;
	the_robot->ecp_command.instruction.get_arm_type = lib::FRAME;
	the_robot->ecp_command.instruction.motion_type = lib::RELATIVE;
	the_robot->ecp_command.instruction.interpolation_type = lib::TCIM;
	the_robot->ecp_command.instruction.motion_steps = td.internode_step_no;
	the_robot->ecp_command.instruction.value_in_step_no = td.value_in_step_no;

	for (int i = 0; i < 6; i++) {
		the_robot->ecp_command.instruction.arm.pf_def.force_xyz_torque_xyz[i] = 0;
		the_robot->ecp_command.instruction.arm.pf_def.arm_coordinates[i] = 0;
	}

	for (int i = 0; i < 3; i++) {
		the_robot->ecp_command.instruction.arm.pf_def.inertia[i] = lib::FORCE_INERTIA;
		the_robot->ecp_command.instruction.arm.pf_def.inertia[i + 3] = lib::TORQUE_INERTIA;
	}

	if (both_axes_running)
		for (int i = 0; i < 2; i++) {
			the_robot->ecp_command.instruction.arm.pf_def.reciprocal_damping[i] = lib::FORCE_RECIPROCAL_DAMPING;
			the_robot->ecp_command.instruction.arm.pf_def.behaviour[i] = lib::CONTACT;
		}
	else {
		the_robot->ecp_command.instruction.arm.pf_def.reciprocal_damping[1] = lib::FORCE_RECIPROCAL_DAMPING;
		the_robot->ecp_command.instruction.arm.pf_def.behaviour[1] = lib::CONTACT;
		the_robot->ecp_command.instruction.arm.pf_def.behaviour[0] = lib::UNGUARDED_MOTION;
	}

	for (int i = 2; i < 6; i++) {
		the_robot->ecp_command.instruction.arm.pf_def.behaviour[i] = lib::UNGUARDED_MOTION;
	}

	return true;
}

// ----------------------------------------------------------------------------------------------
// -----------------------------------  metoda	next_step -----------------------------------
// ----------------------------------------------------------------------------------------------

bool tff_rubik_grab::next_step()
{
	// Generacja trajektorii prostoliniowej o zadany przyrost polozenia i orientacji
	// Funkcja zwraca false gdy koniec generacji trajektorii
	// Funkcja zwraca true gdy generacja trajektorii bedzie kontynuowana
	// UWAGA: dzialamy na jednoelementowej liscie robotow
	//	  cout << "next_step" << endl;


	// Przygotowanie kroku ruchu - do kolejnego wezla interpolacji
	the_robot->ecp_command.instruction.instruction_type = lib::SET_GET;

	// Obliczenie zadanej pozycji posredniej w tym kroku ruchu
	/*
	 if (node_counter == 1) {

	 the_robot->ecp_command.instruction.arm.pf_def.gripper_coordinate = 0;
	 desired_absolute_gripper_coordinate
	 = the_robot->reply_package.arm.pf_def.gripper_coordinate;

	 }

	 if ((desired_absolute_gripper_coordinate > goal_position) || (node_counter
	 < min_node_counter)) {
	 desired_absolute_gripper_coordinate -= position_increment;
	 the_robot->ecp_command.instruction.arm.pf_def.gripper_coordinate
	 = -position_increment;
	 } else {
	 return false;
	 }
	 */
	return true;
}

tff_rubik_face_rotate::tff_rubik_face_rotate(common::task::task& _ecp_task, int step) :
	generator(_ecp_task), step_no(step)
{
}

void tff_rubik_face_rotate::configure(double l_turn_angle)
{
	turn_angle = l_turn_angle;
}

// ----------------------------------------------------------------------------------------------
// ---------------------------------    metoda	first_step -------------------------------------
// ----------------------------------------------------------------------------------------------

bool tff_rubik_face_rotate::first_step()
{
	// Generacja trajektorii prostoliniowej o zadany przyrost polozenia i oreintacji
	// Funkcja zwraca false gdy koniec generacji trajektorii
	// Funkcja zwraca true gdy generacja trajektorii bedzie kontynuowana
	// cout << "first_step" << endl;

	td.interpolation_node_no = 1;
	td.internode_step_no = step_no;
	td.value_in_step_no = td.internode_step_no - 2;

	lib::Homog_matrix tool_frame(0.0, 0.0, 0.25);
	tool_frame.get_frame_tab(the_robot->ecp_command.instruction.robot_model.tool_frame_def.tool_frame);

	the_robot->ecp_command.instruction.instruction_type = lib::GET;
	the_robot->ecp_command.instruction.get_type = ARM_DEFINITION; // arm - ORYGINAL
	the_robot->ecp_command.instruction.set_type = ARM_DEFINITION | ROBOT_MODEL_DEFINITION;
	//	the_robot->ecp_command.instruction.set_type = ARM_DEFINITION;
	the_robot->ecp_command.instruction.set_robot_model_type = lib::TOOL_FRAME;
	the_robot->ecp_command.instruction.get_robot_model_type = lib::TOOL_FRAME;
	the_robot->ecp_command.instruction.set_arm_type = lib::PF_VELOCITY;
	the_robot->ecp_command.instruction.get_arm_type = lib::FRAME;
	the_robot->ecp_command.instruction.motion_type = lib::RELATIVE;
	the_robot->ecp_command.instruction.interpolation_type = lib::TCIM;
	the_robot->ecp_command.instruction.motion_steps = td.internode_step_no;
	the_robot->ecp_command.instruction.value_in_step_no = td.value_in_step_no;

	for (int i = 0; i < 6; i++) {
		the_robot->ecp_command.instruction.arm.pf_def.force_xyz_torque_xyz[i] = 0;
		the_robot->ecp_command.instruction.arm.pf_def.arm_coordinates[i] = 0;
	}

	for (int i = 0; i < 3; i++) {
		the_robot->ecp_command.instruction.arm.pf_def.inertia[i] = lib::FORCE_INERTIA;
		the_robot->ecp_command.instruction.arm.pf_def.inertia[i + 3] = lib::TORQUE_INERTIA;
	}

	the_robot->ecp_command.instruction.arm.pf_def.reciprocal_damping[5] = lib::TORQUE_RECIPROCAL_DAMPING / 4;
	the_robot->ecp_command.instruction.arm.pf_def.behaviour[5] = lib::CONTACT;

	if (-0.1 < turn_angle && turn_angle < 0.1) {
		for (int i = 0; i < 6; i++)
			the_robot->ecp_command.instruction.arm.pf_def.behaviour[i] = lib::UNGUARDED_MOTION;
	} else {
		for (int i = 0; i < 3; i++) {
			the_robot->ecp_command.instruction.arm.pf_def.reciprocal_damping[i] = lib::FORCE_RECIPROCAL_DAMPING;
			the_robot->ecp_command.instruction.arm.pf_def.behaviour[i] = lib::CONTACT;
		}
		for (int i = 3; i < 5; i++)
			the_robot->ecp_command.instruction.arm.pf_def.behaviour[i] = lib::UNGUARDED_MOTION;

		the_robot->ecp_command.instruction.arm.pf_def.reciprocal_damping[5] = lib::TORQUE_RECIPROCAL_DAMPING;
		the_robot->ecp_command.instruction.arm.pf_def.behaviour[5] = lib::CONTACT;
		the_robot->ecp_command.instruction.arm.pf_def.force_xyz_torque_xyz[5] = copysign(5.0, turn_angle);
	}

	return true;
}

// ----------------------------------------------------------------------------------------------
// -----------------------------------  metoda	next_step -----------------------------------
// ----------------------------------------------------------------------------------------------

bool tff_rubik_face_rotate::next_step()
{
	// Generacja trajektorii prostoliniowej o zadany przyrost polozenia i orientacji
	// Funkcja zwraca false gdy koniec generacji trajektorii
	// Funkcja zwraca true gdy generacja trajektorii bedzie kontynuowana
	// UWAGA: dzialamy na jednoelementowej liscie robotow
	// cout << "next_step" << endl;

	// Przygotowanie kroku ruchu - do kolejnego wezla interpolacji
	the_robot->ecp_command.instruction.instruction_type = lib::SET_GET;

	// Obliczenie zadanej pozycji posredniej w tym kroku ruchu

	if (node_counter == 1) {

		if (turn_angle < -0.1 || 0.1 < turn_angle) {
			lib::Homog_matrix frame(the_robot->reply_package.arm.pf_def.arm_frame);
			lib::Xyz_Euler_Zyz_vector xyz_eul_zyz;
			frame.get_xyz_euler_zyz(xyz_eul_zyz);
			double angle_to_move = (turn_angle / 180.0) * M_PI;
			if (xyz_eul_zyz[5] + angle_to_move < -M_PI) {
				stored_gamma = 2 * M_PI + xyz_eul_zyz[5] + angle_to_move;
				range_change = true;
			} else if (xyz_eul_zyz[5] + angle_to_move > M_PI) {
				stored_gamma = -2 * M_PI + xyz_eul_zyz[5] + angle_to_move;
				range_change = true;
			} else {
				stored_gamma = xyz_eul_zyz[5] + angle_to_move;
				range_change = false;
			}
		}
	} else {

		if (turn_angle < -0.1 || 0.1 < turn_angle) {
			lib::Homog_matrix current_frame(the_robot->reply_package.arm.pf_def.arm_frame);
			lib::Xyz_Euler_Zyz_vector xyz_eul_zyz;
			current_frame.get_xyz_euler_zyz(xyz_eul_zyz);
			double current_gamma = xyz_eul_zyz[5];
			if (!range_change) {
				if ((turn_angle < 0.0 && stored_gamma > current_gamma) || (turn_angle > 0.0 && stored_gamma
						< current_gamma)) {
					return false;
				}
			} else {
				if ((turn_angle < 0.0 && stored_gamma < current_gamma) || (turn_angle > 0.0 && stored_gamma
						> current_gamma)) {
					range_change = false;
				}
			}
		}

	}

	return true;
}

tff_gripper_approach::tff_gripper_approach(common::task::task& _ecp_task, int step) :
	generator(_ecp_task), step_no(step)
{
}

void tff_gripper_approach::configure(double l_speed, unsigned int l_motion_time)
{
	speed = l_speed;
	motion_time = l_motion_time;
}

// ----------------------------------------------------------------------------------------------
// ---------------------------------    metoda	first_step -------------------------------------
// ----------------------------------------------------------------------------------------------

bool tff_gripper_approach::first_step()
{
	td.interpolation_node_no = 1;
	td.internode_step_no = step_no;
	td.value_in_step_no = td.internode_step_no - 2;

	lib::Homog_matrix tool_frame(0.0, 0.0, 0.25);
	tool_frame.get_frame_tab(the_robot->ecp_command.instruction.robot_model.tool_frame_def.tool_frame);

	the_robot->ecp_command.instruction.instruction_type = lib::GET;
	the_robot->ecp_command.instruction.get_type = ARM_DEFINITION; // arm - ORYGINAL
	the_robot->ecp_command.instruction.set_type = ARM_DEFINITION | ROBOT_MODEL_DEFINITION;
	//	the_robot->ecp_command.instruction.set_type = ARM_DEFINITION;
	the_robot->ecp_command.instruction.set_robot_model_type = lib::TOOL_FRAME;
	the_robot->ecp_command.instruction.get_robot_model_type = lib::TOOL_FRAME;
	the_robot->ecp_command.instruction.set_arm_type = lib::PF_VELOCITY;
	the_robot->ecp_command.instruction.get_arm_type = lib::FRAME;
	the_robot->ecp_command.instruction.motion_type = lib::RELATIVE;
	the_robot->ecp_command.instruction.interpolation_type = lib::TCIM;
	the_robot->ecp_command.instruction.motion_steps = td.internode_step_no;
	the_robot->ecp_command.instruction.value_in_step_no = td.value_in_step_no;

	for (int i = 0; i < 6; i++) {
		the_robot->ecp_command.instruction.arm.pf_def.force_xyz_torque_xyz[i] = 0;
		the_robot->ecp_command.instruction.arm.pf_def.arm_coordinates[i] = 0;
	}

	for (int i = 0; i < 6; i++) {
		the_robot->ecp_command.instruction.arm.pf_def.inertia[i] = 0;
	}

	the_robot->ecp_command.instruction.arm.pf_def.inertia[2] = lib::FORCE_INERTIA / 4;
	the_robot->ecp_command.instruction.arm.pf_def.arm_coordinates[2] = speed;

	for (int i = 0; i < 6; i++) {
		the_robot->ecp_command.instruction.arm.pf_def.behaviour[i] = lib::UNGUARDED_MOTION;
		//		the_robot->EDP_data.ECPtoEDP_reciprocal_damping[i] = 0;
	}

	the_robot->ecp_command.instruction.arm.pf_def.behaviour[2] = lib::GUARDED_MOTION;
	the_robot->ecp_command.instruction.arm.pf_def.reciprocal_damping[2] = lib::FORCE_RECIPROCAL_DAMPING / 2;

	return true;
}

// ----------------------------------------------------------------------------------------------
// -----------------------------------  metoda	next_step -----------------------------------
// ----------------------------------------------------------------------------------------------

bool tff_gripper_approach::next_step()
{
	// Przygotowanie kroku ruchu - do kolejnego wezla interpolacji
	the_robot->ecp_command.instruction.instruction_type = lib::SET_GET;

	// Obliczenie zadanej pozycji posredniej w tym kroku ruchu

	if (node_counter == 1) {

	} else if (node_counter > motion_time) {
		return false;
	}

	return true;
}

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // ///////////////////
//
// 			ecp_force_tool_change_generator
//
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // ///////////////////

force_tool_change::force_tool_change(common::task::task& _ecp_task) :
	generator(_ecp_task)
{
	set_tool_parameters(-0.18, 0.0, 0.25, 0);
}

bool force_tool_change::first_step()
{
	the_robot->ecp_command.instruction.instruction_type = lib::SET;
	the_robot->ecp_command.instruction.set_type = ROBOT_MODEL_DEFINITION;
	the_robot->ecp_command.instruction.set_robot_model_type = lib::FORCE_TOOL;

	for (int i = 0; i < 3; i++)
		the_robot->ecp_command.instruction.robot_model.force_tool.position[i] = tool_parameters[i];
	the_robot->ecp_command.instruction.robot_model.force_tool.weight = weight;

	return true;
}

bool force_tool_change::next_step()
{
	return false;
}

void force_tool_change::set_tool_parameters(double x, double y, double z, double v)
{
	tool_parameters[0] = x;
	tool_parameters[1] = y;
	tool_parameters[2] = z;
	weight = v;
}

} // namespace generator
} // namespace common
} // namespace ecp
} // namespace mrrocpp