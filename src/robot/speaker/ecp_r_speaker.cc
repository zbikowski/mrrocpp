// -------------------------------------------------------------------------
//                            ecp.cc
//            Effector Control Process (lib::ECP) - methods
// Funkcje do tworzenia procesow ECP
// robot - speaker
//
// Ostatnia modyfikacja: 16.04.98
// -------------------------------------------------------------------------

#include "base/lib/impconst.h"
#include "base/lib/com_buf.h"

#include "robot/speaker/ecp_r_speaker.h"

namespace mrrocpp {
namespace ecp {
namespace speaker {

robot::robot(lib::configurator &_config, lib::sr_ecp &_sr_ecp) :
	_base_robot_t(lib::speaker::ROBOT_NAME, 0, lib::speaker::EDP_SECTION, _config, _sr_ecp)
{
}

robot::robot(common::task::task_base& _ecp_object) :
	_base_robot_t(lib::speaker::ROBOT_NAME, 0, lib::speaker::EDP_SECTION, _ecp_object)
{
}

} // namespace speaker
} // namespace ecp
} // namespace mrrocpp


