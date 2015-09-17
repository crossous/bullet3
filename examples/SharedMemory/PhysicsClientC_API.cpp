#include "PhysicsClientC_API.h"
#include "PhysicsClient.h"
#include "Bullet3Common/b3Scalar.h"
#include <string.h>
#include "SharedMemoryCommands.h"



b3SharedMemoryCommandHandle	b3LoadUrdfCommandInit(b3PhysicsClientHandle physClient, const char* urdfFileName)
{
    PhysicsClientSharedMemory* cl = (PhysicsClientSharedMemory* ) physClient;
    b3Assert(cl);
    b3Assert(cl->canSubmitCommand());
    
    
    struct SharedMemoryCommand* command = cl->getAvailableSharedMemoryCommand();
    b3Assert(command);
	command->m_type = CMD_LOAD_URDF;
	int len = strlen(urdfFileName);
	if (len<MAX_URDF_FILENAME_LENGTH)
	{
		strcpy(command->m_urdfArguments.m_urdfFileName,urdfFileName);
	} else
	{
		command->m_urdfArguments.m_urdfFileName[0] = 0;
	}
	command->m_updateFlags = URDF_ARGS_FILE_NAME;
	
	return (b3SharedMemoryCommandHandle) command;
}


int	b3LoadUrdfCommandSetUseFixedBase(b3SharedMemoryCommandHandle commandHandle, int useFixedBase)
{
    struct SharedMemoryCommand* command = (struct SharedMemoryCommand*) commandHandle;
    b3Assert(command);
    b3Assert(command->m_type == CMD_LOAD_URDF);
    command->m_updateFlags |=URDF_ARGS_USE_FIXED_BASE;
    command->m_urdfArguments.m_useFixedBase = useFixedBase;
    
    return 0;
}



int	b3LoadUrdfCommandSetStartPosition(b3SharedMemoryCommandHandle commandHandle, double startPosX,double startPosY,double startPosZ)
{
    struct SharedMemoryCommand* command = (struct SharedMemoryCommand*) commandHandle;
	b3Assert(command);
	b3Assert(command->m_type == CMD_LOAD_URDF);
	command->m_urdfArguments.m_initialPosition[0] = startPosX;
	command->m_urdfArguments.m_initialPosition[1] = startPosY;
	command->m_urdfArguments.m_initialPosition[2] = startPosZ;
	command->m_updateFlags|=URDF_ARGS_INITIAL_POSITION;
	return 0;
}

int	b3LoadUrdfCommandSetStartOrientation(b3SharedMemoryCommandHandle commandHandle, double startOrnX,double startOrnY,double startOrnZ, double startOrnW)
{
    struct SharedMemoryCommand* command = (struct SharedMemoryCommand*) commandHandle;
	b3Assert(command);
	b3Assert(command->m_type == CMD_LOAD_URDF);
	command->m_urdfArguments.m_initialOrientation[0] = startOrnX;
	command->m_urdfArguments.m_initialOrientation[1] = startOrnY;
	command->m_urdfArguments.m_initialOrientation[2] = startOrnZ;
	command->m_urdfArguments.m_initialOrientation[3] = startOrnW;
	command->m_updateFlags|=URDF_ARGS_INITIAL_ORIENTATION;
	return 0;
}

b3SharedMemoryCommandHandle     b3InitPhysicsParamCommand(b3PhysicsClientHandle physClient)
{
    PhysicsClientSharedMemory* cl = (PhysicsClientSharedMemory* ) physClient;
    b3Assert(cl);
    b3Assert(cl->canSubmitCommand());
    struct SharedMemoryCommand* command = cl->getAvailableSharedMemoryCommand();
    b3Assert(command);
	command->m_type = CMD_SEND_PHYSICS_SIMULATION_PARAMETERS;
	command->m_updateFlags = 0;

    return (b3SharedMemoryCommandHandle) command;
}
int     b3PhysicsParamSetGravity(b3SharedMemoryCommandHandle commandHandle, double gravx,double gravy, double gravz)
{
    struct SharedMemoryCommand* command = (struct SharedMemoryCommand*) commandHandle;
	b3Assert(command->m_type == CMD_SEND_PHYSICS_SIMULATION_PARAMETERS);
	command->m_physSimParamArgs.m_gravityAcceleration[0] = gravx;
	command->m_physSimParamArgs.m_gravityAcceleration[1] = gravy;
	command->m_physSimParamArgs.m_gravityAcceleration[2] = gravz; 
	command->m_updateFlags |= SIM_PARAM_UPDATE_GRAVITY;
	return 0;
}

