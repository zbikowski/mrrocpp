# Recurse into subdirectories.

add_subdirectory (bclikeregions)
add_subdirectory (ball)
add_subdirectory (bird_hand_test)
add_subdirectory (calibration)
add_subdirectory (draughts)
add_subdirectory (dung)
add_subdirectory (ecp_transparent)
add_subdirectory (edge_follow)
add_subdirectory (generator_tester)
add_subdirectory (graspit)
add_subdirectory (haar)
add_subdirectory (haptic)
add_subdirectory (haptic_stiffness)
add_subdirectory (mp_common)
add_subdirectory (multiplayer)
add_subdirectory (neuron)
add_subdirectory (playerjoy)
add_subdirectory (pnml)
add_subdirectory (rcsc)
if(USE_DISCODE)
    add_subdirectory (servovision_discode)
else(USE_DISCODE)
    add_subdirectory (servovision)
endif(USE_DISCODE)
add_subdirectory (swarm_demo)
add_subdirectory (swarmitfix)
add_subdirectory (teach)
add_subdirectory (teach-dll)
add_subdirectory (time)
add_subdirectory (wii_teach)
