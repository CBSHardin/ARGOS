////////////////////////////////////////
//
// Waypoint follower
//
///////////////////////////////////////
// Esta libreria implementa las funciones de navegación
// por waypoints. Una vez se de la orden de inicio del plan
// el robot seguira los waypoints y se detendrá cuando el plan se haya completado.



#define MAX_NUMBER_WAYPOINTS 20


// Tolerance parameters
#define waypoint_pos_tolerance_bandgap 0.05
#define steady_state_v_lineal_bandgap 0.1

#define turn_angle_tolerance_bandgap 2*PI/180
#define steady_state_w_bandgap 0.05



// Timming paramenters
#define controlSystemPeriodMillis 25
#define stateSupervisorPeriodMillis 25




typedef enum {WPF_STOPPED, WPF_RUNNING, WPF_PAUSED, WPF_FINISHED} waypointsFollowerStates;

typedef struct
{
  float x;
  float y;
}
posVector2D;





boolean addWaypointToPlan(posVector2D waypoint);

void startPlanExecution();

void pausePlanExecution();

void resumePlanExecution();

void deletePlan();

void restartPlanExecution();

waypointsFollowerStates getPlanExecutionStatus();


void waypointsFollowerInitTask();

void waypointsFollowerDoTask();
