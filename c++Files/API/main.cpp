#include "robot_project.h"

...

// Skeleton example of the flow of execution of the main program, and calls to 
// the different methods of the class RobotProject.
int main(int argc, char * argv[])
{
  
  ...

  RobotProject rp(argc, argv);
  if (!rp.preprocessMap(img))
  {
    std::cerr << "(Critical) Failed to preprocess map" << std::endl;
    return false;
  }

  Path path;
  if (!rp.planPath(img, path))
  {
    std::cerr << "(Critical) Failed to plan path" << std::endl;
    return false;
  }
  
  
  ...

  while (...)
  {
    ...

    std::vector<double> state;
    if (!rp.localize(img, state))
    {
      std::cerr << "(Warning) Failed localization" << std::endl;
      continue;
    }
    
    ...

  }

  ...

}
