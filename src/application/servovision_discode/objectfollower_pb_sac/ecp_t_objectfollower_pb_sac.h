/*
 * ecp_t_objectfollower_pb_sac.h
 *
 *  Created on: May 20, 2010
 *      Author: mboryn
 */

#ifndef ECP_T_OBJECTFOLLOWER_PB_SAC_H_
#define ECP_T_OBJECTFOLLOWER_PB_SAC_H_


#include <boost/shared_ptr.hpp>
#include "base/lib/logger.h"
#include "../single_visual_servo_manager.h"
#include "../pb_sac_visual_servo.h"
#include "../cubic_constraint.h"
#include "../object_reached_termination_condition.h"
#include "../visual_servo_regulator_p.h"

#include "base/ecp/ecp_task.h"
#include "robot/irp6p_m/ecp_r_irp6p_m.h"

using mrrocpp::ecp::common::generator::single_visual_servo_manager;
using namespace mrrocpp::ecp::servovision;

namespace mrrocpp {

namespace ecp {

namespace irp6ot_m {

namespace task {

/** @addtogroup servovision
 *  @{
 */

/**
 *
 */
class ecp_t_objectfollower_pb_sac : public mrrocpp::ecp::common::task::task
{
public:
	ecp_t_objectfollower_pb_sac(mrrocpp::lib::configurator& config);

	void main_task_algorithm();
protected:
	boost::shared_ptr <visual_servo_regulator> reg;
	boost::shared_ptr <single_visual_servo_manager> sm;
	boost::shared_ptr <visual_servo> vs;
};

/** @} */

} // namespace task

} // namespace irp6ot

} // namespace ecp

} // namespace mrrocpp

#endif /* ECP_T_OBJECTFOLLOWER_PB_SAC_H_ */
