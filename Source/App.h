#ifndef RVYA_APP__
#define RVYA_APP__

#include <AR/ar.h>


#include <string>
#include <chrono>
#include <thread>
#include <vector>

#include "Marker.h"
#include "Renderer.h"
#include "ConfigFile.h"


/**
 * \brief Transforms a target framerate frequence to period in milliseconds
 * 
 * The class App has a member variable that stores the target framerate 
 * of the application, in order to create a timepoint that waits until the 
 * next frame this function is needed.
 * 
 * \param framerate The target framerate in frames per second.
 * \return Integer number of milliseconds to wait untill next frame.
 **/
constexpr int64_t FramerateToFrametime (float framerate)
    { return (1.f/framerate) * 1000 - 1; }


using namespace std;
using namespace chrono;
using namespace this_thread;


using StringList    = vector<string>;
using Clock         = system_clock;


/**
 * @brief Class to encapsulate the main workflow of the application.
 * 
 * The App class is used as a wrapper to control the frametimes, store 
 * global variables and define workflow functions.
 * 
 * \param configFilePath is the path of the .cfg or .ini file that 
 * specifies the configuration of the class.
 * 
 */
class App
{
public:
    
    App (const string &configFilePath = "./Config/Game.cfg");
    
    virtual ~App ();
    
    /** Rule of 5 **/

    /**
     * @brief An App should not be moved.
     * 
     */
    App (App &&) = delete; 

    /**
     * @brief An App should not be cloned.
     * 
     */
    App (const App &) = delete;

    /**
     * @brief And app should not be move assigned.
     * 
     */
    virtual App &&operator= (App &&) = delete;

    /**
     * @brief And app should not be copy assigned.
     * 
     */
    virtual App &operator= (const App &) = delete;

    /**
     * @brief Main entrypoint of the application
     * 
     * The intended use is to create the App and Run it
     * 
     */
    void Run ();

    /**
     * @brief Preinitializa an application
     * 
     * This function needs to be called before the Run() function.
     * Creates and initializes the ARToolkit library.
     * 
     */
    void Setup ( );

protected:

    /**
     * @brief Stores the configuration of the Application.
     * 
     */
    ConfigFile configuration;

    /**
     * @brief Markers used by the application
     * 
     */
    vector<Marker> markers;

    /**
     * @brief Flag to finish the execution of the application.
     * 
     */
    bool finished;

    /**
     * @brief Renderer that draw stuff.
     * 
     */
    Renderer renderer;

protected:

    /**
     * @brief Update function.
     * 
     * Function to be Called every frame. Can be overridable 
     * in child classes.
     * 
     * @param elapsedTime is the time passed since the beginning 
     * of the session. 
     */
    virtual void Tick (float elapsedTime);

private:

    /**
     * @brief Timestamp to calculate frametimes and elapsed times.
     * 
     */
    Clock::time_point beginTime;

    /**
     * @brief Size of the historical.
     * 
     */
    uint16_t zBufferSize;

    /**
     * @brief Sets the target framerate of the aplication.
     * 
     * Usually set between 15 and 60, the quality of the camera may 
     * allow faster framerates. Be carefull, the CPU and GPU cost of 
     * raising the number beyond 25 is very expensive.
     * 
     */
    float targetFramerate;

    /**
     * @brief Black and white threshold of ARToolKit detection algorithm. 
     * 
     */
    int errorThreshold;

private:

    /**
     * @brief Initiializes ARToolKit camera.
     * 
     */
    void SetupVideoCapture();

    /**
     * @brief Instantiates all the Markers.
     * 
     */
    void CreatePatterns();

    /**
     * @brief Frees up resources.
     * 
     */
    void Cleanup();

};


#endif // !RVYA_APP__
