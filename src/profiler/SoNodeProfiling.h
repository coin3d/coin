#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#ifdef HAVE_SCENE_PROFILING

#include <Inventor/nodes/SoGroup.h>
#include <Inventor/annex/Profiler/SoProfiler.h>

#include "misc/SoDBP.h" // for global envvar COIN_PROFILER
#include "profiler/SoProfilerElement.h"

/*
  The SoNodeProfiling class contains instrumentation code for scene
  graph profiling.
*/

class SoNodeProfiling {
public:
  SoNodeProfiling(void)
    : pretime(SbTime::zero())
  {
  }

  void preTraversal(SoAction * action, SoNode * parent, SoNode * child)
  {
    if (!this->isActive(action)) return;

    pretime = SbTime::getTimeOfDay();
  }

  void postTraversal(SoAction * action, SoNode * parent, SoNode * child)
  {
    if (!this->isActive(action)) return;

    static int synchronousgl = -1;
    if (synchronousgl == -1) {
      const char * env = coin_getenv(SoDBP::EnvVars::COIN_PROFILER_SYNCGL);
      synchronousgl = (env && (atoi(env) > 0)) ? 1 : 0;
    }

    if (synchronousgl && action->isOfType(SoGLRenderAction::getClassTypeId()))
      glFinish();

    const SbTime posttime(SbTime::getTimeOfDay());

    SoState * state = action->getState();
    SoProfilerElement * profilerelt = SoProfilerElement::get(state);
    assert(profilerelt);

    profilerelt->setTimingProfile(child, posttime - pretime, parent);
  }

  bool isActive(SoAction * action)
  {
    SoState * state = action->getState();
    if (SoProfiler::isActive() &&
        state->isElementEnabled(SoProfilerElement::getClassStackIndex()))
      return true;
    return false;
  }

private:
  SbTime pretime;

};

#else // !HAVE_SCENE_PROFILING

// The version for when no profiling is being done

class SoNodeProfiling {
public:
  SoNodeProfiling(void)
  {
  }

  void preTraversal(SoAction * a, SoNode * p, SoNode * c)
  {
  }

  void postTraversal(SoAction * a, SoNode * p, SoNode * c)
  {
  }

};

#endif // !HAVE_SCENE_PROFILING

