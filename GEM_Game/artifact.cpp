#include "artifact.h"

/* ================= DEFAULT CONSTRUCTOR ================= */

Artifact::Artifact()
    : Item()
{

}

/* ================= PARAMETERIZED CONSTRUCTOR ================= */

Artifact::Artifact(
    std::string name,
    bool collected
    )
    : Item(
          name,
          collected,
          "Artifact"
          )
{

}