int	b3PhysicsParamSetTimeStep(b3SharedMemoryCommandHandle commandHandle, double timeStep)
{
    struct SharedMemoryCommand* command = (struct SharedMemoryCommand*) commandHandle;
	b3Assert(command->m_type == CMD_SEND_PHYSICS_SIMULATION_PARAMETERS);
	command->m_physSimParamArgs.m_deltaTime = timeStep;
	return 0;
}

b3SharedMemoryCommandHandle	b3InitStepSimulationCommand(b3PhysicsClientHandle physClient)
{
    PhysicsClientSharedMemory* cl = (PhysicsClientSharedMemory* ) physClient;
    b3Assert(cl);
    b3Assert(cl->canSubmitCommand());
    struct SharedMemoryCommand* command = cl->getAvailableSharedMemoryCommand();
    b3Assert(command);
	command->m_type = CMD_STEP_FORWARD_SIMULATION;
	command->m_updateFlags = 0;
	 return (b3SharedMemoryCommandHandle) command;
}

b3SharedMemoryCommandHandle     b3InitResetSimulationCommand(b3PhysicsClientHandle physClient)
{
    PhysicsClientSharedMemory* cl = (PhysicsClientSharedMemory* ) physClient;
    b3Assert(cl);
    b3Assert(cl->canSubmitCommand());
    struct SharedMemoryCommand* command = cl->getAvailableSharedMemoryCommand();
    b3Assert(command);
    command->m_type = CMD_RESET_SIMULATION;
    command->m_updateFlags = 0;

     return (b3SharedMemoryCommandHandle) command;

}


b3SharedMemoryCommandHandle b3JointControlCommandInit( b3PhysicsClientHandle physClient, int controlMode)
{
    PhysicsClientSharedMemory* cl = (PhysicsClientSharedMemory* ) physClient;
    b3Assert(cl);
    b3Assert(cl->canSubmitCommand());
    struct SharedMemoryCommand* command = cl->getAvailableSharedMemoryCommand();
    b3Assert(command);
	command->m_type = CMD_SEND_DESIRED_STATE;
    command->m_sendDesiredStateCommandArgument.m_controlMode = controlMode;
	command->m_updateFlags = 0;
    return (b3SharedMemoryCommandHandle) command;
}

int b3JointControlSetDesiredPosition(b3SharedMemoryCommandHandle commandHandle, int dofIndex, double value)
{
    struct SharedMemoryCommand* command = (struct SharedMemoryCommand*) commandHandle;
    b3Assert(command);
    command->m_sendDesiredStateCommandArgument.m_desiredStateQ[dofIndex] = value;
    return 0;
}

int b3JointControlSetKp(b3SharedMemoryCommandHandle commandHandle, int dofIndex, double value)
{
    struct SharedMemoryCommand* command = (struct SharedMemoryCommand*) commandHandle;
    b3Assert(command);
    command->m_sendDesiredStateCommandArgument.m_Kp[dofIndex] = value;
    return 0;
}

int b3JointControlSetKd(b3SharedMemoryCommandHandle commandHandle, int dofIndex, double value)
{
    struct SharedMemoryCommand* command = (struct SharedMemoryCommand*) commandHandle;
    b3Assert(command);
    command->m_sendDesiredStateCommandArgument.m_Kd[dofIndex] = value;
    return 0;
}

int b3JointControlSetDesiredVelocity(b3SharedMemoryCommandHandle commandHandle, int dofIndex, double value)
{
    struct SharedMemoryCommand* command = (struct SharedMemoryCommand*) commandHandle;
    b3Assert(command);
    command->m_sendDesiredStateCommandArgument.m_desiredStateQdot[dofIndex] = value;
    return 0;
}


int b3JointControlSetMaximumForce(b3SharedMemoryCommandHandle commandHandle, int dofIndex,  double value)
{
    struct SharedMemoryCommand* command = (struct SharedMemoryCommand*) commandHandle;
    b3Assert(command);
    command->m_sendDesiredStateCommandArgument.m_desiredStateForceTorque[dofIndex] = value;
    return 0;
}

int b3JointControlSetDesiredForceTorque(b3SharedMemoryCommandHandle commandHandle, int  dofIndex, double value)
{
    struct SharedMemoryCommand* command = (struct SharedMemoryCommand*) commandHandle;
    b3Assert(command);
    command->m_sendDesiredStateCommandArgument.m_desiredStateForceTorque[dofIndex] = value;
    return 0;
}


b3SharedMemoryCommandHandle b3RequestActualStateCommandInit(b3PhysicsClientHandle physClient)
{
    PhysicsClientSharedMemory* cl = (PhysicsClientSharedMemory* ) physClient;
    b3Assert(cl);
    b3Assert(cl->canSubmitCommand());
    struct SharedMemoryCommand* command = cl->getAvailableSharedMemoryCommand();
    b3Assert(command);
    command->m_type =CMD_REQUEST_ACTUAL_STATE;
    return (b3SharedMemoryCommandHandle) command;
}

