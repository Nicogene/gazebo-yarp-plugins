/*
 * Copyright (C) 2007-2013 Istituto Italiano di Tecnologia ADVR & iCub Facility
 * Authors: Enrico Mingo, Alessio Rocchi, Mirko Ferrati, Silvio Traversaro and Alessandro Settimi
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */


#include "gazebo_yarp_plugins/ControlBoardDriver.h"
#include <gazebo/physics/Model.hh>
#include <gazebo/physics/Joint.hh>
#include <gazebo/transport/Publisher.hh>


using namespace yarp::dev;

void GazeboYarpControlBoardDriver::prepareResetJointMsg(int j)
{
    gazebo::msgs::JointCmd j_cmd;
    j_cmd.set_reset(true);
    j_cmd.set_name(this->m_robot->GetJoint(m_jointNames[j])->GetScopedName());
    this->m_jointCommandPublisher->WaitForConnection();
    this->m_jointCommandPublisher->Publish(j_cmd);
}

bool GazeboYarpControlBoardDriver::setPositionMode(int j) //WORKS
{
    return this->setControlMode(j,VOCAB_CM_POSITION);
}

bool GazeboYarpControlBoardDriver::setVelocityMode(int j) //WORKS
{
    return this->setControlMode(j,VOCAB_CM_VELOCITY);
}

bool GazeboYarpControlBoardDriver::getControlMode(int j, int *mode) //WORKS
{
    if (!mode || j < 0 || j >= (int)m_numberOfJoints)
        return false;
    *mode = m_controlMode[j];
    return true;
}

bool GazeboYarpControlBoardDriver::getControlModes(int *modes) //NOT TESTED
{
    if (!modes) return false;
    for(unsigned int j = 0; j < m_numberOfJoints; ++j) {
        modes[j] = m_controlMode[j];
    }
    return true;
}

bool GazeboYarpControlBoardDriver::setTorqueMode(int j) //NOT TESTED
{
    return this->setControlMode(j,VOCAB_CM_TORQUE);
}

bool GazeboYarpControlBoardDriver::setImpedancePositionMode(int j)//NOT TESTED
{
    bool ret = true;
    ret = ret && this->setControlMode(j,VOCAB_CM_POSITION);
    ret = ret && this->setInteractionMode(j,VOCAB_IM_COMPLIANT);
    return ret;
}

bool GazeboYarpControlBoardDriver::setImpedanceVelocityMode(int) //NOT IMPLEMENTED
{
    return false;
}

bool GazeboYarpControlBoardDriver::setOpenLoopMode(int j) //NOT IMPLEMENTED
{
    return this->setControlMode(j,VOCAB_CM_OPENLOOP);
}

bool GazeboYarpControlBoardDriver::getControlModes(const int n_joint, const int *joints, int *modes)
{
    bool ret = true;
    for(int i=0; i<n_joint; i++)
        ret = ret && getControlMode(joints[i], &modes[i]);
    return ret;
}

bool GazeboYarpControlBoardDriver::setControlMode(const int j, const int mode)
{
    if (j < 0 || j >= (int)m_numberOfJoints) return false;
    prepareResetJointMsg(j);
    m_controlMode[j] = mode;

    // mode specific switching actions
    switch( mode ) {
        case VOCAB_CM_POSITION :
        case VOCAB_CM_POSITION_DIRECT :
            m_referencePositions[j] = m_positions[j];
            m_trajectoryGenerationReferencePosition[j] = m_positions[j];
        break;
        case VOCAB_CM_VELOCITY :
            m_referenceVelocities[j] = 0.0;
        break;
        case VOCAB_CM_TORQUE :
        case VOCAB_CM_OPENLOOP :
            m_referenceTorques[j] = m_torques[j];
        break;
        default :
        break;
    }
    return true;
}

bool GazeboYarpControlBoardDriver::setControlModes(const int n_joint, const int *joints, int *modes)
{
    bool ret = true;
    for(int i=0; i<n_joint; i++)
        ret = ret && setControlMode(joints[i], modes[i]);
    return ret;
}

bool GazeboYarpControlBoardDriver::setControlModes(int *modes)
{
    bool ret = true;
    for(int i=0; i<(int)m_numberOfJoints; i++)
        ret = ret && setControlMode(i, modes[i]);
    return ret;
}