b3SharedMemoryCommandHandle b3CreateBoxShapeCommandInit(b3PhysicsClientHandle physClient)
{
    PhysicsClientSharedMemory* cl = (PhysicsClientSharedMemory* ) physClient;
    b3Assert(cl);
    b3Assert(cl->canSubmitCommand());
    struct SharedMemoryCommand* command = cl->getAvailableSharedMemoryCommand();
    b3Assert(command);
    command->m_type = CMD_CREATE_BOX_COLLISION_SHAPE;
    command->m_updateFlags =0;
    return (b3SharedMemoryCommandHandle) command;
}

int	b3CreateBoxCommandSetStartPosition(b3SharedMemoryCommandHandle commandHandle, double startPosX,double startPosY,double startPosZ)
{
    struct SharedMemoryCommand* command = (struct SharedMemoryCommand*) commandHandle;
    b3Assert(command);
    b3Assert(command->m_type == CMD_CREATE_BOX_COLLISION_SHAPE);
    command->m_updateFlags |=BOX_SHAPE_HAS_INITIAL_POSITION;
    
    command->m_createBoxShapeArguments.m_initialPosition[0] = startPosX;
    command->m_createBoxShapeArguments.m_initialPosition[1] = startPosY;
    command->m_createBoxShapeArguments.m_initialPosition[2] = startPosZ;
    return 0;
}

int	b3CreateBoxCommandSetStartOrientation(b3SharedMemoryCommandHandle commandHandle, double startOrnX,double startOrnY,double startOrnZ, double startOrnW)
{
    struct SharedMemoryCommand* command = (struct SharedMemoryCommand*) commandHandle;
    b3Assert(command);
    b3Assert(command->m_type == CMD_CREATE_BOX_COLLISION_SHAPE);
    command->m_updateFlags |=BOX_SHAPE_HAS_INITIAL_ORIENTATION;
    
    command->m_createBoxShapeArguments.m_initialOrientation[0] = startOrnX;
    command->m_createBoxShapeArguments.m_initialOrientation[1] = startOrnY;
    command->m_createBoxShapeArguments.m_initialOrientation[2] = startOrnZ;
    command->m_createBoxShapeArguments.m_initialOrientation[3] = startOrnW;
    return 0;
}

int	b3CreateBoxCommandSetHalfExtents(b3SharedMemoryCommandHandle commandHandle, double halfExtentsX,double halfExtentsY,double halfExtentsZ)
{
    struct SharedMemoryCommand* command = (struct SharedMemoryCommand*) commandHandle;
    b3Assert(command);
    b3Assert(command->m_type == CMD_CREATE_BOX_COLLISION_SHAPE);
    command->m_updateFlags |=BOX_SHAPE_HAS_HALF_EXTENTS;
    
    command->m_createBoxShapeArguments.m_halfExtentsX = halfExtentsX;
    command->m_createBoxShapeArguments.m_halfExtentsY = halfExtentsY;
    command->m_createBoxShapeArguments.m_halfExtentsZ = halfExtentsZ;

    return 0;
}



b3SharedMemoryCommandHandle b3CreateSensorCommandInit(b3PhysicsClientHandle physClient)
{
    PhysicsClientSharedMemory* cl = (PhysicsClientSharedMemory* ) physClient;
    b3Assert(cl);
    b3Assert(cl->canSubmitCommand());
    struct SharedMemoryCommand* command = cl->getAvailableSharedMemoryCommand();
    b3Assert(command);
    
    command->m_type = CMD_CREATE_SENSOR;
    command->m_updateFlags = 0;
    command->m_createSensorArguments.m_numJointSensorChanges = 0;
    return (b3SharedMemoryCommandHandle) command;
    
}

int b3CreateSensorEnable6DofJointForceTorqueSensor(b3SharedMemoryCommandHandle commandHandle, int jointIndex, int enable)
{
    struct SharedMemoryCommand* command = (struct SharedMemoryCommand*) commandHandle;
    b3Assert(command);
    b3Assert(command->m_type == CMD_CREATE_SENSOR);
    int curIndex = command->m_createSensorArguments.m_numJointSensorChanges;
    command->m_createSensorArguments.m_sensorType[curIndex] = SENSOR_FORCE_TORQUE;
    
    command->m_createSensorArguments.m_jointIndex[curIndex] = jointIndex;
    command->m_createSensorArguments.m_enableJointForceSensor[curIndex] = enable;
    command->m_createSensorArguments.m_numJointSensorChanges++;
    return 0;
}

int b3CreateSensorEnableIMUForLink(b3SharedMemoryCommandHandle commandHandle, int linkIndex, int enable)
{
    struct SharedMemoryCommand* command = (struct SharedMemoryCommand*) commandHandle;
    b3Assert(command);
    b3Assert(command->m_type == CMD_CREATE_SENSOR);
    int curIndex = command->m_createSensorArguments.m_numJointSensorChanges;
    command->m_createSensorArguments.m_sensorType[curIndex] = SENSOR_IMU;
    command->m_createSensorArguments.m_linkIndex[curIndex] = linkIndex;
    command->m_createSensorArguments.m_enableSensor[curIndex] = enable;
    command->m_createSensorArguments.m_numJointSensorChanges++;
	return 0;    
}


b3PhysicsClientHandle b3ConnectSharedMemory(int key)
{

	PhysicsClientSharedMemory* cl = new PhysicsClientSharedMemory();
    ///client should never create shared memory, only the server does
    cl->setSharedMemoryKey(key);
    cl->connect();
	return (b3PhysicsClientHandle ) cl;
}

void	b3DisconnectSharedMemory(b3PhysicsClientHandle physClient)
{
	PhysicsClientSharedMemory* cl = (PhysicsClientSharedMemory* ) physClient;
	delete cl;
}

b3SharedMemoryStatusHandle b3ProcessServerStatus(b3PhysicsClientHandle physClient)
{
	PhysicsClientSharedMemory* cl = (PhysicsClientSharedMemory* ) physClient;
	const SharedMemoryStatus* stat = cl->processServerStatus();
    return (b3SharedMemoryStatusHandle) stat;
    
}




int b3GetStatusType(b3SharedMemoryStatusHandle statusHandle)
{
    const SharedMemoryStatus* status = (const SharedMemoryStatus* ) statusHandle;
    b3Assert(status);
    if (status)
    {
        return status->m_type;
    }
    return 0;
}


int	b3CanSubmitCommand(b3PhysicsClientHandle physClient)
{
	PhysicsClientSharedMemory* cl = (PhysicsClientSharedMemory* ) physClient;
	return (int)cl->canSubmitCommand();
}

int	b3SubmitClientCommand(b3PhysicsClientHandle physClient, const b3SharedMemoryCommandHandle commandHandle)
{
    struct SharedMemoryCommand* command = (struct SharedMemoryCommand*) commandHandle;
    PhysicsClientSharedMemory* cl = (PhysicsClientSharedMemory* ) physClient;
    return (int)cl->submitClientCommand(*command);
}

b3SharedMemoryStatusHandle b3SubmitClientCommandAndWaitStatus(b3PhysicsClientHandle physClient, const b3SharedMemoryCommandHandle commandHandle)
{
    int timeout = 1024*1024*1024;
    b3SharedMemoryStatusHandle statusHandle=0;
    
    b3SubmitClientCommand(physClient,commandHandle);
    
    while ((statusHandle==0) && (timeout-- > 0))
    {
        statusHandle =b3ProcessServerStatus(physClient);
    }
    return (b3SharedMemoryStatusHandle) statusHandle;
    
}


int	b3GetNumJoints(b3PhysicsClientHandle physClient)
{
	PhysicsClientSharedMemory* cl = (PhysicsClientSharedMemory* ) physClient;
	return cl->getNumJoints();
}


void	b3GetJointInfo(b3PhysicsClientHandle physClient, int linkIndex, struct b3JointInfo* info)
{
	PhysicsClientSharedMemory* cl = (PhysicsClientSharedMemory* ) physClient;
	cl->getJointInfo(linkIndex,*info);
}



b3SharedMemoryCommandHandle b3InitRequestDebugLinesCommand(b3PhysicsClientHandle physClient, int debugMode)
{
    PhysicsClientSharedMemory* cl = (PhysicsClientSharedMemory* ) physClient;
    b3Assert(cl);
    b3Assert(cl->canSubmitCommand());
    struct SharedMemoryCommand* command = cl->getAvailableSharedMemoryCommand();
    b3Assert(command);
    
    command->m_type =CMD_REQUEST_DEBUG_LINES;
    command->m_requestDebugLinesArguments.m_debugMode = debugMode;
    command->m_requestDebugLinesArguments.m_startingLineIndex = 0;
    return (b3SharedMemoryCommandHandle) command;
}
void    b3GetDebugLines(b3PhysicsClientHandle physClient, struct b3DebugLines* lines)
{
    PhysicsClientSharedMemory* cl = (PhysicsClientSharedMemory* ) physClient;

    b3Assert(lines);
    if (lines)
    {
        lines->m_numDebugLines = cl->getNumDebugLines();
        lines->m_linesFrom = cl->getDebugLinesFrom();
        lines->m_linesTo = cl->getDebugLinesTo();
        lines->m_linesColor = cl->getDebugLinesColor();
        
    }
    
}
